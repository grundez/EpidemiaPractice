#include "includer.h"
#include "people.h"
#include "Areal.h"

constexpr qreal Pi = M_PI;
constexpr qreal TwoPi = 2 * M_PI;

People::People(int r, Areal* safeA,Areal* quarA, string st) : QGraphicsEllipseItem(0)
{
    idCounter++;
    idPeople = idCounter;
    radius = r;
    speed = 0;
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
}

int People::getRadius(){
    return radius;
}

int People::getId(){
    return idPeople;
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

string People::getStatus()
{
    return status;
}

void People::setFillColor(const QColor &col)
{
    color = col;
    update();
}

void People::setQuar(Areal* quar)
{
    areal = quar;
    wasInQuar = true;
    //setPos(QRandomGenerator::global()->bounded(30,600),
    //       QRandomGenerator::global()->bounded(30,450));
}

void People::exitQuar(Areal* safe)
{
    areal = safe;
    recoveredFromQuar = true;
    //setPos(QRandomGenerator::global()->bounded(640,780),
    // QRandomGenerator::global()->bounded(440,530));
}

int People::getSafeDistance()
{
    safeDistance = safeAreal->getSafeRadius();
    return safeDistance;
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

void People::quarCheck()
{
    if (status == "Recovered" && areal == quarAreal) {
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [this, &timer]() {
            if (!recoveredFromQuar) {
                setPos(QRandomGenerator::global()->bounded(30, 540),
                       QRandomGenerator::global()->bounded(30, 450));
            }
            exitQuar(safeAreal);
            safeAreal->getScene()->addItem(this);
            timer->deleteLater();
        });
        timer->start(2000);
    }

    if (status == "Infectious" && areal == safeAreal) {
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [this, &timer]() {
            if (!wasInQuar) {
                setPos(QRandomGenerator::global()->bounded(640, 780),
                       QRandomGenerator::global()->bounded(440, 530));
            }
            setQuar(quarAreal);
            quarAreal->getScene()->addItem(this);
            timer->deleteLater();
        });
        timer->start(2000);
    }

    /*
    if (this->status == "Recovered" && areal==quarAreal) {
        if(!recoveredFromQuar){
            setPos(QRandomGenerator::global()->bounded(30, 650),
                   QRandomGenerator::global()->bounded(30,560));
            exitQuar(safeAreal);
            safeAreal->getScene()->addItem(this);
            safeAreal->addNewPeople(this);
        }
    }

    if (this->status == "Infectious" && areal==safeAreal) {
        if(!wasInQuar){
            setPos(QRandomGenerator::global()->bounded(730,900),
                   QRandomGenerator::global()->bounded(410,600));
            setQuar(quarAreal);
            quarAreal->getScene()->addItem(this);
            quarAreal->addNewPeople(this);
        }
    }
    */

}

void People::recoveredCheck()
{
    if ((status == "Infectious" || status == "InvisibleInfectious")) {

        QTimer::singleShot(5000, [this]() { // Добавлено захватывание по ссылке флага alreadyChanged
            if (!alreadyChanged) { // Проверка флага перед изменением статуса
                alreadyChanged = true; // Устанавливаем флаг уже измененного статуса

                if (QRandomGenerator::global()->bounded(1, 101) >= 5) {
                    status = "Recovered";
                } else {
                    status = "Dead";
                }
            }
        });
    }

    /*
    if ((this->status == "Infectious" || this->status == "InvisibleInfectious")) {
        if (!alreadyChanged) { // Проверка флага перед изменением статуса
            alreadyChanged = true; // Устанавливаем флаг уже измененного статуса

            if (QRandomGenerator::global()->bounded(1, 101) >= 5) {
                this->status = "Recovered";
            } else {
                this->status = "Dead";
            }
        }
    }
    */
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
                QLineF lineToCenter(QPointF(0, 0), mapFromScene(780, 400));
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
                && (status == "Susceptible" && (people->status == "Infectious" || people->status == "InvisibleInfectious"))){
                QLineF lineToPeople(QPointF(0, 0), mapFromItem(people, 0, 0));
                qreal distanceToPeople = lineToPeople.length();  // Расстояние до больного
                qreal safeDistance = radius + people->getRadius();  // Безопасное расстояние

                //Запись новой дистанции среди людей
                int intDistance = static_cast<int>(distanceToPeople);
                areal->addNewRadius(intDistance);


                if(distanceToPeople < safeDistance && (QRandomGenerator::global()->bounded(1,101) <= infectiousChanse)){
                    if(QRandomGenerator::global()->bounded(1,101) > invisInfectiousChanse){
                        status = "Infectious";
                    }
                    else{
                        status = "InvisibleInfectious";
                    }
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
            }

            //Если карантин нужен
            if(quarNeeds){
                if (status == "Recovered" && areal == quarAreal) {
                    QTimer* timer = new QTimer(this);
                    connect(timer, &QTimer::timeout, this, [=]() {
                        if (!recoveredFromQuar) {
                            setPos(QRandomGenerator::global()->bounded(30, 540),
                                   QRandomGenerator::global()->bounded(30, 450));
                            exitQuar(safeAreal);
                            safeAreal->getScene()->addItem(this);
                        }
                        timer->deleteLater();
                    });
                    timer->start(3000);
                }

                if (status == "Infectious" && areal == safeAreal) {
                    QTimer* timer = new QTimer(this);
                    connect(timer, &QTimer::timeout, this, [=]() {
                        if (!wasInQuar) {
                            setPos(QRandomGenerator::global()->bounded(640, 900),
                                   QRandomGenerator::global()->bounded(270, 500));
                            setQuar(quarAreal);
                            quarAreal->getScene()->addItem(this);
                        }
                        timer->deleteLater();
                    });
                    timer->start(3000);
                }
            }

            if ((status == "Infectious" || status == "InvisibleInfectious")) {
                QTimer* timer = new QTimer(this);
                connect(timer, &QTimer::timeout, this, [=]() {
                    if (!alreadyChanged) {
                        alreadyChanged = true;
                        if (QRandomGenerator::global()->bounded(1, 101) >= deadChanse) {
                            status = "Recovered";
                        } else {
                            status = "Dead";
                        }
                    }
                    timer->deleteLater();
                });
                timer->start(7000);
            }

            /*
            if ((status == "Infectious" || status == "InvisibleInfectious")) {

                QTimer::singleShot(5000, [this]() { // Добавлено захватывание по ссылке флага alreadyChanged
                    if (!alreadyChanged) { // Проверка флага перед изменением статуса
                        alreadyChanged = true; // Устанавливаем флаг уже измененного статуса

                        if (QRandomGenerator::global()->bounded(1, 101) >= 5) {
                            status = "Recovered";
                        } else {
                            status = "Dead";
                        }
                    }
                });
            }

            if (status == "Recovered" && areal==quarAreal) {
                QTimer::singleShot(2000, [this]() {
                    if(!recoveredFromQuar){
                        setPos(QRandomGenerator::global()->bounded(30, 540),
                               QRandomGenerator::global()->bounded(30,450));
                    }
                    exitQuar(safeAreal);
                    safeAreal->getScene()->addItem(this);
                });
            }

            if (status == "Infectious" && areal==safeAreal) {
                QTimer::singleShot(2000, [this]() {
                    if(!wasInQuar){
                        setPos(QRandomGenerator::global()->bounded(640,780),
                               QRandomGenerator::global()->bounded(440,530));
                    }
                    setQuar(quarAreal);
                    quarAreal->getScene()->addItem(this);
                });
            }
            */
    } 
}

People::~People(){
}



