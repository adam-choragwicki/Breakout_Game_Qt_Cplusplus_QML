#pragma once

#include "collision_common.h"
#include "ball_manager.h"
#include "entities/ball.h"
#include "entities/paddle.h"
#include "error_utils.h"
#include <algorithm>
#include <QtMath>

class PaddleCollisionHandler
{
public:
    explicit PaddleCollisionHandler(BallManager& ball_manager) : ballManager_(ball_manager) {}

    void handleBallCollisionWithPaddle(Ball& ball, const Paddle& paddle, const HitSide paddleHitSide)
    {
        constexpr qreal EPS = 1e-6; // epsilon for floating point comparisons
        const qreal ballBottom = ball.getY() + ball.getDiameter();
        const qreal paddleTop = paddle.getY();

        if (ball.getVelocityY() < 0.0 && ballBottom <= paddleTop + EPS)
        {
            // Ball already moving up and not overlapping -> ignore
            return;
        }

        // Handle collision based on the hit side
        switch (paddleHitSide)
        {
            case HitSide::Top:
            {
                // Handle topside hit
                // Apply an angle to the bounce depending on where the ball hits relative to the paddle center.
                // Center hit → mostly straight upward
                // Edge hit → more steep angled bounce
                constexpr double verticalPushOutFactor = 0.5;

                // First physically move ball above paddle
                ball.setY(paddle.getY() - ball.getDiameter() - verticalPushOutFactor);

                // Compute relative position of ball on paddle in [-1, 1]
                const qreal paddleCenterX = paddle.getX() + paddle.getWidth() * 0.5;
                const qreal ballCenterX = ball.getX() + ball.getDiameter() * 0.5;

                qreal rel = (ballCenterX - paddleCenterX) / (paddle.getWidth() * 0.5);
                rel = std::clamp(rel, -1.0, 1.0);

                // Angle deviation (max 60°)
                constexpr qreal maxAngleDegrees = 60.0;
                const qreal angle = qDegreesToRadians(rel * maxAngleDegrees);

                // Preserve original ball speed
                const qreal speed = std::hypot(ball.getVelocityX(), ball.getVelocityY());

                // Set new velocity components
                ball.setVelocityX(speed * std::sin(angle)); // angled left/right
                ball.setVelocityY(-speed * std::cos(angle)); // always upward
                return;
            }
            case HitSide::Left:
            case HitSide::Right:
            {
                // Handle horizontal hits (Left or Right)
                // Side-hit → reflect horizontally. Push the ball outside the paddle to prevent re-colliding on the next frame.
                constexpr double horizontalPushOutFactor = 0.5;

                if (paddleHitSide == HitSide::Left)
                {
                    // Ball hit the left side -> place it on the left
                    ball.setX(paddle.getX() - ball.getDiameter() - horizontalPushOutFactor);
                }
                else
                {
                    // Ball hit the right side -> place it on the right
                    ball.setX(paddle.getX() + paddle.getWidth() + horizontalPushOutFactor);
                }

                // Mirror horizontal velocity
                ballManager_.bounceBallHorizontally();
                return;
            }
            case HitSide::Bottom:
            {
                // Ignore collisions from the bottom of the paddle
                return;
            }
            default:
            {
                // Defensive: enum not implemented or memory corruption
                unreachable("Unexpected paddleHitSide enum value");
            }
        }
    }

private:
    BallManager& ballManager_;
};
