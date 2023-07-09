#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "includer.h"
#define frame 1000/33

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("background-color: rgb(15, 15, 15);");
    setWindowTitle("Pandemia model");
    safeScene = new QGraphicsScene(0,0,550,460,this);
    quarScene = new QGraphicsScene(650,420,147,122,this);
    ui->graphicsView->setStyleSheet("border: 2px solid white;");
    QColor backgroundColor = QColor(15,15,15);  // Цвет фона (в данном случае - светло-серый)
    ui->graphicsView->setBackgroundBrush(backgroundColor);  // Установка кисти с цветом фона

    ui->graphicsView_2->setScene(quarScene);
    ui->graphicsView_2->setStyleSheet("border: 2px solid red;");
    ui->graphicsView_2->setBackgroundBrush(backgroundColor);
    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView_2->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView_2->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView_2->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView_2->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Quarantine"));

    ui->graphicsView->setScene(safeScene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Areal"));

    ui->spinBox->setStyleSheet("QSpinBox { color: white; }");

    safeAreal = new Areal(safeScene->width(),safeScene->height(), safeScene);
    quarAreal = new Areal(quarScene->width(), quarScene->height(), quarScene);

    Corridor *safe = new Corridor(0,0,550,460, "safe", safeScene);
    Corridor *quar = new Corridor(660,430,780,530, "quar", quarScene);
    safeAreal->addCorridor(safe);
    quarAreal->addCorridor(quar);

    QObject::connect(&animTimer, &QTimer::timeout, safeScene, &QGraphicsScene::advance);
    QObject::connect(&animTimer, &QTimer::timeout, quarScene, &QGraphicsScene::advance);
    ui->label_4->hide();
    ui->safeValueLable->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    int PeopleCount = ui->spinBox->value();
    QRectF safeSceneRect = safeScene->sceneRect();
    QRectF quarSceneRect = quarScene->sceneRect();
    ui->safeValueLable->show();
    ui->label_4->show();

    for (int i = 0; i < PeopleCount; ++i) {
        int chanse = QRandomGenerator::global()->bounded(1,101);
        if(chanse<26){
            int chanseInvis = QRandomGenerator::global()->bounded(1,101);
            if (chanseInvis<26){
                people = new People(QRandomGenerator::global()->bounded(15, 20), safeAreal, quarAreal, "InvisibleInfectious");
            }
            else{
                people = new People(QRandomGenerator::global()->bounded(15, 20), safeAreal, quarAreal, "Infectious");//Зараженный
            }
        }
        else{
            people = new People(QRandomGenerator::global()->bounded(15, 20), safeAreal, quarAreal, "Susceptible"); //Не болевшие
        }

        safeAreal->addNewPeople(people);
        qreal x = QRandomGenerator::global()->bounded(safeSceneRect.width());
        qreal y = QRandomGenerator::global()->bounded(safeSceneRect.height());
        people->setPos(safeSceneRect.x() + x, safeSceneRect.y() + y);
        safeScene->addItem(people);
    }

    animTimer.start(1000 / 33);
}

