#ifndef PEOPLE_H
#define PEOPLE_H

#include "qtimer.h"
#include <QObject>
#include <QGraphicsEllipseItem>
#include <queue>
#include <vector>

const int inf = 999999;

// Перед объявлением класса University добавляем forward declaration
class Areal;

class People : public QObject, public QGraphicsEllipseItem
{
public:
    People(int radius, Areal* safeA,Areal* quarA, std::string st);
    ~People();
    int idCounter = 0;
    int getRadius();
    int getSpeed();
    int getX();
    int getY();
    void initPos(int x,int y);
    void stopMoves();
    QColor getColor();
    void setFillColor(const QColor& col);
    std::string status;
    void setQuar(Areal* quar);
    void exitQuar(Areal* safe);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool recovered;

protected:
    void advance(int step) override;

private:
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
    QTimer statusTimer;
    qreal angle=0;
    qreal PeopleEyeDirection=0;

    Areal* areal; // Добавляем указатель на объект areal

    Areal* safeAreal;
    Areal* quarAreal;
};

#endif // PEOPLE_H
