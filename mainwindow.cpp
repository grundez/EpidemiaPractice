#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "includer.h"
#include "QPainter"
#define frame 1000/40

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), EpidemiaSettings()
{
    ui->setupUi(this);
    setStyleSheet("background-color: rgb(15, 15, 15);");
    setWindowTitle("Pandemia model");
    safeScene = new QGraphicsScene(0,0,717,607,this);
    quarScene = new QGraphicsScene(650,260,387,387,this);
    statScene = new QGraphicsScene(0,0,18,140,this);
    ui->graphicsView->setStyleSheet("border: 2px solid white;");
    QColor backgroundColor = QColor(15,15,15);  // Цвет фона (в данном случае - светло-серый)
    ui->graphicsView->setBackgroundBrush(backgroundColor);  // Установка кисти с цветом фона

    ui->graphicsView_3->setScene(statScene);
    ui->graphicsView_3->setStyleSheet("border: 0px;");
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


    safeAreal = new Areal(45, 45, (safeScene->width()-45),(safeScene->height()-45), safeScene);
    quarAreal = new Areal(685, 285, (quarScene->width()-45), (quarScene->height()-45), quarScene);
    statAreal = new Areal(0,0, statScene->width(), statScene->height(), statScene);

    Corridor *safe = new Corridor(25,25,690,580, "safe", safeScene);
    Corridor *quar = new Corridor(670,280,1020,620, "quar", quarScene);
    Corridor *socialZone = new Corridor(305,255,390,340, "social", safeScene);

    safeAreal->addCorridor(safe);
    //safeAreal->addCorridor(socialZone);
    quarAreal->addCorridor(quar);

    ui->label_4->hide();
    ui->safeValueLable->hide();
    ui->deadLableValue->hide();
    ui->InfecLableValue->hide();
    ui->notInfecLableValue->hide();
    ui->recoveredLableValue->hide();
    ui->inviInfecLableValue->hide();

    ui->label_9->hide();
    ui->label_10->hide();
    ui->label_11->hide();
    ui->label_12->hide();
    ui->label_13->hide();
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
        ui->safeValueLable->setStyleSheet("QLabel { font-size:14pt; font-weight:700; color: White }");
        ui->safeValueLable->setText("available");
    }
    if(safeAreal->getOtherPeoples().size() > 20 && epidemia){
        safeValueStr = QString::number(people->getSafeDistance());
        ui->safeValueLable->setStyleSheet("QLabel { font-size:14pt; font-weight:700; color: White }");
        ui->safeValueLable->setText(safeValueStr);
    }
}

void MainWindow::updateStatistic()
{
    int infected = 0;
    int invisInfected = 0;
    int recovered = 0;
    int suscepteble = 0;
    int dead = 0;

    for(const auto& people : safeAreal->getOtherPeoples()){
        if (people->getStatus() == "Infectious"){
            infected++;
        }
        if(people->getStatus() == "Susceptible"){
            suscepteble++;
        }
        else if(people->getStatus() == "Recovered"){
            recovered++;
        }
        else if(people->getStatus() == "InvisibleInfectious"){
            invisInfected++;
        }
        else if(people->getStatus() == "Dead"){
            dead++;
        }
    }

    infValueStr = QString::number(infected);
    invisInfValueStr = QString::number(invisInfected);
    recValueStr = QString::number(recovered);
    nonInfValueStr = QString::number(suscepteble);
    deadValueStr = QString::number(dead);

    ui->deadLableValue->show();
    ui->InfecLableValue->show();
    ui->notInfecLableValue->show();
    ui->recoveredLableValue->show();
    ui->inviInfecLableValue->show();

    ui->deadLableValue->setStyleSheet("QLabel { font-size:14pt; font-weight:700; color: White }");
    ui->deadLableValue->setText(deadValueStr);
    ui->InfecLableValue->setStyleSheet("QLabel { font-size:14pt; font-weight:700; color: White }");
    ui->InfecLableValue->setText(infValueStr);
    ui->notInfecLableValue->setStyleSheet("QLabel { font-size:14pt; font-weight:700; color: White }");
    ui->notInfecLableValue->setText(nonInfValueStr);
    ui->recoveredLableValue->setStyleSheet("QLabel { font-size:14pt; font-weight:700; color: White }");
    ui->recoveredLableValue->setText(recValueStr);
    ui->inviInfecLableValue->setStyleSheet("QLabel { font-size:14pt; font-weight:700; color: White }");
    ui->inviInfecLableValue->setText(invisInfValueStr);
}

