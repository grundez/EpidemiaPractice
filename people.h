#ifndef PEOPLE_H
#define PEOPLE_H

#include "epidemiasettings.h"
#include <QObject>
#include <QGraphicsEllipseItem>
#include <queue>
#include <vector>
#include <string>
#include "includer.h"

// Перед объявлением класса University добавляем forward declaration
class Areal;

class People : public QObject, public QGraphicsEllipseItem, public EpidemiaSettings
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
    bool wasInSocialZone = false;

    bool socialZoneNeeds = false;
    bool quarNeeds = false;

    EpidemiaSettings settings;
    int infectiousChanse = settings.getChanseInfected();
    int deadChanse = settings.getChanseeDead();
    int invisInfectiousChanse = settings.getChanseAsymptotic();

    // Функции для точек графика
    void addNewSusceptible(int newCount);
    void addNewInfected(int newCount);
    void addNewAsymptomatic(int newCount);
    void addNewRecovered(int newCount);
    void addNewDeaths(int newCount);

    void addSusceptibleCount();
    void addInfectedCount();
    void addInvisInfectedCount();

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

    Areal* areal;
    Areal* safeAreal;
    Areal* quarAreal;
    Areal* statAreal;

    //QVector<int> dataNonInfected;
    //QVector<int> dataInfected;
    //QVector<int> dataAsymptomatic;
    //QVector<int> dataRecovered;
    //QVector<int> dataDeaths;

};

#endif // PEOPLE_H
