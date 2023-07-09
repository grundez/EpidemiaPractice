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
    Areal(int w, int h, QGraphicsScene* arealScene);
    Areal();
    ~Areal();
    int PeoplesCount;
    int getWidthAreal();
    int getHeightAreal();
    void addNewPeople(People* People);
    void addNewRadius(int radius);
    void addCorridor(Corridor *corridor);
    void enterCorridor(int corridorIndex);
    void exitCorridor(int corridorIndex);
    void printCorridorStatus() const;
    int getSafeRadius();
    std::string getTitle();
    QGraphicsScene* getScene();

    QList<People*> getOtherPeoples() const;
    QList<Corridor*> getZone() const;

private:
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
