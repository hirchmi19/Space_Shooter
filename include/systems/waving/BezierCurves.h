#pragma once
#include <raylib.h>
#include "entities/enemies/Enemy.h"

void CalcControlPointsTop(Enemy& enemy, const Vector2& start, const Vector2& end);
void CalcControlPointsSide(Enemy& enemy, const Vector2& start, const Vector2& end);
void CalcControlPointsSolo(Enemy& enemy, const Vector2& start, const Vector2& end);
void CalcControlPointsSideRandom(Enemy& enemy, const Vector2& start, const Vector2& end);
void CalcControlPointsLoop(Enemy& enemy, const Vector2& start, const Vector2& end);
void CalcControlPointsLoopRandom(Enemy &enemy, const Vector2 &start, const Vector2 &end);


