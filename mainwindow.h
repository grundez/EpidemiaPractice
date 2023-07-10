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
public slots:
    void updateSafeValue();

private slots:
    void on_addButton_clicked();    // СЛОТ-обработчик нажатия кнопки добавления

    void on_addButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *safeScene;
    QGraphicsScene *quarScene;

    QTimer animTimer;
    QTimer recoveredTimer;
    QTimer quarTimer;

    People* people;
    Areal* safeAreal;
    Areal* quarAreal;

    QString safeValueStr;
};
#endif // MAINWINDOW_H
