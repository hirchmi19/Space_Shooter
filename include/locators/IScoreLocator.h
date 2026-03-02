//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once

enum class EnemyType;

struct IScoreLocator {

    virtual ~IScoreLocator() = default;
    virtual int GetEnemyScore(const EnemyType& id) = 0;
    virtual void AddHighScore(int score) = 0;
    virtual void ResetScore() = 0;
    virtual int GetHighScore() const = 0;

};

