#include "corridor.h"
#include "qlist.h"

Corridor::Corridor()
{
}

Corridor::Corridor(int startX, int startY, int endX, int endY, std::string title, QGraphicsScene* scene)
    : startX(startX), startY(startY), endX(endX), endY(endY), titleCor(title), currentPeoples(0), sceneCor(scene) {
}

int Corridor::getStartX() const { return startX; }

int Corridor::getStartY() const { return startY; }

int Corridor::getEndX() const { return endX; }

int Corridor::getEndY() const { return endY; }

int Corridor::getCapacity() const { return capacity; }

int Corridor::getCurrentPeoples() const { return currentPeoples; }

QGraphicsScene Corridor::getScene()  { return sceneCor; }



void Corridor::enterCorridor() {
    if (currentPeoples < capacity) {
        currentPeoples++;
    } else {
        std::cout << "The corridor is full. Cannot enter." << std::endl;
    }
}

void Corridor::exitCorridor() {
    if (currentPeoples > 0) {
        currentPeoples--;
    } else {
        std::cout << "The corridor is empty. Cannot exit." << std::endl;
    }
}

int Corridor::getCorridorByCoordinates(int x, int y) {
    // Проверка допустимости координат
    if (x >= getStartX() && x <= getEndX() && y <= getStartY() && y >= getEndY()) {
        return 1;
    }
    return 0;
}

bool Corridor::intersects(int x, int y, int radius) const
{
    // Проверяем пересечение с границами стены
    if (x - radius <= startX || x + radius >= endX || y - radius <= startY || y + radius >= endY) {
        return true;
    }
    return false;
}

std::string Corridor::getTitle()
{
    return titleCor;
}

int Corridor::inOtherCorridor(QList<Corridor*> corridors) const {
    for (const auto& corridor : corridors) {
        if (corridor != this){
            return 1;
        }
    }
    return 0;
}


Corridor::~Corridor(){}

