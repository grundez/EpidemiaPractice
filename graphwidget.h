#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QStyleOptionGraphicsItem>

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWidget(QWidget *parent = nullptr);
    //void paintEvent(QPaintEvent*);
    void drawGraph();

    void setNonInfected(QVector<int>);
    void setInfected(QVector<int>);
    void setAsymptomatic(QVector<int>);
    void setRecovered(QVector<int>);
    void setDeaths(QVector<int>);

public slots:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QVector<int> dataNonInfected;
    QVector<int> dataInfected;
    QVector<int> dataAsymptomatic;
    QVector<int> dataRecovered;
    QVector<int> dataDeaths;
};

#endif // GRAPHWIDGET_H
