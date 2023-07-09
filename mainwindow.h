#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "people.h"
#include "qtimer.h"
#include "areal.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();    // СЛОТ-обработчик нажатия кнопки добавления

private:
    Ui::MainWindow *ui;
    QGraphicsScene *safeScene;
    QGraphicsScene *quarScene;
    QTimer animTimer;
    QTimer quarTimer;

    People* people;
    Areal* safeAreal;
    Areal* quarAreal;
};
#endif // MAINWINDOW_H
