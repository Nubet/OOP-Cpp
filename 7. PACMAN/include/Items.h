#ifndef ITEMS_H
#define ITEMS_H

#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QObject>

class GameItem : public QGraphicsRectItem {
public:
    GameItem(QColor itemColor, int initialX, int initialY);
    virtual void moveItem() = 0;
};

class Wall : public QGraphicsRectItem {
public:
    Wall(int x, int y);
};

class Point : public QGraphicsEllipseItem {
public:
    Point(int x, int y);
};

class PowerPellet : public QGraphicsEllipseItem {
public:
    PowerPellet(int x, int y);
};

class Player : public GameItem {
    int dx = 0, dy = 0;
    int futureHorizontalVelocity = 0, futureVerticalVelocity = 0;

    bool canMoveTo(qreal newX, qreal newY);
    void tryToChangeDirection();
    void moveForward();

public:
    Player();
    void setDirection(int keyPress);
    void moveItem() override;
};

#endif