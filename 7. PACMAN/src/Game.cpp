#include "Game.h"
#include "Config.h"
#include "Ghost.h"
#include "Items.h"

#include <QApplication>
#include <QFont>
#include <QKeyEvent>
#include <QMessageBox>
#include <cmath>
#include <QTimer>

static qreal signOf(qreal value) { return (value > 0) ? 1 : (value < 0 ? -1 : 0); }

static QPointF convertPixelMovementToAxisDirection(QPointF directionVector) {
    qreal absX = std::abs(directionVector.x());
    qreal absY = std::abs(directionVector.y());

    if (absX == 0 && absY == 0) return {0, 0};

    if (absX > absY) {
        return {signOf(directionVector.x()), 0};
    }
    else {
        return {0, signOf(directionVector.y())};
    }
}


static QPoint convertScenePositionToTileIndex(const QPointF& scenePosition) {
    return {static_cast<int>(std::round(scenePosition.x() / config::TileSize)),
            static_cast<int>(std::round(scenePosition.y() / config::TileSize))};
}

static QPointF convertTileIndexToScenePosition(const QPoint& tileIndex) {
    return {static_cast<qreal>(tileIndex.x() * config::TileSize), static_cast<qreal>(tileIndex.y() * config::TileSize)};
}

Game::Game(QWidget* parent)
    : QGraphicsView(parent) {
    initScene();
    initEntities();
    drawMap();
    initScoreDisplay();
    initTimers();
}

void Game::initScene() {
    scene = new QGraphicsScene(0, 0, config::SceneWidth, config::SceneHeight);
    scene->setBackgroundBrush(Qt::black);
    setScene(scene);
    setFixedSize(config::ViewWidth, config::ViewHeight);
}

void Game::initEntities() {
    player = new Player();

    ghosts.clear();
    ghosts = {
        new Ghost(GhostType::Blinky),
        new Ghost(GhostType::Pinky),
        new Ghost(GhostType::Inky),
        new Ghost(GhostType::Clyde)
    };

    scene->addItem(player);
    for (auto* g : ghosts) scene->addItem(g);
}

void Game::initScoreDisplay() {
    scoreDisplay = new QGraphicsTextItem();
    scoreDisplay->setDefaultTextColor(Qt::white);
    scoreDisplay->setFont(QFont("Arial", config::ScoreFontSize, QFont::Bold));
    scoreDisplay->setPos(config::ScorePosX, config::ScorePosY);
    scoreDisplay->setZValue(10);
    scene->addItem(scoreDisplay);
    updateScoreDisplay();
}

void Game::initTimers() {
    gameUpdateTimer = new QTimer(this);
    connect(gameUpdateTimer, &QTimer::timeout, this, &Game::updateGame);
    gameUpdateTimer->start(config::TickMs);

    powerTimer = new QTimer(this);
    powerTimer->setSingleShot(true);
    connect(powerTimer, &QTimer::timeout, this, &Game::deactivatePowerMode);
}

Game::TileType Game::mapCharacterToTileType(char c) {
    switch (c) {
    case '#':
        return TileType::Wall;
    case 'P':
        return TileType::PlayerStart;
    case 'B':
        return TileType::BlinkyStart;
    case 'I':
        return TileType::InkyStart;
    case 'K':
        return TileType::PinkyStart;
    case 'C':
        return TileType::ClydeStart;
    case 'F':
        return TileType::Fruit;
    case 'E':
        return TileType::EmptyFloor;
    case 'T':
        return TileType::Teleport;
    case ' ':
        return TileType::Floor;
    default:
        return TileType::Floor;
    }
}

