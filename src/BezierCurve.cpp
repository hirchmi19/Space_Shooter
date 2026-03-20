#include "constants/GameConstants.h"
#include "systems/waving/BezierCurves.h"


void CalcControlPointsTop(Enemy& enemy, const Vector2& start, const Vector2& end) {

    enemy.wave.controlPoints[0] = start;

    enemy.wave.controlPoints[1] = {start.x * 0.8f + end.x * 0.2f,start.y + 400.0f };
    enemy.wave.controlPoints[2] = { end.x,start.y + 600.0f };

    enemy.wave.controlPoints[3] = end;

    enemy.wave.t = 0.0f;
}



void CalcControlPointsSide(Enemy &enemy, const Vector2 &start, const Vector2 &end) {

    enemy.wave.controlPoints[0] = start;

    const bool fromLeft = start.x < GameConstants::SCREEN_WIDTH / 2;

    constexpr float horizontalPull = 200.0f;
    constexpr float verticalPull  = 800.0f;

    enemy.wave.controlPoints[1] = {start.x + (fromLeft ? horizontalPull : -horizontalPull),start.y + verticalPull};
    enemy.wave.controlPoints[2].x = end.x + (fromLeft ? -40.0f : 40.0f);
    enemy.wave.controlPoints[3] = end;

    enemy.wave.t = 0.0f;
}

void CalcControlPointsSideRandom(Enemy &enemy, const Vector2 &start, const Vector2 &end) {

    enemy.wave.controlPoints[0] = start;

    const bool fromLeft = start.x < GameConstants::SCREEN_WIDTH / 2;

    const int horizontalPull = GetRandomValue(200, GameConstants::SCREEN_WIDTH - 200);
    const int verticalPull  = GetRandomValue(200, GameConstants::SCREEN_HEIGHT - 200);

    enemy.wave.controlPoints[1] = {start.x + (fromLeft ? horizontalPull : -horizontalPull), start.y};
    enemy.wave.controlPoints[2] = {enemy.wave.controlPoints[1].x, enemy.wave.controlPoints[1].y + verticalPull};
    enemy.wave.controlPoints[3] = end;

    enemy.wave.t = 0.0f;
}


void CalcControlPointsSolo(Enemy &enemy, const Vector2 &start, const Vector2 &end) {

    enemy.wave.controlPoints[0] = start;

    const bool fromLeft = start.x < GameConstants::SCREEN_WIDTH / 2;
    constexpr float horizontalPull = 450.0f;

    enemy.wave.controlPoints[1] = {start.x + (fromLeft ? horizontalPull : -horizontalPull),start.y + 300.0f};
    enemy.wave.controlPoints[2] = {end.x + (fromLeft ? -60.0f : 60.0f),end.y - 150.0f};
    enemy.wave.controlPoints[3] = {end.x, end.y + 100.0f};

    enemy.wave.t = 0.0f;
}

void CalcControlPointsLoop(Enemy& enemy, const Vector2& start, const Vector2& end) {

    enemy.wave.controlPoints[0] = start;

    const bool fromLeft = start.x < GameConstants::SCREEN_WIDTH / 2;
    constexpr float loopWidth  = 350.0f;
    constexpr float loopHeight = 250.0f;

    enemy.wave.controlPoints[1] = {
        start.x + (fromLeft ? loopWidth : -loopWidth),
        start.y - loopHeight
    };

    enemy.wave.controlPoints[2] = {
        start.x - (fromLeft ? loopWidth * 0.5f : -loopWidth * 0.5f),
        start.y - loopHeight * 0.5f
    };

    enemy.wave.controlPoints[3] = end;
    enemy.wave.t = 0.0f;
}

void CalcControlPointsLoopRandom(Enemy& enemy, const Vector2& start, const Vector2& end) {

    enemy.wave.controlPoints[0] = start;

    const bool fromLeft = start.x < GameConstants::SCREEN_WIDTH / 2;
    const float loopWidth  = static_cast<float>(GetRandomValue(250, 450));
    const float loopHeight = static_cast<float>(GetRandomValue(200, 350));

    enemy.wave.controlPoints[1] = {
        start.x + (fromLeft ? loopWidth : -loopWidth),
        start.y - loopHeight
    };

    enemy.wave.controlPoints[2] = {
        start.x - (fromLeft ? loopWidth * 0.5f : -loopWidth * 0.5f),
        start.y - loopHeight * 0.5f
    };

    enemy.wave.controlPoints[3] = end;
    enemy.wave.t = 0.0f;
}