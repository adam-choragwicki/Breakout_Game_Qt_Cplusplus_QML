#pragma once

#include "entities/ball.h"

class BallManager
{
public:
    explicit BallManager(Ball& ball);
    void launchBall();

    void moveBall(qreal dtStep);

    void bounceBallHorizontally();
    void bounceBallVertically();

    Ball& ball_;
};
