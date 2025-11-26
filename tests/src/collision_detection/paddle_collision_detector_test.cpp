#include <gtest/gtest.h>
#include "collision_detection/paddle_collision_detector.h"
#include "entities/ball.h"
#include "entities/paddle.h"

class PaddleCollisionDetectorTest : public ::testing::Test
{
protected:
    Ball ball{0, 0, 10, 0, 0};
    Paddle paddle{100, 100, 50, 10}; // default paddle for most tests
};

// -----------------------------------------------------------------------------
// No collision
// -----------------------------------------------------------------------------
TEST_F(PaddleCollisionDetectorTest, NoCollisionWhenSeparated)
{
    const auto collided = PaddleCollisionDetector::checkBallCollisionWithPaddle(ball, paddle);

    EXPECT_FALSE(collided);
}

// -----------------------------------------------------------------------------
// Collision: Top
// -----------------------------------------------------------------------------
TEST_F(PaddleCollisionDetectorTest, BallOverlappingPaddleTopEdgeReturnsCollision)
{
    // Ball overlapping from above
    ball.setX(paddle.getX() + paddle.getWidth() * 0.5 - ball.getDiameter() * 0.5);
    ball.setY(paddle.getY() - ball.getDiameter() + 1);

    const auto collided = PaddleCollisionDetector::checkBallCollisionWithPaddle(ball, paddle);

    EXPECT_TRUE(collided);
    EXPECT_EQ(collided->hitSide, HitSide::Top);
}

// -----------------------------------------------------------------------------
// Collision: Left
// -----------------------------------------------------------------------------
TEST_F(PaddleCollisionDetectorTest, BallOverlappingPaddleLeftEdgeReturnsCollision)
{
    ball.setX(paddle.getX() - ball.getDiameter() + 1);
    ball.setY(paddle.getY() + 2);

    const auto collided = PaddleCollisionDetector::checkBallCollisionWithPaddle(ball, paddle);

    EXPECT_TRUE(collided);
    EXPECT_EQ(collided->hitSide, HitSide::Left);
}

// -----------------------------------------------------------------------------
// Collision: Right
// -----------------------------------------------------------------------------
TEST_F(PaddleCollisionDetectorTest, BallOverlappingPaddleRightEdgeReturnsCollision)
{
    ball.setX(paddle.getX() + paddle.getWidth() - 1);
    ball.setY(paddle.getY() + 2);

    const auto collided = PaddleCollisionDetector::checkBallCollisionWithPaddle(ball, paddle);

    EXPECT_TRUE(collided);
    EXPECT_EQ(collided->hitSide, HitSide::Right);
}

// -----------------------------------------------------------------------------
// Touching but not collision
// -----------------------------------------------------------------------------
TEST_F(PaddleCollisionDetectorTest, BallTouchingPaddleBottomReturnsNoCollision)
{
    ball.setX(paddle.getX() + paddle.getWidth() * 0.5 - ball.getDiameter() * 0.5);
    ball.setY(paddle.getY() + paddle.getHeight());

    const auto collided = PaddleCollisionDetector::checkBallCollisionWithPaddle(ball, paddle);

    EXPECT_FALSE(collided);
}

// -----------------------------------------------------------------------------
// Corner / Tie-break rule
// -----------------------------------------------------------------------------
TEST_F(PaddleCollisionDetectorTest, CornerOverlapTieBreakerPrefersLeft)
{
    constexpr qreal k = 1.0;
    ball.setX(paddle.getX() - ball.getDiameter() + k);
    ball.setY(paddle.getY() - ball.getDiameter() + k);

    const auto collided = PaddleCollisionDetector::checkBallCollisionWithPaddle(ball, paddle);

    EXPECT_TRUE(collided);
    EXPECT_EQ(collided->hitSide, HitSide::Left);
}
