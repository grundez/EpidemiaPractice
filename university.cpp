#include "Areal.h"

Areal::Areal()
{
    width = 0;
    height = 0;
}

Areal::Areal(int w, int h)
{
    width = w;
    height = h;
}

Areal::~Areal(){
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

void Areal::addCorridor(Corridor *corridor) {
    zone.append(corridor);
}

QList<People*> Areal::getOtherPeoples() const {
    return otherPeoples;
};

void Areal::printCorridorStatus() const {
    for (int i = 0; i < zone.size(); i++) {
        std::cout << "Zone " << i+1 << ": ";
        std::cout << "Current people: " << zone[i]->getCurrentPeoples();
    }
}

