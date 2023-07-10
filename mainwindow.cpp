#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "includer.h"
#define frame 1000/45

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("background-color: rgb(15, 15, 15);");
    setWindowTitle("Pandemia model");
    safeScene = new QGraphicsScene(0,0,550,460,this);
    quarScene = new QGraphicsScene(650,260,277,277,this);
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
    ui->spinBoxInfectious->setStyleSheet("QSpinBox { color: white; }");
    ui->spinBoxDead->setStyleSheet("QSpinBox { color: white; }");
    ui->spinBoxInvis->setStyleSheet("QSpinBox { color: white; }");

    safeAreal = new Areal(safeScene->width(),safeScene->height(), safeScene);
    quarAreal = new Areal(quarScene->width(), quarScene->height(), quarScene);

    Corridor *safe = new Corridor(0,0,550,460, "safe", safeScene);
    Corridor *quar = new Corridor(660,270,900,510, "quar", quarScene);
    safeAreal->addCorridor(safe);
    quarAreal->addCorridor(quar);

    ui->label_4->hide();
    ui->safeValueLable->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSafeValue()
{
    bool epidemia = false;
    for(const auto& people : safeAreal->getOtherPeoples()){
        if (people->getStatus() == "Infectious" || people->getStatus() == "InvisibleInfectious"){
            epidemia = true;
            break;
        }
    }

    if (safeAreal->getOtherPeoples().size() > 0){
        ui->safeValueLable->setStyleSheet("QLabel { font-size:13pt; font-weight:700; color: White }");
        ui->safeValueLable->setText("available");
    }
    if(safeAreal->getOtherPeoples().size() > 20 && epidemia){
        safeValueStr = QString::number(people->getSafeDistance());
        ui->safeValueLable->setStyleSheet("QLabel { font-size:13pt; font-weight:700; color: White }");
        ui->safeValueLable->setText(safeValueStr);
    }
}

void MainWindow::on_addButton_clicked()
{
    QObject::connect(&animTimer, &QTimer::timeout, safeScene, &QGraphicsScene::advance);
    QObject::connect(&animTimer, &QTimer::timeout, quarScene, &QGraphicsScene::advance);
    QObject::connect(&animTimer, &QTimer::timeout, this, &MainWindow::updateSafeValue);


    //Отключить старт кнопку
    ui->addButton->setEnabled(false);

    //Количество людей в модели
    int PeopleCount = ui->spinBox->value();

    //Размеры сцен для объектов
    QRectF safeSceneRect = safeScene->sceneRect();
    QRectF quarSceneRect = quarScene->sceneRect();

    //Отобразить лейблы для показа коэфа интервалов
    ui->safeValueLable->show();
    ui->label_4->show();

    // Создание объектов с учетом настроек
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


        //QObject::connect(&quarTimer, &QTimer::timeout, people, &People::quarCheck);
        //QObject::connect(&recoveredTimer, &QTimer::timeout, people, &People::recoveredCheck);
        //quarTimer.start(2000);
        //recoveredTimer.start(5000);

        safeAreal->addNewPeople(people);
        qreal x = QRandomGenerator::global()->bounded(safeSceneRect.width());
        qreal y = QRandomGenerator::global()->bounded(safeSceneRect.height());
        people->setPos(safeSceneRect.x() + x, safeSceneRect.y() + y);
        safeScene->addItem(people);
        if(ui->checkBox->isChecked()){
            people->quarNeeds = true;
        }

        people->infectiousChanse = ui->spinBoxInfectious->value();
        people->deadChanse = ui->spinBoxDead->value();
        people->invisInfectiousChanse = ui->spinBoxInvis->value();
        //else { people->quarNeeds = false; }
    } 

    // Запуск таймеров для отоброжения событий на сцене
    animTimer.start(frame);
}


void MainWindow::on_addButton_2_clicked()
{
    ui->addButton->setEnabled(true);

    safeAreal->deleteListPeoples();
    quarAreal->deleteListPeoples();

    animTimer.stop();
    //quarTimer.stop();
    //recoveredTimer.stop();

    QObject::disconnect(&animTimer, &QTimer::timeout, safeScene, &QGraphicsScene::advance);
    QObject::disconnect(&animTimer, &QTimer::timeout, quarScene, &QGraphicsScene::advance);
    QObject::disconnect(&animTimer, &QTimer::timeout, this, &MainWindow::updateSafeValue);
    //QObject::disconnect(&quarTimer, &QTimer::timeout, people, &People::quarCheck);
    //QObject::disconnect(&recoveredTimer, &QTimer::timeout, people, &People::recoveredCheck);

    safeScene->clear();
    quarScene->clear();

    ui->label_4->hide();
    ui->safeValueLable->hide();
}

