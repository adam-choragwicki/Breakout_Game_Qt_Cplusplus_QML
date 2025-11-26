#pragma once

#include "entities/ball.h"
#include "entities/paddle.h"
#include <QSize>

class ArenaEdgeCollisionDetector
{
public:
    static bool checkBallCollisionWithArenaBottomEdge(const Ball& ball, QSize gameArenaSize);
    static bool checkBallCollisionWithArenaTopEdge(const Ball& ball);
    static bool checkBallCollisionWithArenaLeftEdge(const Ball& ball);
    static bool checkBallCollisionWithArenaRightEdge(const Ball& ball, qreal rightEdgeX);
};
