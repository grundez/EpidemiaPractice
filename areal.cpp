#include "Areal.h"
#include <set>

Areal::Areal()
{
    width = 0;
    height = 0;
}

Areal::Areal(int w, int h, QGraphicsScene* arealS)
{
    width = w;
    height = h;
    arealScene = arealS;
}



int Areal::getWidthAreal(){
    return width;
}

int Areal::getHeightAreal(){
    return height;
}

void Areal::addNewPeople(People *People)
{
    otherPeoples.append(People);
}

void Areal::addNewRadius(int radius)
{
    listSafeRadiuses.append(radius);
}

void Areal::addCorridor(Corridor *corridor) {
    zoneList.append(corridor);
}

QList<People*> Areal::getOtherPeoples() const {
    return otherPeoples;
}

QList<Corridor *> Areal::getZone() const
{
    return zoneList;
}

void Areal::printCorridorStatus() const {
    for (int i = 0; i < zoneList.size(); i++) {
        std::cout << "Zone " << i+1 << ": ";
        std::cout << "Current people: " << zoneList[i]->getCurrentPeoples();
    }
}

int Areal::getSafeRadius(){
    int summRadius = 0;
    for(const auto& radius : listSafeRadiuses){
        summRadius += radius;
    }
    //коэф списка радиусов
    safeRadius = summRadius / listSafeRadiuses.size();
    return safeRadius;
}

std::string Areal::getTitle()
{
    return titleAreal;
}

QGraphicsScene *Areal::getScene()
{
    return arealScene;
}

Areal::~Areal(){
}

