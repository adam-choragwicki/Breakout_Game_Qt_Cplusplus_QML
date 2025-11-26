#include "ball_manager.h"
#include <QtMath>
#include <QRandomGenerator>

BallManager::BallManager(Ball& ball) : ball_(ball) {}

void BallManager::launchBall()
{
    if constexpr (constexpr bool ENABLE_RANDOM_BALL_LAUNCH_SPREAD = true; ENABLE_RANDOM_BALL_LAUNCH_SPREAD)
    {
        qDebug() << "Launching ball at random angle";

        const qreal baseVelocityX = ball_.getVelocityX();
        const qreal baseVelocityY = ball_.getVelocityY();

        constexpr qreal angleSpreadDegrees = 10.0; // +/- degrees offset from vertical
        const qreal offsetDegrees = -angleSpreadDegrees + QRandomGenerator::global()->generateDouble() * (2.0 * angleSpreadDegrees);
        const qreal angleRadians = qDegreesToRadians(90.0 + offsetDegrees); // 90 = straight up

        const qreal velocityX = baseVelocityX * std::cos(angleRadians);
        const qreal velocityY = -baseVelocityY * std::sin(angleRadians);

        ball_.setVelocityX(velocityX);
        ball_.setVelocityY(velocityY);
    }
    else
    {
        qDebug() << "Launching ball straight upward";

        // Straight upward launch
        constexpr qreal baseVx = 0.0;
        ball_.setVelocityX(baseVx);
        ball_.setVelocityY(-ball_.getVelocityY()); // negative = upward
    }
}

void BallManager::moveBall(qreal dtStep)
{
    ball_.setX(ball_.getX() + ball_.getVelocityX() * dtStep);
    ball_.setY(ball_.getY() + ball_.getVelocityY() * dtStep);
}

void BallManager::bounceBallHorizontally()
{
    ball_.setVelocityX(-ball_.getVelocityX());
}

void BallManager::bounceBallVertically()
{
    ball_.setVelocityY(-ball_.getVelocityY());
}
