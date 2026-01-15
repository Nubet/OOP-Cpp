#pragma once

#include "Items.h"
#include <vector>

struct Direction
{
    int dx, dy;
    double distanceToTarget;
};

enum class GhostType
{
    Blinky,
    Pinky,
    Inky,
    Clyde
};

class Ghost : public GameItem
{
    GhostType type;
    bool scared = false;

    QPointF target;
    int dx = 0, dy = 0;

    bool canMoveTo(qreal shiftX, qreal shiftY);
    bool isAlignedToGrid();
    std::vector<Direction> getPossibleMoves();

    bool isReverseOfCurrent(const Direction& direction) const;
    bool trySetFirstValidDirection(const std::vector<Direction>& directions, bool allowReverse);

    void chooseBestDirection();
    void performMove();

    QPointF computeChaseTarget(const QPointF& playerPos, const QPointF& playerDir, const QPointF& blinkyPos,
                               const QSizeF& sceneSize) const;
    QPointF computeScaredTarget(const QSizeF& sceneSize) const;

  public:
    explicit Ghost(GhostType t = GhostType::Blinky);

    GhostType ghostType() const { return type; }

    void setTarget(QPointF targetPosition);
    void setScared(bool isScared);

    void updateAI(const QPointF& playerPos, const QPointF& playerDir, const QPointF& blinkyPos,
                  const QSizeF& sceneSize);

    void moveItem() override;
};
