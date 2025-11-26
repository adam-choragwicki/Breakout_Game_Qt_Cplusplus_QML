#include <gtest/gtest.h>
#include "collision_detection/brick_collision_detector.h"
#include "entities/ball.h"
#include "entities/brick.h"

class BrickCollisionDetectorTest : public ::testing::Test
{
protected:
    Ball ball{0, 0, 10, 0, 0};
    std::vector<Brick> bricks;

    static constexpr int brickWidth = 20;
    static constexpr int brickHeight = 10;

    void addRectangularBrick(int x, int y)
    {
        bricks.emplace_back(x, y, brickWidth, brickHeight, Qt::red);
    }
};

// -----------------------------------------------------------------------------
// NO COLLISION
// -----------------------------------------------------------------------------
TEST_F(BrickCollisionDetectorTest, BallSeparatedFromBricksReturnsNoCollision)
{
    addRectangularBrick(100, 100);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);

    EXPECT_FALSE(collision);
}

// -----------------------------------------------------------------------------
// SINGLE SIDE COLLISIONS
// -----------------------------------------------------------------------------
TEST_F(BrickCollisionDetectorTest, CollisionFromTop)
{
    addRectangularBrick(50, 50);
    const Brick& brick = bricks[0];

    ball.setX(brick.x() + brick.width() * 0.5 - ball.getDiameter() * 0.5);
    ball.setY(brick.y() - ball.getDiameter() + 1);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);

    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->brickIndex, 0);
    EXPECT_EQ(collision->hitSide, HitSide::Top);
}

TEST_F(BrickCollisionDetectorTest, CollisionFromBottom)
{
    addRectangularBrick(50, 50);
    const Brick& brick = bricks[0];

    ball.setX(brick.x() + 5);
    ball.setY(brick.y() + brick.height() - 1);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);

    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Bottom);
}

TEST_F(BrickCollisionDetectorTest, CollisionFromLeft)
{
    addRectangularBrick(50, 50);
    const Brick& brick = bricks[0];

    ball.setX(brick.x() - ball.getDiameter() + 1);
    ball.setY(brick.y() + 2);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);

    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Left);
}

TEST_F(BrickCollisionDetectorTest, CollisionFromRight)
{
    addRectangularBrick(50, 50);
    const Brick& brick = bricks[0];

    ball.setX(brick.x() + brick.width() - 1);
    ball.setY(brick.y() + 2);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);

    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Right);
}

// -----------------------------------------------------------------------------
// TOUCHING EDGES
// -----------------------------------------------------------------------------
TEST_F(BrickCollisionDetectorTest, TouchingLeftEdgeCountsAsCollision)
{
    addRectangularBrick(50, 50);
    const Brick& brick = bricks[0];

    ball.setX(brick.x() - ball.getDiameter());
    ball.setY(brick.y() + 5);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    EXPECT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Left);
}

TEST_F(BrickCollisionDetectorTest, TouchingRightEdgeCountsAsCollision)
{
    addRectangularBrick(50, 50);
    const Brick& brick = bricks[0];

    ball.setX(brick.x() + brick.width());
    ball.setY(brick.y() + 5);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    EXPECT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Right);
}

TEST_F(BrickCollisionDetectorTest, TouchingBottomEdgeCountsAsCollision)
{
    addRectangularBrick(50, 50);
    const Brick& brick = bricks[0];

    ball.setX(brick.x() + 5);
    ball.setY(brick.y() + brick.height());

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    EXPECT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Bottom);
}

// -----------------------------------------------------------------------------
// CORNER / TIE BREAKING
// -----------------------------------------------------------------------------
TEST_F(BrickCollisionDetectorTest, TopLeftCornerTieBreaker)
{
    addRectangularBrick(100, 100);
    const Brick& brick = bricks[0];

    constexpr qreal k = 1.0;
    ball.setX(brick.x() - ball.getDiameter() + k);
    ball.setY(brick.y() - ball.getDiameter() + k);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Left);
}

TEST_F(BrickCollisionDetectorTest, TopRightCornerTieBreaker)
{
    addRectangularBrick(100, 100);
    const Brick& brick = bricks[0];

    constexpr qreal k = 1.0;
    ball.setX(brick.x() + brick.width() - k);
    ball.setY(brick.y() - ball.getDiameter() + k);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Right);
}

TEST_F(BrickCollisionDetectorTest, BottomLeftCornerTieBreaker)
{
    addRectangularBrick(100, 100);
    const Brick& brick = bricks[0];

    constexpr qreal k = 1.0;
    ball.setX(brick.x() - ball.getDiameter() + k);
    ball.setY(brick.y() + brick.height() - k);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Left);
}

TEST_F(BrickCollisionDetectorTest, BottomRightCornerTieBreaker)
{
    addRectangularBrick(100, 100);
    const Brick& brick = bricks[0];

    constexpr qreal k = 1.0;
    ball.setX(brick.x() + brick.width() - k);
    ball.setY(brick.y() + brick.height() - k);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->hitSide, HitSide::Right);
}

// -----------------------------------------------------------------------------
// MULTI-BRICK SCENARIOS
// -----------------------------------------------------------------------------
TEST_F(BrickCollisionDetectorTest, IgnoresDeadBricksAndHitsNextAlive)
{
    addRectangularBrick(10, 10);
    addRectangularBrick(30, 10);
    bricks[0].setAlive(false);

    const Brick& second = bricks[1];
    ball.setX(second.x() + 1);
    ball.setY(second.y() + 1);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->brickIndex, 1);
}

TEST_F(BrickCollisionDetectorTest, MultipleBricksSimultaneousCollisionReturnsFirst)
{
    addRectangularBrick(10, 10);
    addRectangularBrick(15, 12);

    ball.setX(16);
    ball.setY(12);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->brickIndex, 0);
}

// -----------------------------------------------------------------------------
// EDGE CASES: Ball partially outside arena
// -----------------------------------------------------------------------------
TEST_F(BrickCollisionDetectorTest, BallNegativeCoordinatesStillDetectsCollision)
{
    addRectangularBrick(-5, -5);

    ball.setX(-4);
    ball.setY(-4);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    ASSERT_TRUE(collision);
}

TEST_F(BrickCollisionDetectorTest, BallPartiallyOutsideRightDetectsCollision)
{
    addRectangularBrick(90, 0);

    ball.setX(95);
    ball.setY(5);

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks);
    ASSERT_TRUE(collision);
}

// -----------------------------------------------------------------------------
// ROBUSTNESS
// -----------------------------------------------------------------------------
TEST_F(BrickCollisionDetectorTest, VeryLargeBallDetectsCollision)
{
    addRectangularBrick(40, 40);

    const Ball largeBall(30, 30, 100, 0, 0); // new instance

    const auto collision = BrickCollisionDetector::checkBallCollisionWithBricks(largeBall, bricks);
    ASSERT_TRUE(collision);
    EXPECT_EQ(collision->brickIndex, 0);
}
