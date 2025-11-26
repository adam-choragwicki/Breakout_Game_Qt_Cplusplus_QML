#include <gtest/gtest.h>
#include "collision_detection/arena_edge_collision_detector.h"
#include "entities/ball.h"

class ArenaEdgeCollisionDetectorTest : public ::testing::Test
{
protected:
    static constexpr QSize arenaSize{100, 100};
    Ball ball{0, 0, 10, 0, 0};
};

// -------------------------------------------------------------
// Ball collision with arena bottom edge
// -------------------------------------------------------------
TEST_F(ArenaEdgeCollisionDetectorTest, BallTouchingArenaBottomEdgeReturnsCollision)
{
    ball.setY(arenaSize.height() - ball.getDiameter()); // ball touching arena bottom edge
    EXPECT_TRUE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaBottomEdge(ball, arenaSize));
}

TEST_F(ArenaEdgeCollisionDetectorTest, BallOverlappingArenaBottomEdgeReturnsCollision)
{
    ball.setY(arenaSize.height() - ball.getDiameter() + 1); // ball overlapping arena bottom edge
    EXPECT_TRUE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaBottomEdge(ball, arenaSize));
}

TEST_F(ArenaEdgeCollisionDetectorTest, BallAboveArenaBottomEdgeReturnsNoCollision)
{
    ball.setY(arenaSize.height() - ball.getDiameter() - 1); // ball above arena bottom edge
    EXPECT_FALSE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaBottomEdge(ball, arenaSize));
}

// -------------------------------------------------------------
// Ball collision with arena top edge
// -------------------------------------------------------------
TEST_F(ArenaEdgeCollisionDetectorTest, BallTouchingArenaTopEdgeReturnsCollision)
{
    ball.setY(0); // ball touching arena top edge
    EXPECT_TRUE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaTopEdge(ball));
}

TEST_F(ArenaEdgeCollisionDetectorTest, BallOverlappingArenaTopEdgeReturnsCollision)
{
    ball.setY(-1); // ball overlapping arena top edge
    EXPECT_TRUE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaTopEdge(ball));
}

TEST_F(ArenaEdgeCollisionDetectorTest, BallBelowArenaTopEdgeReturnsNoCollision)
{
    ball.setY(1); // ball below arena top edge
    EXPECT_FALSE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaTopEdge(ball));
}

// -------------------------------------------------------------
// Ball collision with arena left edge
// -------------------------------------------------------------
TEST_F(ArenaEdgeCollisionDetectorTest, BallTouchingArenaLeftEdgeReturnsCollision)
{
    ball.setX(0); // ball touching arena left edge
    EXPECT_TRUE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaLeftEdge(ball));
}

TEST_F(ArenaEdgeCollisionDetectorTest, BallOverlappingArenaLeftEdgeReturnsCollision)
{
    ball.setX(-1); // ball overlapping arena left edge
    EXPECT_TRUE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaLeftEdge(ball));
}

TEST_F(ArenaEdgeCollisionDetectorTest, BallRightOfArenaLeftEdgeReturnsNoCollision)
{
    ball.setX(1); // the ball is to the right of an arena left edge
    EXPECT_FALSE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaLeftEdge(ball));
}

// -------------------------------------------------------------
// Ball collision with arena right edge
// -------------------------------------------------------------
TEST_F(ArenaEdgeCollisionDetectorTest, BallTouchingArenaRightEdgeReturnsCollision)
{
    const qreal rightX = arenaSize.width() - ball.getDiameter();
    ball.setX(rightX); // ball touching arena right edge
    EXPECT_TRUE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaRightEdge(ball, rightX));
}

TEST_F(ArenaEdgeCollisionDetectorTest, BallOverlappingArenaRightEdgeReturnsCollision)
{
    const qreal rightX = arenaSize.width() - ball.getDiameter();
    ball.setX(rightX + 1); // ball overlapping arena right edge
    EXPECT_TRUE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaRightEdge(ball, rightX));
}

TEST_F(ArenaEdgeCollisionDetectorTest, BallLeftOfArenaRightEdgeReturnsNoCollision)
{
    const qreal rightX = arenaSize.width() - ball.getDiameter();
    ball.setX(rightX - 1); // the ball is to the left of an arena right edge
    EXPECT_FALSE(ArenaEdgeCollisionDetector::checkBallCollisionWithArenaRightEdge(ball, rightX));
}
