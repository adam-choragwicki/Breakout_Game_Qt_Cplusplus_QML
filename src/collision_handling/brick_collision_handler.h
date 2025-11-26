#pragma once

#include "collision_common.h"
#include "ball_manager.h"
#include "entities/ball.h"
#include "entities/brick.h"
#include "bricks_model.h"
#include "error_utils.h"

class BrickCollisionHandler
{
public:
    explicit BrickCollisionHandler(BallManager& ball_manager) : ballManager_(ball_manager) {}

    void handleBallCollisionWithBrick(Ball& ball, std::vector<Brick>& bricks, BricksModel& bricksModel, const HitSide brickHitSide, const int brickIndex)
    {
        Brick& brick = bricks[brickIndex];

        // Prevent multi-frame jitter: always push slightly further out
        constexpr qreal pushOutFactor = 0.5;

        switch (brickHitSide)
        {
            // Move the ball in a given direction with extra push out
            case HitSide::Left:
                ball.setX(brick.x() - ball.getDiameter() - pushOutFactor);
                ballManager_.bounceBallHorizontally();
                break;
            case HitSide::Right:
                ball.setX(brick.x() + brick.width() + pushOutFactor);
                ballManager_.bounceBallHorizontally();
                break;
            case HitSide::Top:
                ball.setY(brick.y() - ball.getDiameter() - pushOutFactor);
                ballManager_.bounceBallVertically();
                break;
            case HitSide::Bottom:
                ball.setY(brick.y() + brick.height() + pushOutFactor);
                ballManager_.bounceBallVertically();
                break;
            default:
                unreachable("Unexpected brickHitSide enum value");
        }

        qInfo() << "Brick destroyed: " << "ID:" << brickIndex << " x: " << brick.x() << " y: " << brick.y();
        brick.setAlive(false);
        bricksModel.brickChanged(brickIndex);
    }

private:
    BallManager& ballManager_;
};
