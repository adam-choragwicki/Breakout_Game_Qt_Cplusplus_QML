#include "paddle_collision_detector.h"
#include "collision_detection_utils.h"

std::optional<PaddleCollisionDetector::PaddleCollisionInfo> PaddleCollisionDetector::checkBallCollisionWithPaddle(const Ball& ball, const Paddle& paddle)
{
    const QRectF ballRect(ball.getX(), ball.getY(), ball.getDiameter(), ball.getDiameter());
    const QRectF paddleRect(paddle.getX(), paddle.getY(), paddle.getWidth(), paddle.getHeight());

    if (!isRectOverlappingRect(ballRect, paddleRect))
    {
        // Ball overlapping paddle is considered collision.
        // Ball simply touching paddle is considered no collision to avoid false positives during initial launch.
        return std::nullopt; // no collision
    }

    // Compute penetration/overlap on each hitSide
    const qreal overlapLeft = ballRect.right() - paddleRect.left();
    const qreal overlapRight = paddleRect.right() - ballRect.left();
    const qreal overlapTop = ballRect.bottom() - paddleRect.top();
    const qreal overlapBottom = paddleRect.bottom() - ballRect.top();

    qreal minOverlap = std::numeric_limits<qreal>::infinity();
    auto hitSide = HitSide::Top;

    auto test = [&](const qreal overlap, const HitSide side)
    {
        if (overlap > 0.0 && overlap < minOverlap)
        {
            minOverlap = overlap;
            hitSide = side;
        }
    };

    test(overlapLeft, HitSide::Left);
    test(overlapRight, HitSide::Right);
    test(overlapTop, HitSide::Top);
    test(overlapBottom, HitSide::Bottom);

    return PaddleCollisionInfo{hitSide};
}
