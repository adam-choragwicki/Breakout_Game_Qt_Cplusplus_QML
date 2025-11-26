#include "brick_collision_detector.h"
#include "collision_detection_utils.h"

std::optional<BrickCollisionDetector::BrickCollisionInfo> BrickCollisionDetector::checkBallCollisionWithBricks(const Ball& ball, const std::vector<Brick>& bricks)
{
    const QRectF ballRect(ball.getX(), ball.getY(), ball.getDiameter(), ball.getDiameter());

    for (int brickIndex = 0; brickIndex < bricks.size(); ++brickIndex)
    {
        const Brick& brick = bricks[brickIndex];

        if (!brick.isAlive())
        {
            continue;
        }

        const QRectF brickRect(brick.x(), brick.y(), brick.width(), brick.height());

        if (!isRectOverlappingOrTouchingRect(ballRect, brickRect))
        {
            // Ball overlapping brick is considered collision.
            // Ball simply touching brick is also considered collision.
            continue; // no collision
        }

        // Compute penetration/overlap on each hitSide (allow zero for touching)
        const qreal overlapLeft = ballRect.right() - brickRect.left();
        const qreal overlapRight = brickRect.right() - ballRect.left();
        const qreal overlapTop = ballRect.bottom() - brickRect.top();
        const qreal overlapBottom = brickRect.bottom() - ballRect.top();

        qreal minOverlap = std::numeric_limits<qreal>::infinity();
        auto hitSide = HitSide::Top;

        auto test = [&](const qreal overlap, const HitSide side)
        {
            if (overlap >= 0.0 && overlap < minOverlap)
            {
                minOverlap = overlap;
                hitSide = side;
            }
        };

        // Choose the side with the smallest penetration.
        // If multiple overlaps are equal (e.g., corner touch), choose the first in order: Left → Right → Top → Bottom.
        test(overlapLeft, HitSide::Left);
        test(overlapRight, HitSide::Right);
        test(overlapTop, HitSide::Top);
        test(overlapBottom, HitSide::Bottom);

        return BrickCollisionInfo{hitSide, brickIndex}; // return first collided brick
    }

    return std::nullopt; // no collision
}
