#pragma once

#include "entities/ball.h"
#include "entities/paddle.h"
#include <iostream>

static void printBallInfo(const Ball& ball)
{
    std::cout << "Ball(x=" << ball.getX() << " y=" << ball.getY() << " d=" << ball.getDiameter() << " vx=" << ball.getVelocityX() << " vy=" << ball.getVelocityY() << ")\n";
}

static void printPaddleInfo(const Paddle& paddle)
{
    std::cout << "Paddle(x=" << paddle.getX() << " y=" << paddle.getY() << " w=" << paddle.getWidth() << " h=" << paddle.getHeight() << ")\n";
}
