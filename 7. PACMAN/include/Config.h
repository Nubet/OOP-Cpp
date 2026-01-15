#ifndef CONFIG_H
#define CONFIG_H

#include <QColor>

namespace config
{
    inline constexpr int TileSize = 40;
    inline constexpr int MapCols = 28;
    inline constexpr int MapRows = 31;

    inline constexpr int SceneWidth = MapCols * TileSize;
    inline constexpr int SceneHeight = MapRows * TileSize;
    inline constexpr double ViewScale = 0.5;

    inline constexpr int ViewPadding = 5;
    inline constexpr int ViewWidth = SceneWidth + ViewPadding;
    inline constexpr int ViewHeight = SceneHeight + ViewPadding;

    inline constexpr int ScoreFontSize = 20;
    inline constexpr int ScorePosX = 500;
    inline constexpr int ScorePosY = 560;

    inline constexpr int PlayerRectInset = 2;
    inline constexpr int PlayerRectSize = 36;
    inline constexpr int PlayerSpeed = 8;

    inline constexpr int GhostRectInset = 2;
    inline constexpr int GhostRectSize = 36;
    inline constexpr int GhostSpeed = 5;
    inline constexpr int GhostSpawnOffscreenX = -50;
    inline constexpr int GhostSpawnOffscreenY = -50;

    inline constexpr int PointInset = 16;
    inline constexpr int PointSize = 8;

    inline constexpr int PowerPelletInset = 10;
    inline constexpr int PowerPelletSize = 20;

    inline constexpr int TickMs = 30;
    inline constexpr int PowerModeMs = 6000;

    inline constexpr int ScorePoint = 1;
    inline constexpr int ScorePowerPellet = 50;
    inline constexpr int ScoreGhostEaten = 200;

    inline constexpr int PinkyLookAheadTiles = 4;
    inline constexpr int InkyLookAheadTiles = 2;
    inline constexpr int ClydeChaseDistanceTiles = 6;
} // namespace config

#endif
