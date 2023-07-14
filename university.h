#ifndef Areal_H
#define Areal_H

#include <QList>
#include <iostream>
#include "corridor.h"

// Перед объявлением класса People добавляем forward declaration
class People;

class Areal : public Corridor
{
public:
    Areal(int w, int h);
    Areal();
    ~Areal();
    int PeoplesCount;
    int getWidthAreal();
    int getHeightAreal();
    void addNewPeople(People* People);
    void addCorridor(Corridor *corridor);
    void enterCorridor(int corridorIndex);
    void exitCorridor(int corridorIndex);
    void printCorridorStatus() const;

    QList<People*> getOtherPeoples() const;
    QList<Corridor*> zone;

private:
    int width;
    int height;
    //std::vector<Corridor> corridors;
    QList<People*> otherPeoples;

};

#endif // Areal_H
