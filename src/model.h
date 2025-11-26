#pragma once

#include "entities/paddle.h"
#include "entities/ball.h"
#include "bricks_manager.h"
#include "bricks_model.h"
#include "config/game_config.h"
#include "score_manager.h"
#include <memory>

class Model
{
public:
    explicit Model(const GameConfig& gameConfig);
    void reset();

    [[nodiscard]] Paddle& getPaddle() const { return *paddle_; }

    [[nodiscard]] Ball& getBall() const { return *ball_; }

    [[nodiscard]] BricksManager& getBricksManager() { return bricksManager_; }

    [[nodiscard]] BricksModel& getBricksModel() { return bricksModel_; }

    [[nodiscard]] ScoreManager& getScoreManager() const { return *scoreManager_; }

private:
    void postInitialize();

    const GameConfig& gameConfig_;

    std::unique_ptr<Paddle> paddle_;
    std::unique_ptr<Ball> ball_;

    BricksManager bricksManager_;
    BricksModel bricksModel_;

    std::unique_ptr<ScoreManager> scoreManager_;
};
