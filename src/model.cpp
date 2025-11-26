#include "model.h"
#include "entities/paddle.h"
#include "entities/ball.h"
#include "brick_layouts/brick_layout_library.h"
#include "config/game_config.h"
#include <QDebug>

Model::Model(const GameConfig& gameConfig) : gameConfig_(gameConfig)
{
    // qDebug() << "Model::Model()";

    const auto& [WIDTH, HEIGHT, INITIAL_POSITION_X, INITIAL_POSITION_Y] = gameConfig.getPaddleConfig();
    paddle_ = std::make_unique<Paddle>(INITIAL_POSITION_X,
                                       INITIAL_POSITION_Y,
                                       WIDTH,
                                       HEIGHT);

    const auto& ballConfig = gameConfig.getBallConfig();
    ball_ = std::make_unique<Ball>(ballConfig.INITIAL_POSITION_X,
                                   ballConfig.INITIAL_POSITION_Y,
                                   ballConfig.DIAMETER,
                                   ballConfig.INITIAL_VELOCITY_X,
                                   ballConfig.INITIAL_VELOCITY_Y);


    bricksModel_.setBricksManager(&bricksManager_);

    bricksManager_.createBricksFromLayout(BrickLayoutLibrary::defaultPattern(), gameConfig_.getGameArenaSize());
    // bricksManager_.createBricksFromLayout(BrickLayoutLibrary::classicPattern(), gameConfig_.getGameArenaSize());
    // bricksManager_.createBricksFromLayout(BrickLayoutLibrary::pyramidPattern(), gameConfig_.getGameArenaSize());
    // bricksManager_.createBricksFromLayout(BrickLayoutLibrary::checkerboardPattern(), gameConfig_.getGameArenaSize());
    // bricksManager_.createBricksFromLayout(BrickLayoutLibrary::smilePattern(), gameConfig_.getGameArenaSize());
    // bricksManager_.createBricksFromLayout(BrickLayoutLibrary::manualPattern(), gameConfig_.getGameArenaSize());

    bricksModel_.refresh();

    scoreManager_ = std::make_unique<ScoreManager>();

    postInitialize();
}

void Model::reset()
{
    qDebug() <<"Resetting model";

    paddle_->reset();
    ball_->reset();
    bricksManager_.reset();

    bricksManager_.createBricksFromLayout(BrickLayoutLibrary::defaultPattern(), gameConfig_.getGameArenaSize());

    /* Tell QML to rebuild the model from scratch */
    bricksModel_.refresh();

    postInitialize();

    scoreManager_->reset();
}

void Model::postInitialize()
{
    if constexpr (constexpr bool ENABLE_BALL_VERTICAL_MOVEMENT_ONLY = false; ENABLE_BALL_VERTICAL_MOVEMENT_ONLY)
    {
        const auto& ballConfig = gameConfig_.getBallConfig();

        ball_->setVelocityX(0.0);
        ball_->setVelocityY(2 * ballConfig.INITIAL_VELOCITY_Y);
    }
}
