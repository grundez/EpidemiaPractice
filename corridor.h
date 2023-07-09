#ifndef CORRIDOR_H
#define CORRIDOR_H

#include "qgraphicsscene.h"
#include "qlist.h"
#include <iostream>

class Corridor {
public:
    Corridor(int startX, int startY, int endX, int endY, std::string title, QGraphicsScene* Scene);
    Corridor();
    ~Corridor();
    // Методы для получения информации о коридоре
    int getStartX() const;
    int getStartY() const;
    int getEndX() const;
    int getEndY() const;
    int getCapacity() const;
    int getCurrentPeoples() const;
    bool intersects(int x, int y, int radius) const;
    std::string getTitle();
    QGraphicsScene getScene();

    // Методы для управления количеством студентов в коридоре
    void enterCorridor();
    void exitCorridor();
    int inOtherCorridor(QList<Corridor*> corridors) const;
    int getCorridorByCoordinates(int x, int y);

private:
    std::string titleCor;
    int startX;
    int startY;
    int endX;
    int endY;
    int capacity;
    int currentPeoples;
    QGraphicsScene* sceneCor;
};

#endif // CORRIDOR_H
