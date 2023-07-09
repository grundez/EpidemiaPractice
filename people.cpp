#include "includer.h"
#include "people.h"
#include "Areal.h"

constexpr qreal Pi = M_PI;
constexpr qreal TwoPi = 2 * M_PI;

People::People(int r, Areal* safeA,Areal* quarA, std::string st) : QGraphicsEllipseItem(0)
{
    idCounter++;
    radius = r;
    speed = 0;
    idPeople = idCounter;
    steps=0;
    randMoveX = 0;
    randMoveY = 0;
    xpos =0;
    ypos = 0;
    areal = safeA; //Все появляются сначала в safe
    safeAreal = safeA;
    quarAreal = quarA;
    angle = 0;
    count =0;
    setRotation(QRandomGenerator::global()->bounded(360 * 16));
    status = st;
    statusTimer.start(1000/150);
}

int People::getRadius(){
    return radius;
}

void People::initPos(int x,int y){
    // Центр
    xpos = x;
    ypos = y;
}

void People::stopMoves()
{
    randMoveX = 0;
    randMoveY = 0;
}

QColor People::getColor()
{
    return color;
}

void People::setFillColor(const QColor &col)
{
    color = col;
    update();
}

void People::setQuar(Areal* quar)
{
    areal = quar;
    setPos(730, 480);
}

void People::exitQuar(Areal* safe)
{
    areal = safe;
    setPos(270,230);
}

int People::getSpeed(){
    return speed;
}

int People::getX()
{
    return xpos;
}

int People::getY()
{
    return ypos;
}

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

QRectF People::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
}

QPainterPath People::shape() const
{
    QPainterPath path;
    path.addRect(-radius, -radius, radius, radius);
    return path;
}

void People::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Body
    if (status == "Infectious"){
        color = QColor(255,0,0);
    }
    if(status == "Susceptible"){
        color = QColor(0,155,255);
    }
    else if(status == "Recovered"){
        color = QColor(0,255,155);
    }
    else if(status == "InvisibleInfectious"){
        color = QColor(255,255,155);
    }
    else if(status == "Dead"){
        color = QColor(100,100,100);
    }

    //painter->setBrush(scene()->collidingItems(this).isEmpty() ? QColor(0,155,255) : Qt::red);
    painter->setBrush(color);
    painter->drawEllipse(0, 0, radius, radius);
}

