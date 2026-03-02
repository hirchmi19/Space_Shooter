//
// Created by Michael Hirsch on 23.02.26.
//

#pragma once
#include <array>
#include <vector>
#include <sys/types.h>

#include "raylib.h"
#include "../../game/IGameSystem.h"
#include "../../entities/enemies/EnemyType.h"
#include "../../locators/IScoreLocator.h"
#include "../../utils/utils.h"

struct ScoreUI;

class ScoreSystem: public IScoreLocator, public IGameSystem {

    public:

    ScoreSystem();
    ~ScoreSystem() override = default;

    void Run() override;
    void Init() override;

    int GetHighScore() const override {return highScore;}
    float GetMult() const override {return mult;}
    void AddHighScore(const int &score, const Vector2 &pos) override;
    int GetEnemyScore(const EnemyType& id) override;
    void ResetScore() override {highScore = 0; mult = 1.0f; timerStarted = false;}
    void ResetMult() override {mult = 1.0f; timerStarted = false;}
    void CreateScore(const float &score, const Vector2& pos) override;
    std::vector<ScoreUI>& GetScoreUI() override {return scoreUI;}


    private:

    int highScore = 0;
    float mult = 1.0f;
    size_t multTimer{};
    bool timerStarted = false;
    std::array<uint, ToIndex(EnemyType::COUNT)> enemyScores{};
    std::vector<ScoreUI> scoreUI{};

};


