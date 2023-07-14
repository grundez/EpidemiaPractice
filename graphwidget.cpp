#include "graphwidget.h"
#include <QPainter>

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent)
{
    // Инициализация данных для рисования графика, если требуется
}

void GraphWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void GraphWidget::drawGraph()
{
    QPainter painter(this);
    dataNonInfected = {100,50,25};
    dataInfected = {15, 25};
    dataAsymptomatic = {55, 155};
    dataRecovered = {15,5};
    dataDeaths = {10, 105};

    // Определяем размеры графика
    int graphWidth = 300;
    int graphHeight = 300;

    // Определяем максимальное значение на оси Y
    int maxY = 0;
    for (int i = 0; i < dataNonInfected.size()-1; ++i) {
        maxY = qMax(maxY, qMax(dataNonInfected[i],
                               qMax(dataInfected[i], qMax(dataAsymptomatic[i],
                               qMax(dataRecovered[i], dataDeaths[i])))));
    }

    // Определяем шаги для осей X и Y
    int stepX = graphWidth / (dataNonInfected.size() - 1);
    int stepY = graphHeight / maxY;

    // Рисуем фон графика
    painter.fillRect(0, 0, graphWidth, graphHeight, Qt::black);

    // Рисуем оси X и Y
    painter.setPen(Qt::white);
    painter.drawLine(0, graphHeight, graphWidth, graphHeight); // Ось X
    painter.drawLine(0, 0, 0, graphHeight); // Ось Y

    // Рисуем линию для не заболевших людей (1 линия)
    painter.setPen(Qt::green);
    for (int i = 0; i < dataNonInfected.size() - 1; ++i) {
        int x1 = i * stepX;
        int y1 = graphHeight - dataNonInfected[i] * stepY;
        int x2 = (i + 1) * stepX;
        int y2 = graphHeight - dataNonInfected[i + 1] * stepY;
        painter.drawLine(x1, y1, x2, y2);
    }

    // Рисуем линию для заболевших людей (2 линия)
    painter.setPen(Qt::red);
    for (int i = 0; i < dataInfected.size() - 1; ++i) {
        int x1 = i * stepX;
        int y1 = graphHeight - dataInfected[i] * stepY;
        int x2 = (i + 1) * stepX;
        int y2 = graphHeight - dataInfected[i + 1] * stepY;
        painter.drawLine(x1, y1, x2, y2);
    }

    // Рисуем линию для бессимптомно заболевших людей (3 линия)
    painter.setPen(Qt::blue);
    for (int i = 0; i < dataAsymptomatic.size() - 1; ++i) {
        int x1 = i * stepX;
        int y1 = graphHeight - dataAsymptomatic[i] * stepY;
        int x2 = (i + 1) * stepX;
        int y2 = graphHeight - dataAsymptomatic[i + 1] * stepY;
        painter.drawLine(x1, y1, x2, y2);
    }

    // Рисуем линию для выздоровевших людей (4 линия)
    painter.setPen(Qt::cyan);
    for (int i = 0; i < dataRecovered.size() - 1; ++i) {
        int x1 = i * stepX;
        int y1 = graphHeight - dataRecovered[i] * stepY;
        int x2 = (i + 1) * stepX;
        int y2 = graphHeight - dataRecovered[i + 1] * stepY;
        painter.drawLine(x1, y1, x2, y2);
    }

    // Рисуем линию для умерших людей (5 линия)
    painter.setPen(Qt::black);
    for (int i = 0; i < dataDeaths.size() - 1; ++i) {
        int x1 = i * stepX;
        int y1 = graphHeight - dataDeaths[i] * stepY;
        int x2 = (i + 1) * stepX;
        int y2 = graphHeight - dataDeaths[i + 1] * stepY;
        painter.drawLine(x1, y1, x2, y2);
    }
}


void GraphWidget::setNonInfected(QVector<int> n)
{
    dataNonInfected = n;
}

void GraphWidget::setInfected(QVector<int> i)
{
    dataInfected = i;
}

void GraphWidget::setAsymptomatic(QVector<int> a)
{
    dataAsymptomatic = a;
}

void GraphWidget::setRecovered(QVector<int> r)
{
    dataRecovered = r;
}

void GraphWidget::setDeaths(QVector<int> d)
{
    dataDeaths = d;
}
