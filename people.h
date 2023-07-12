#ifndef PEOPLE_H
#define PEOPLE_H

#include "qtimer.h"
#include <QObject>
#include <QGraphicsEllipseItem>
#include <queue>
#include <vector>
#include <string>


const int inf = 999999;

// Перед объявлением класса University добавляем forward declaration
class Areal;

class People : public QObject, public QGraphicsEllipseItem
{
public:
    People(int r, Areal* areal, std::string st);
    People(int radius, Areal* safeA,Areal* quarA, std::string st);
    ~People();
    int idCounter = 0;
    std::string getStatus();
    int getRadius();
    int getSpeed();;
    int getId();
    int getX();
    int getY();
    void initPos(int x,int y);
    void stopMoves();
    QColor getColor();
    void setFillColor(const QColor& col);
    void setQuar(Areal* quar);
    void exitQuar(Areal* safe);
    int getSafeDistance();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool wasInQuar = false;
    bool recoveredFromQuar = false;
    bool alreadyChanged = false; // Флаг для отслеживания изменения статуса
    bool quarNeeds = false;

    int infectiousChanse;
    int deadChanse;
    int invisInfectiousChanse;

    void addNewSusceptible(int newCount);
    void addNewInfected(int newCount);
    void addNewAsymptomatic(int newCount);
    void addNewRecovered(int newCount);
    void addNewDeaths(int newCount);

    QVector<int> getSusceptible();
    QVector<int> getInfected();
    QVector<int> getAsymptomatic();
    QVector<int> getRecovered();
    QVector<int> getDeaths();

    int infec = 0;
    int suscept = 0;
    int recovered = 0;
    int invis = 0;
    int dead = 0;
   // QTimer quarTimer;         // Таймер для проверки карантина
    //QTimer recoveredTimer;    // Таймер для проверки выздоровления

public slots:
    void quarCheck();
    void recoveredCheck();

protected:
    void advance(int step) override;

private:
    std::string status;
    int status_int;
    int count;
    QColor color;
    int radius;
    qreal speed=0;
    int idPeople;
    int steps;
    int randMoveX;
    int randMoveY;    
    int xpos;
    int ypos;
    int safeDistance;
    qreal angle=0;
    qreal PeopleEyeDirection=0;

    Areal* areal; // Добавляем указатель на объект areal
    Areal* safeAreal;
    Areal* quarAreal;
    Areal* statAreal;

    QVector<int> dataNonInfected;
    QVector<int> dataInfected;
    QVector<int> dataAsymptomatic;
    QVector<int> dataRecovered;
    QVector<int> dataDeaths;

    //QTimer quarTimer;         // Таймер для проверки карантина
    //QTimer recoveredTimer;    // Таймер для проверки выздоровления
};

#endif // PEOPLE_H