std::vector<std::string> Game::loadMapLayout() const
{
    return {
        "############################",
        "#            ##            #",
        "# #### ##### ## ##### #### #",
        "#F#EE# #EEE# ## #EEE# #EE#F#",
        "# #### ##### ## ##### #### #",
        "#                          #",
        "# #### ## ######## ## #### #",
        "# #### ## ######## ## #### #",
        "#      ##    ##    ##      #",
        "###### ##### ## ##### ######",
        "EEEEE# ##### ## ##### #EEEEE",
        "EEEEE# ##          ## #EEEEE",
        "EEEEE# ##  B    I  ## #EEEEE",
        "###### ## ######## ## ######",
        "T         #EEEEEE#         T",
        "###### ## ######## ## ######",
        "EEEEE# ##  K    C  ## #EEEEE",
        "EEEEE# ##          ## #EEEEE",
        "EEEEE# ## ######## ## #EEEEE",
        "###### ## ######## ## ######",
        "#            ##            #",
        "# #### ##### ## ##### #### #",
        "# #### ##### ## ##### #### #",
        "#F  ##       P        ##  F#",
        "### ## ## ######## ## ## ###",
        "### ## ## ######## ## ## ###",
        "#      ##    ##    ##      #",
        "# ########## ## ########## #",
        "# ########## ## ########## #",
        "#                          #",
        "############################"
    };
}

Game::EntityStartPositions Game::placeMapTiles(const std::vector<std::string>& map) {
    EntityStartPositions StartPositions;
    numOfPoints = 0;
    teleportEntry.reset();
    teleportExit.reset();

    const int rows = static_cast<int>(map.size());
    const int cols = static_cast<int>(map[0].size());
    scene->setSceneRect(0, 0, cols * config::TileSize, rows * config::TileSize);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            const int px = x * config::TileSize;
            const int py = y * config::TileSize;

            switch (mapCharacterToTileType(map[y][x])) {
                case TileType::Wall:
                    scene->addItem(new Wall(px, py));
                    break;

                case TileType::Floor:
                    scene->addItem(new Point(px, py));
                    ++numOfPoints;
                    break;

                case TileType::EmptyFloor:
                    break;

                case TileType::Fruit:
                    scene->addItem(new PowerPellet(px, py));
                    break;

                case TileType::Teleport:
                    if (!teleportEntry)
                        teleportEntry = QPoint(px, py);
                    else if (!teleportExit)
                        teleportExit = QPoint(px, py);
                    break;

                case TileType::PlayerStart:
                    StartPositions.player = QPoint(px, py);
                    break;

                case TileType::BlinkyStart:
                    StartPositions.blinky = QPoint(px, py);
                    break;

                case TileType::PinkyStart:
                    StartPositions.pinky = QPoint(px, py);
                    break;

                case TileType::InkyStart:
                    StartPositions.inky = QPoint(px, py);
                    break;

                case TileType::ClydeStart:
                    StartPositions.clyde = QPoint(px, py);
                    break;
            }
        }
    }

    return StartPositions;
}

void Game::setEnemiesStartPositions(const EntityStartPositions& starts) {
    if (starts.player.x() >= 0) player->setPos(starts.player);

    if (ghosts.size() >= 4) {
        if (starts.blinky.x() >= 0) ghosts[0]->setPos(starts.blinky);
        if (starts.pinky.x() >= 0) ghosts[1]->setPos(starts.pinky);
        if (starts.inky.x() >= 0) ghosts[2]->setPos(starts.inky);
        if (starts.clyde.x() >= 0) ghosts[3]->setPos(starts.clyde);
    }
}

void Game::drawMap() { setEnemiesStartPositions(placeMapTiles(loadMapLayout())); }

void Game::deactivatePowerMode() {
    powerMode = false;
    for (auto* g : ghosts) g->setScared(false);
}

void Game::teleportIfOnTeleportTile(QGraphicsItem* item) {
    if (!item || !teleportEntry || !teleportExit) return;

    const int teleportCooldownTicks = 6;
    static constexpr int teleportCooldownKey = 0;
    static constexpr int lastPosKey = 1;

    const int remainingTeleportCooldown = item->data(teleportCooldownKey).toInt();
    if (remainingTeleportCooldown > 0) {
        item->setData(teleportCooldownKey, remainingTeleportCooldown - 1);
        return;
    }

    const QPoint itemTile = convertScenePositionToTileIndex(item->pos());
    const QPoint entryTile = convertScenePositionToTileIndex(*teleportEntry);
    const QPoint exitTile  = convertScenePositionToTileIndex(*teleportExit);

    const bool isOnEntryTile = (itemTile == entryTile);
    const bool isOnExitTile  = (itemTile == exitTile);
    if (!isOnEntryTile && !isOnExitTile) return;

    const QPoint destinationTile = isOnEntryTile ? exitTile : entryTile;
    const QPointF destinationPos = convertTileIndexToScenePosition(destinationTile);

    item->setPos(destinationPos);
    item->setData(teleportCooldownKey, teleportCooldownTicks);
    item->setData(lastPosKey, item->pos());
}

