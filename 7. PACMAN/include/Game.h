#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QPoint>
#include <QPointF>
#include <optional>
#include <string>
#include <vector>

class QGraphicsScene;
class QGraphicsTextItem;
class QKeyEvent;
class QTimer;

class Player;
class Ghost;

class Game final : public QGraphicsView {
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();
    void deactivatePowerMode();

private:
    enum class TileType {
        Wall,
        Floor,
        EmptyFloor,
        PlayerStart,
        BlinkyStart,
        InkyStart,
        PinkyStart,
        ClydeStart,
        Fruit,
        Teleport
    };

    struct EntityStartPositions {
        QPoint player{-1, -1};
        QPoint blinky{-1, -1};
        QPoint pinky{-1, -1};
        QPoint inky{-1, -1};
        QPoint clyde{-1, -1};
    };


    void initScene();
    void initEntities();
    void initScoreDisplay();
    void initTimers();

    std::vector<std::string> loadMapLayout() const;
    static TileType mapCharacterToTileType(char c);
    EntityStartPositions placeMapTiles(const std::vector<std::string> &map);
    void setEnemiesStartPositions(const EntityStartPositions &starts);
    void drawMap();

    void handleTeleports();
    void tryTeleport(QGraphicsItem *item);

    QPointF calculatePlayerDirection();
    QPointF playerDirectionVector() const;
    void updateGhostBehavior(const QPointF &player_dir);

    void checkCollisions();
    void handleGhostCollision(Ghost *g);
    void handleItemCollisions();
    void checkWinCondition();
    void updateScoreDisplay();

    QGraphicsScene *scene = nullptr;
    Player *player = nullptr;
    std::vector<Ghost *> ghosts;

    QTimer *gameUpdateTimer = nullptr;
    QTimer *powerTimer = nullptr;
    QGraphicsTextItem *scoreDisplay = nullptr;

    int score = 0;
    int numOfPoints = 0;
    bool powerMode = false;

    std::optional<QPoint> teleportEntry;
    std::optional<QPoint> teleportExit;
};

#endif
