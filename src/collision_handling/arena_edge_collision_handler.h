#pragma once

#include "ball_manager.h"
#include "entities/ball.h"
#include "config/game_config.h"
#include <QSize>

class ArenaEdgeCollisionHandler
{
public:
    explicit ArenaEdgeCollisionHandler(const GameConfig& gameConfig, BallManager& ball_manager) : ENABLE_BALL_TOUCHING_FLOOR_ENDS_GAME(gameConfig.isBallTouchingFloorEndingGameEnabled()),
                                                                                                  ballManager_(ball_manager)
    {}

    enum class ArenaBottomEdgeCollisionResult
    {
        BallBouncedOffArenaBottomEdge,
        GameLost,
    };

    ArenaBottomEdgeCollisionResult handleBallCollisionWithArenaBottomEdge(Ball& ball, const QSize& gameArenaSize)
    {
        // Fix position
        ball.setY(gameArenaSize.height() - ball.getDiameter());

        if (ENABLE_BALL_TOUCHING_FLOOR_ENDS_GAME)
        {
            qInfo() << "Collision: Ball hit bottom edge => Game Over";
            return ArenaBottomEdgeCollisionResult::GameLost;
        }
        else
        {
            qDebug() << "Collision: Ball hit bottom edge => Bounce";

            // Bounce vertically (upward)
            ballManager_.bounceBallVertically();
            return ArenaBottomEdgeCollisionResult::BallBouncedOffArenaBottomEdge;
        }
    }

private:
    const bool ENABLE_BALL_TOUCHING_FLOOR_ENDS_GAME;
    BallManager& ballManager_;
};
