#ifndef Areal_H
#define Areal_H

#include <QList>
#include <iostream>
#include "corridor.h"
#include <set>


// Перед объявлением класса People добавляем forward declaration
class People;

class Areal : public Corridor
{
public:
    Areal(int rx, int ty, int w, int h, QGraphicsScene* arealScene);
    Areal();
    ~Areal();
    int PeoplesCount;
    int getWidthAreal();
    int getHeightAreal();
    int getX();
    int getY();
    void addNewPeople(People* People);
    void removePeople();
    void addNewRadius(int radius);
    void addCorridor(Corridor *corridor);
    void enterCorridor(int corridorIndex);
    void exitCorridor(int corridorIndex);
    void printCorridorStatus() const;
    void clearSafeDistances();
    void deleteListPeoples();
    int getSafeRadius();
    std::string getTitle();
    QGraphicsScene* getScene();

    QList<People*> getOtherPeoples() const;
    QList<Corridor*> getZone() const;

public slots:
    void updateStatisticInfo();

private:
    int x;
    int y;
    std::string titleAreal;
    int width;
    int height;
    int safeRadius;
    //std::vector<Corridor> corridors;
    QList<People*> otherPeoples;
    QList<int> listSafeRadiuses;
    QList<Corridor*> zoneList;
    QGraphicsScene* arealScene;

};

#endif // Areal_H