void MainWindow::on_addButton_clicked()
{
    // Лейблы и элипсы для отображения статистики
    ui->label_9->show();
    ui->label_10->show();
    ui->label_11->show();
    ui->label_12->show();
    ui->label_13->show();

    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(0, 0, 20, 20);
    circle->setPos(0,-30);
    circle->setBrush(QColor(0,155,255));
    statScene->addItem(circle);

    QGraphicsEllipseItem* circle1 = new QGraphicsEllipseItem(0, 0, 20, 20);
    circle1->setPos(0,15);
    circle1->setBrush(QColor(255,0,0));
    statScene->addItem(circle1);

    QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem(0, 0, 20, 20);
    circle2->setPos(0,60);
    circle2->setBrush(QColor(255,255,155));
    statScene->addItem(circle2);

    QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem(0, 0, 20, 20);
    circle3->setPos(0,105);
    circle3->setBrush(QColor(0,255,155));
    statScene->addItem(circle3);

    QGraphicsEllipseItem* circle4 = new QGraphicsEllipseItem(0, 0, 20, 20);
    circle4->setPos(0,150);
    circle4->setBrush(QColor(100,100,100));
    statScene->addItem(circle4);

    //Отключить старт кнопку
    ui->addButton->setEnabled(false);
    ui->addButton_2->setEnabled(true);

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

        safeAreal->addNewPeople(people);
        qreal x = QRandomGenerator::global()->bounded(safeSceneRect.width());
        qreal y = QRandomGenerator::global()->bounded(safeSceneRect.height());
        people->setPos(safeSceneRect.x() + x, safeSceneRect.y() + y);
        people->setZValue(1);
        safeScene->addItem(people);

        people->infectiousChanse = ui->spinBoxInfectious->value();
        people->deadChanse = ui->spinBoxDead->value();
        people->invisInfectiousChanse = ui->spinBoxInvis->value();

        EpidemiaSettings(ui->spinBox->value(), ui->spinBoxInfectious->value(),
                         ui->spinBoxDead->value(), ui->spinBoxInvis->value(), ui->checkBox->isChecked(),0,0);

        if(ui->checkBox->isChecked())
            people->quarNeeds = true;

        if(ui->checkBox_2->isChecked())
            people->socialZoneNeeds = true;
    }

    // ФУНКЦИЯ ГРАФИКА (В РАЗРАБОТКЕ)
    //ui->widget->show();
    //graphWidget->setInfected(people->getInfected());
    //graphWidget->setNonInfected(people->getSusceptible());
    //graphWidget->setAsymptomatic(people->getAsymptomatic());
    //graphWidget->setRecovered(people->getRecovered());
    //graphWidget->setDeaths(people->getDeaths());
    //graphWidget->drawGraph();

    // Прорисовка социальной зоны
    if(people->socialZoneNeeds == true){
        QPen pen;
        pen.setWidth(3);
        pen.setColor(QColor(255,50,255));
        QGraphicsRectItem* socialZone = new QGraphicsRectItem(305, 250, 100, 100);
        QColor color = QColor(15, 15, 15);
        socialZone->setPen(pen);
        socialZone->setOpacity(1);
        socialZone->setBrush(color);
        socialZone->setZValue(0);
        safeScene->addItem(socialZone);
    }

    // Подключение таймеров для отображения анимаций/изменений
    QObject::connect(&animTimer, &QTimer::timeout, safeScene, &QGraphicsScene::advance);
    QObject::connect(&animTimer, &QTimer::timeout, quarScene, &QGraphicsScene::advance);
    QObject::connect(&animTimer, &QTimer::timeout, this, &MainWindow::updateSafeValue);
    QObject::connect(&animTimer, &QTimer::timeout, this, &MainWindow::updateStatistic);

    // Запуск таймера для отображения событий на сцене
    animTimer.start(frame);
}


void MainWindow::on_addButton_2_clicked()
{
    // Перенастройка виджетов
    ui->addButton->setEnabled(true);
    ui->addButton_2->setEnabled(false);

    ui->label_9->hide();
    ui->label_10->hide();
    ui->label_11->hide();
    ui->label_12->hide();
    ui->label_13->hide();
    ui->deadLableValue->hide();
    ui->InfecLableValue->hide();
    ui->notInfecLableValue->hide();
    ui->recoveredLableValue->hide();
    ui->inviInfecLableValue->hide();

    // Перенастройка виджетов окна
    safeScene->clear();
    quarScene->clear();
    statScene->clear();
    ui->label_4->hide();
    ui->safeValueLable->hide();

    //  Очистка списков
    safeAreal->deleteListPeoples();
    quarAreal->deleteListPeoples();
    statAreal->deleteListPeoples();

    // Остановка таймеров анимаций
    animTimer.stop();
    QObject::disconnect(&animTimer, &QTimer::timeout, safeScene, &QGraphicsScene::advance);
    QObject::disconnect(&animTimer, &QTimer::timeout, quarScene, &QGraphicsScene::advance);
    QObject::disconnect(&animTimer, &QTimer::timeout, this, &MainWindow::updateSafeValue);
    QObject::disconnect(&animTimer, &QTimer::timeout, this, &MainWindow::updateStatistic);
}
