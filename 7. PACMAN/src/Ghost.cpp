#include "Ghost.h"
#include "Config.h"
#include <QGraphicsScene>
#include <algorithm>
#include <cmath>

static QColor colorForType(GhostType ghostType) {
    switch (ghostType) {
        case GhostType::Blinky:
            return Qt::red;
        case GhostType::Pinky:
            return QColor(255, 184, 255);
        case GhostType::Inky:
            return QColor(0, 255, 255);
        case GhostType::Clyde:
            return QColor(255, 184, 82);
        }
    return Qt::red;
}

Ghost::Ghost(GhostType t)
    : GameItem(colorForType(t), config::GhostSpawnOffscreenX, config::GhostSpawnOffscreenY), type(t) {
    setRect(config::GhostRectInset, config::GhostRectInset, config::GhostRectSize, config::GhostRectSize);
    setZValue(1);
}

void Ghost::setTarget(QPointF targetPosition) { target = targetPosition; }

void Ghost::setScared(bool isScared) {
    scared = isScared;
    if (scared) {
        setBrush(QBrush(Qt::white));
    }
    else {
        setBrush(QBrush(colorForType(type)));
    }
}

bool Ghost::canMoveTo(qreal shiftX, qreal shiftY) {
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

bool Ghost::isAtIntersection() {
    return (static_cast<int>(x()) % config::TileSize == 0) && (static_cast<int>(y()) % config::TileSize == 0);
}

std::vector<Direction> Ghost::getPossibleMoves() {
    int ghostSpeed = config::GhostSpeed;
    std::vector<Direction> moves = {
        {ghostSpeed, 0, 0.0},
        {-ghostSpeed, 0, 0.0},
        {0, ghostSpeed, 0.0},
        {0, -ghostSpeed, 0.0}
    };

    for (auto& move : moves) {
        double potentialX = x() + move.dx;
        double potentialY = y() + move.dy;
        move.distance = std::sqrt(std::pow(potentialX - target.x(), 2) + std::pow(potentialY - target.y(), 2));
    }

    std::sort(moves.begin(), moves.end(),
              [](const Direction& a, const Direction& b) { return a.distance < b.distance; });

    return moves;
}

bool Ghost::isReverseOfCurrent(const Direction& direction) const {
    const bool isMoving = (dx != 0) || (dy != 0);
    return isMoving && direction.dx == -dx && direction.dy == -dy;
}

bool Ghost::trySetFirstValidDirection(const std::vector<Direction>& directions, bool allowReverse) {
    for (const auto& direction : directions) {
        if (!allowReverse && isReverseOfCurrent(direction)) continue;
        if (!canMoveTo(direction.dx, direction.dy)) continue;

        dx = direction.dx;
        dy = direction.dy;
        return true;
    }
    return false;
}

void Ghost::chooseBestDirection() {
    const auto possibleDirections = getPossibleMoves();

    if (trySetFirstValidDirection(possibleDirections, false)) {
        return;
    }
    trySetFirstValidDirection(possibleDirections, true);
}
void Ghost::performMove() {
    if (canMoveTo(dx, dy)) {
        setPos(x() + dx, y() + dy);
    }
}

static QPointF clampToSceneBounds(const QPointF& p, const QSizeF& sceneSize) {
    double clampedX = std::clamp(p.x(), 0.0, sceneSize.width() - static_cast<double>(config::TileSize));
    double clampedY = std::clamp(p.y(), 0.0, sceneSize.height() - static_cast<double>(config::TileSize));
    return {clampedX, clampedY};
}

QPointF Ghost::computeChaseTarget(const QPointF& playerPos, const QPointF& playerDir, const QPointF& blinkyPos,
                                  const QSizeF& sceneSize) const {
    const double tileSize = config::TileSize;

    switch (type) {
        case GhostType::Blinky:
            return clampToSceneBounds(playerPos, sceneSize);

        case GhostType::Pinky: {
            double predictedDistance =  (static_cast<double>(config::PinkyLookAheadTiles) * tileSize);
            QPointF ahead = playerPos + playerDir * predictedDistance;
            return clampToSceneBounds(ahead, sceneSize);
        }

        case GhostType::Inky: {
            double predictedDistance = (static_cast<double>(config::InkyLookAheadTiles) * tileSize);
            QPointF ahead = playerPos + playerDir * predictedDistance ;
            QPointF vectorFromBlinkyToPredictedPlayerPosition = ahead - blinkyPos;
            QPointF inkyTargetPosition = blinkyPos + ( vectorFromBlinkyToPredictedPlayerPosition * 2.0 );
            return clampToSceneBounds(inkyTargetPosition, sceneSize);
        }

        case GhostType::Clyde: {
            double distanceToPlayer = std::hypot(x() - playerPos.x(), y() - playerPos.y());
            double clydeChaseDistance = static_cast<double>(config::ClydeChaseDistanceTiles) * tileSize;

            if (distanceToPlayer > clydeChaseDistance) {
                return clampToSceneBounds(playerPos, sceneSize);
            }
            QPointF corner(0.0, sceneSize.height() - tileSize);
            return clampToSceneBounds(corner, sceneSize);
        }
    }

    return clampToSceneBounds(playerPos, sceneSize);
}

QPointF Ghost::computeScaredTarget(const QSizeF& sceneSize) const {
    const double tileSize = config::TileSize;

    QPointF scatterCorner;
    switch (type) {
        case GhostType::Blinky:
            scatterCorner = QPointF(sceneSize.width() - tileSize, 0.0);
            break;
        case GhostType::Pinky:
            scatterCorner = QPointF(0.0, 0.0);
            break;
        case GhostType::Inky:
            scatterCorner = QPointF(sceneSize.width() - tileSize, sceneSize.height() - tileSize);
            break;
        case GhostType::Clyde:
            scatterCorner = QPointF(0.0, sceneSize.height() - tileSize);
            break;
    }

    return clampToSceneBounds(scatterCorner, sceneSize);
}

void Ghost::updateAI(const QPointF& playerPos, const QPointF& playerDir, const QPointF& blinkyPos,
                     const QSizeF& sceneSize) {
    if (scared) {
        target = computeScaredTarget(sceneSize);
    }
    else {
        target = computeChaseTarget(playerPos, playerDir, blinkyPos, sceneSize);
    }
}

void Ghost::moveItem() {
    if (isAtIntersection() || !canMoveTo(dx, dy) || (dx == 0 && dy == 0)) {
        chooseBestDirection();
    }
    performMove();
}