void People::advance(int step)
{
    if (!step)
        return;

    if(status != "Dead"){
    // Don't move too far away
        for(const auto& zone : areal->getZone()){
            if(zone->getTitle() == "safe"){
                QLineF lineToCenter(QPointF(0, 0), mapFromScene(270, 230));
                if (lineToCenter.length() > 150) {
                    qreal angleToCenter = std::atan2(lineToCenter.dy(), lineToCenter.dx());
                    angleToCenter = normalizeAngle((Pi - angleToCenter) + Pi / 2);

                    if (angleToCenter < Pi && angleToCenter > Pi / 4) {
                        // Rotate left
                        angle += (angle < -Pi / 2) ? 0.25 : -0.25;
                    } else if (angleToCenter >= Pi && angleToCenter < (Pi + Pi / 2 + Pi / 4)) {
                        // Rotate right
                        angle += (angle < Pi / 2) ? 0.25 : -0.25;
                    }
                } else if (::sin(angle) < 0) {
                    angle += 0.25;
                } else if (::sin(angle) > 0) {
                    angle -= 0.25;
                }
            }
            else{
                QLineF lineToCenter(QPointF(0, 0), mapFromScene(740, 470));
                if (lineToCenter.length() > 5) {
                    qreal angleToCenter = std::atan2(lineToCenter.dy(), lineToCenter.dx());
                    angleToCenter = normalizeAngle((Pi - angleToCenter) + Pi / 2);

                    if (angleToCenter < Pi && angleToCenter > Pi / 4) {
                        // Rotate left
                        angle += (angle < -Pi / 2) ? 0.25 : -0.25;
                    } else if (angleToCenter >= Pi && angleToCenter < (Pi + Pi / 2 + Pi / 4)) {
                        // Rotate right
                        angle += (angle < Pi / 2) ? 0.25 : -0.25;
                    }
                } else if (::sin(angle) < 0) {
                    angle += 0.25;
                } else if (::sin(angle) > 0) {
                    angle -= 0.25;
                }
            }
        }


        // Try not to crash with any other mice
        const QList<QGraphicsItem *> dangerItem = scene()->items(QPolygonF()
                                                                 << mapToScene(0, 0)
                                                                 << mapToScene(-30, -50)
                                                                 << mapToScene(30, -50));

        for (const QGraphicsItem *item : dangerItem) {
            if (item == this){
                continue;
            }

            QLineF lineToPeople(QPointF(0, 0), mapFromItem(item, 0, 0));
            qreal angleToPeople = std::atan2(lineToPeople.dy(), lineToPeople.dx());
            angleToPeople = normalizeAngle((Pi - angleToPeople) + Pi / 2);

            if (angleToPeople >= 0 && angleToPeople < Pi / 2) {
                // Rotate right
                angle += 0.5;
            } else if (angleToPeople <= TwoPi && angleToPeople > (TwoPi - Pi / 2)) {
                // Rotate left
                angle -= 0.5;
            }
        }

        for(const auto& people : areal->getOtherPeoples()){
            if(people != this
            && ((people->status == "Infectious" && status == "Susceptible")
                    || (people->status == "InvisibleInfectious" && status == "Susceptible"))){
                QLineF lineToPeople(QPointF(0, 0), mapFromItem(people, 0, 0));
                qreal distanceToPeople = lineToPeople.length();  // Расстояние до больного
                qreal safeDistance = radius + people->getRadius();  // Безопасное расстояние

                //Запись новой дистанции среди людей
                int intDistance = static_cast<int>(distanceToPeople);
                areal->addNewRadius(intDistance);

                if(distanceToPeople < safeDistance && (QRandomGenerator::global()->bounded(1,101) < 26)){
                    if(QRandomGenerator::global()->bounded(1,101) < 76){
                        status = "Infectious";
                    }
                    else{
                        status = "InvisibleInfectious";
                    }
                    //printf("Infectious!!\n");
                }
            }
        }

        // Add some random movement
        if (dangerItem.size()>1 && QRandomGenerator::global()->bounded(10) == 0) {
            if (QRandomGenerator::global()->bounded(1))
                angle += QRandomGenerator::global()->bounded(1 / 500.0);
            else
                angle -= QRandomGenerator::global()->bounded(1 / 500.0);
        }

        speed += (-50 + QRandomGenerator::global()->bounded(100)) / 100.0;
        qreal dx = ::sin(angle) * 10;
        PeopleEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;


            setRotation(rotation() + dx);
            setPos(mapToParent(0, -(3 + cos(speed) * 3)));

        // Ограничение перемещения
            qreal xPos = pos().x();
            qreal yPos = pos().y();

            //В декартовы координаты
            int x = static_cast<int>(pos().x());
            int y = static_cast<int>(pos().y());

            for(const auto& zone : areal->getZone()){
                if (x < zone->getStartX()) {
                    setPos(zone->getStartX(), pos().y());
                    angle = Pi;
                } else if (x > zone->getEndX()) {
                    setPos(zone->getEndX(), pos().y());
                    angle = Pi;
                }

                if (y < zone->getStartY()) {
                    setPos(pos().x(), zone->getStartY());
                    angle = Pi;
                } else if (y > zone->getEndY()) {
                    setPos(pos().x(), zone->getEndY());
                    angle = Pi;
                }
                //printf("Zone [%d %d, %d %d]", zone->getStartX(), zone->getStartY(), zone->getEndX(),zone->getEndY());
            }
    }

    for (const auto& zone : areal->getZone()){
        if ((status == "Infectious" || status == "InvisibleInfectious")) {
            QTimer::singleShot(5000, [this]() {
                status = "Recovered";
            });
        }
        if (status == "Recovered" && zone->getTitle() == "quar") {
                QTimer::singleShot(2000, [this]() {
                    exitQuar(safeAreal);
                    safeAreal->getScene()->addItem(this);          
                });
        }
        if (status == "Infectious" && zone->getTitle() == "safe") {
                QTimer::singleShot(2000, [this]() {
                    setQuar(quarAreal);
                    quarAreal->getScene()->addItem(this);  
                });
        }
        break;
    }


    /*
    //Выздоровление(5сек)
    if ((status == "Infectious" || status == "InvisibleInfectious") && status!="Dead") {
        QTimer::singleShot(7000, [this]() {status = "Recovered";});
    }


    for(const auto& people : safeAreal->getOtherPeoples()){
        if(people->status == "Infectious"){
            people->setQuar(quarAreal);
            quarScene->addItem(people);
        }
        else if (people->status == "Recovered"){
            people->exitQuar(safeAreal);
            safeScene->addItem(people);
        }
    }
    */

    //printf("Human {%d, %d}" ,x, y);
}

People::~People(){
}


