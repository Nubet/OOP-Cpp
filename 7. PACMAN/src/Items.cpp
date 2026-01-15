#include "Items.h"
#include "Config.h"
#include <QGraphicsScene>
#include <QPen>

GameItem::GameItem(QColor itemColor, int initialX, int initialY) {
    setRect(0, 0, config::TileSize, config::TileSize);
    setBrush(QBrush(itemColor));
    setPen(Qt::NoPen);
    setPos(initialX, initialY);
}

Wall::Wall(int initialX, int initialY) {
    setRect(0, 0, config::TileSize, config::TileSize);
    setBrush(QBrush(Qt::blue));
    setPen(QPen(Qt::darkBlue));
    setPos(initialX, initialY);
}

Point::Point(int initialX, int initialY) {
    setRect(config::PointInset, config::PointInset, config::PointSize, config::PointSize);
    setBrush(QBrush(Qt::white));
    setPen(Qt::NoPen);
    setPos(initialX, initialY);
}

PowerPellet::PowerPellet(int initialX, int initialY) {
    setRect(config::PowerPelletInset, config::PowerPelletInset, config::PowerPelletSize, config::PowerPelletSize);
    setBrush(QBrush(Qt::green));
    setPen(Qt::NoPen);
    setPos(initialX, initialY);
}

Player::Player() : GameItem(Qt::yellow, -100, -100) {
    setRect(config::PlayerRectInset, config::PlayerRectInset, config::PlayerRectSize, config::PlayerRectSize);
    setZValue(1);
}

void Player::setDirection(int keyPress) {
    if (keyPress == Qt::Key_Left) {
        futureHorizontalVelocity = -config::PlayerSpeed;
        futureVerticalVelocity = 0;
    }
    if (keyPress == Qt::Key_Right) {
        futureHorizontalVelocity = config::PlayerSpeed;
        futureVerticalVelocity = 0;
    }
    if (keyPress == Qt::Key_Up) {
        futureHorizontalVelocity = 0;
        futureVerticalVelocity = -config::PlayerSpeed;
    }
    if (keyPress == Qt::Key_Down) {
        futureHorizontalVelocity = 0;
        futureVerticalVelocity = config::PlayerSpeed;
    }
}

bool Player::canMoveTo(qreal shiftX, qreal shiftY) {
    QRectF potentialPositionRect = rect();
    potentialPositionRect.translate(shiftX, shiftY);
    QList<QGraphicsItem*> items =
        scene()->items(potentialPositionRect.translated(pos()), Qt::IntersectsItemBoundingRect);

    for (auto* item : items) {
        if (dynamic_cast<Wall*>(item) && item != this) {
            return false;
        }
    }
    return true;
}

void Player::tryToChangeDirection() {
    if (futureHorizontalVelocity == 0 && futureVerticalVelocity == 0) return;

    if (futureHorizontalVelocity == -dx && futureVerticalVelocity == -dy) {
        dx = futureHorizontalVelocity;
        dy = futureVerticalVelocity;
        return;
    }

    int currentTileX = static_cast<int>(x());
    int currentTileY = static_cast<int>(y());

    if (currentTileX % config::TileSize == 0 && currentTileY % config::TileSize == 0) {
        if (canMoveTo(futureHorizontalVelocity, futureVerticalVelocity)) {
            dx = futureHorizontalVelocity;
            dy = futureVerticalVelocity;
        }
    }
}

void Player::moveForward() {
    if (canMoveTo(dx, dy)) {
        setPos(x() + dx, y() + dy);
    }
}

void Player::moveItem() {
    tryToChangeDirection();
    moveForward();
}