void Game::handleTeleports() {
    teleportIfOnTeleportTile(player);
    for (auto* g : ghosts) teleportIfOnTeleportTile(g);
}

QPointF Game::calculatePlayerDirection() {
    static QPointF previousPlayerPosition = player->pos();
    const QPointF currentPlayerPosition = player->pos();
    const QPointF d = convertPixelMovementToAxisDirection(currentPlayerPosition - previousPlayerPosition);
    previousPlayerPosition = currentPlayerPosition;
    return d;
}

void Game::updateGhostBehavior(const QPointF& playerDir) {
    const QPointF blinkyPos = ghosts.empty() ? QPointF(0, 0) : ghosts[0]->pos();
    const QSizeF sceneSize(scene->sceneRect().width(), scene->sceneRect().height());

    for (auto* g : ghosts) {
        g->updateAI(player->pos(), playerDir, blinkyPos, sceneSize);
        g->moveItem();
    }

    handleTeleports();
}

void Game::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape)
        QApplication::quit();
    else
        player->setDirection(event->key());
}

QPointF Game::playerDirectionVector() const { return {}; }

void Game::updateGame() {
    static constexpr int lastPositionKey = 1;
    player->setData(lastPositionKey, player->pos());
    for (auto* g : ghosts) g->setData(lastPositionKey, g->pos());

    player->moveItem();
    handleTeleports();

    const QPointF playerDir = calculatePlayerDirection();
    updateGhostBehavior(playerDir);

    checkCollisions();
}


void Game::updateScoreDisplay() { scoreDisplay->setPlainText("Score: " + QString::number(score)); }

void Game::handleGhostCollision(Ghost* g) {
    if (!g) return;

    if (!powerMode) {
        gameUpdateTimer->stop();
        QMessageBox::information(this, "Game Over", "You lost :(");
        QApplication::quit();
        return;
    }

    score += config::ScoreGhostEaten;
    updateScoreDisplay();

    const int min = config::TileSize;
    const int maxTileX = static_cast<int>(scene->sceneRect().width()) - config::TileSize * 2;
    const int maxTileY = static_cast<int>(scene->sceneRect().height()) - config::TileSize * 2;

    switch (g->ghostType()) {
        case GhostType::Blinky:
            g->setPos(min, min);
            break;
        case GhostType::Pinky:
            g->setPos(maxTileX, min);
            break;
        case GhostType::Inky:
            g->setPos(maxTileX, maxTileY);
            break;
        case GhostType::Clyde:
            g->setPos(min, maxTileY);
            break;
        }
}

void Game::handleItemCollisions() {
    const QList<QGraphicsItem*> collidingItems = scene->collidingItems(player);

    for (auto* item : collidingItems) {
        if (auto* collectedPoint = dynamic_cast<Point*>(item)) {
            scene->removeItem(collectedPoint);
            delete collectedPoint;

            score += config::ScorePoint;
            updateScoreDisplay();

            if (--numOfPoints <= 0) {
                gameUpdateTimer->stop();
                QMessageBox::information(this, "Congratulations!", "You won!");
                QApplication::quit();
                return;
            }
            continue;
        }

        if (auto* collectedPowerPellet = dynamic_cast<PowerPellet*>(item)) {
            scene->removeItem(collectedPowerPellet);
            delete collectedPowerPellet;

            score += config::ScorePowerPellet;
            updateScoreDisplay();

            powerMode = true;
            for (auto* g : ghosts) g->setScared(true);
            powerTimer->start(config::PowerModeMs);
        }
    }
}

void Game::checkCollisions() {
    for (auto* g : ghosts) {
        if (player->collidesWithItem(g)) {
            handleGhostCollision(g);
            if (!powerMode) return;
        }
    }

    handleItemCollisions();
}
