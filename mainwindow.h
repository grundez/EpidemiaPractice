#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "people.h"
#include "qtimer.h"
#include "areal.h"
#include "graphwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void updateSafeValue();
    void updateStatistic();

private slots:
    void on_addButton_clicked();    // СЛОТ-обработчик нажатия кнопки добавления

    void on_addButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *safeScene;
    QGraphicsScene *quarScene;
    QGraphicsScene *statScene;

    // Таймеры для отображения событий
    QTimer animTimer;
    QTimer recoveredTimer;
    QTimer quarTimer;


    People* people;
    Areal* safeAreal;
    Areal* quarAreal;
    Areal* statAreal;

    // Значение дистанции
    QString safeValueStr;

    GraphWidget *graphWidget;

};
#endif // MAINWINDOW_H
