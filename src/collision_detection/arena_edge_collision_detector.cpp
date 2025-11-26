#include "arena_edge_collision_detector.h"

bool ArenaEdgeCollisionDetector::checkBallCollisionWithArenaBottomEdge(const Ball& ball, const QSize gameArenaSize)
{
    return ball.getY() + ball.getDiameter() >= gameArenaSize.height();
}

bool ArenaEdgeCollisionDetector::checkBallCollisionWithArenaTopEdge(const Ball& ball)
{
    return ball.getY() <= 0;
}

bool ArenaEdgeCollisionDetector::checkBallCollisionWithArenaLeftEdge(const Ball& ball)
{
    return ball.getX() <= 0;
}

bool ArenaEdgeCollisionDetector::checkBallCollisionWithArenaRightEdge(const Ball& ball, const qreal rightEdgeX)
{
    return ball.getX() >= rightEdgeX;
}
