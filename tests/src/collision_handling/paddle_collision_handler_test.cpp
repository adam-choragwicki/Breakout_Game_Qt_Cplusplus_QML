#include <gtest/gtest.h>

#include "collision_handling/paddle_collision_handler.h"
#include "entities/ball.h"
#include "entities/paddle.h"
#include "ball_manager.h"
#include <cmath>

class PaddleCollisionHandlerTest : public ::testing::Test
{
protected:
    Ball ball{0, 0, 10, 0, 0};
    BallManager ballManager{ball};
    Paddle paddle{100, 100, 50, 10};
    PaddleCollisionHandler handler{ballManager};

    static constexpr double eps = 1e-6;
};

// ----------------------------------------------------------------------------
// No-op when ball already moving upward (should return immediately)
// ----------------------------------------------------------------------------
TEST_F(PaddleCollisionHandlerTest, NoOpWhenBallMovingUp)
{
    // Arrange: ball moving upward
    ball.setVelocityX(10.0);
    ball.setVelocityY(-50.0); // moving up -> handler must ignore

    // Record before
    const qreal beforeX = ball.getX();
    const qreal beforeY = ball.getY();
    const qreal beforeVx = ball.getVelocityX();
    const qreal beforeVy = ball.getVelocityY();

    // Act
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Top);
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Left);
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Right);
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Bottom);

    // Assert: nothing changed
    EXPECT_DOUBLE_EQ(beforeX, ball.getX());
    EXPECT_DOUBLE_EQ(beforeY, ball.getY());
    EXPECT_DOUBLE_EQ(beforeVx, ball.getVelocityX());
    EXPECT_DOUBLE_EQ(beforeVy, ball.getVelocityY());
}

// ----------------------------------------------------------------------------
// Top hit: center of paddle -> straight upward (angle = 0)
// ----------------------------------------------------------------------------
TEST_F(PaddleCollisionHandlerTest, TopCollision_CenterHit_PreservesSpeedAndGoesUp)
{
    // Arrange: put ball above paddle and overlapping (so handler normally called after detection)
    // Set initial incoming velocity: downward positive Vy
    ball.setVelocityX(30.0);
    ball.setVelocityY(40.0); // downward

    // Place the ball horizontally centered on the paddle
    const qreal paddleCenterX = paddle.getX() + paddle.getWidth() * 0.5;
    ball.setX(paddleCenterX - ball.getDiameter() * 0.5);

    // Place the ball slightly overlapping vertically (a colliding scenario)
    ball.setY(paddle.getY() - ball.getDiameter() + 1.0);

    const qreal speedBefore = std::hypot(ball.getVelocityX(), ball.getVelocityY());

    // Act
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Top);

    // Assert: ball was pushed out above paddle by verticalPushOutFactor (0.5)
    EXPECT_DOUBLE_EQ(ball.getY(), paddle.getY() - ball.getDiameter() - 0.5);

    // velocities: center hit => rel == 0 -> angle = 0 -> vx ~= 0, vy ~= -speed
    EXPECT_NEAR(0.0, ball.getVelocityX(), eps);
    EXPECT_NEAR(-speedBefore, ball.getVelocityY(), 1e-5);
}

// ----------------------------------------------------------------------------
// Top hit: near left edge => angled left-up; speed preserved
// ----------------------------------------------------------------------------
TEST_F(PaddleCollisionHandlerTest, TopCollision_EdgeHit_LeftProducesLeftUpAngle)
{
    // Arrange
    ball.setVelocityX(10.0);
    ball.setVelocityY(20.0); // downward

    // Place ball near the left edge => rel should be ~ -1
    // To place at extreme left: ballCenterX = paddleCenter - paddleWidth/2 => ball.x = paddle.x - ball.d / 2,
    // But we will place slightly inside to ensure some overlap and rel approximately -1
    constexpr qreal relTarget = -1.0;
    const qreal paddleCenterX = paddle.getX() + paddle.getWidth() * 0.5;
    const qreal ballCenterX = paddleCenterX + relTarget * (paddle.getWidth() * 0.5);
    ball.setX(ballCenterX - ball.getDiameter() * 0.5);

    ball.setY(paddle.getY() - ball.getDiameter() + 1.0);

    const qreal speedBefore = std::hypot(ball.getVelocityX(), ball.getVelocityY());

    // Act
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Top);

    // Angle should be -60 degrees (max) for rel=-1 -> vx = speed * sin(-60) = -speed * sin(60)
    const qreal expectedVx = speedBefore * std::sin(-M_PI / 3.0); // -60deg
    const qreal expectedVy = -speedBefore * std::cos(M_PI / 3.0); // negative

    EXPECT_NEAR(expectedVx, ball.getVelocityX(), 1e-5);
    EXPECT_NEAR(expectedVy, ball.getVelocityY(), 1e-5);

    // Ensure the ball moved above paddle as push-out
    EXPECT_DOUBLE_EQ(ball.getY(), paddle.getY() - ball.getDiameter() - 0.5);
}

// ----------------------------------------------------------------------------
// Left side collision: ball placed overlapping left -> should be pushed left and vx mirrored
// ----------------------------------------------------------------------------
TEST_F(PaddleCollisionHandlerTest, LeftCollision_PushesLeftAndMirrorsVx)
{
    // Arrange: ball moving to the right (positive vx), collides with paddle left side
    ball.setVelocityX(50.0);
    ball.setVelocityY(0.0);

    // Put the ball-overlapping paddle on the left side
    ball.setX(paddle.getX() - ball.getDiameter() + 1.0);
    ball.setY(paddle.getY() + 2.0);

    // Act
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Left);

    // Assert: ball.x set to paddle.x - ball.d - horizontalPushOutFactor (0.5)
    EXPECT_DOUBLE_EQ(ball.getX(), paddle.getX() - ball.getDiameter() - 0.5);

    // vx should be mirrored (negative)
    EXPECT_LT(ball.getVelocityX(), 0.0);
}

// ----------------------------------------------------------------------------
// Right side collision: push to right and mirror vx
// ----------------------------------------------------------------------------
TEST_F(PaddleCollisionHandlerTest, RightCollision_PushesRightAndMirrorsVx)
{
    // Arrange: ball moving left (negative vx) hitting paddle right side
    ball.setVelocityX(-30.0);
    ball.setVelocityY(0.0);

    // Place the ball overlapping right
    ball.setX(paddle.getX() + paddle.getWidth() - 1.0);
    ball.setY(paddle.getY() + 2.0);

    // Act
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Right);

    // Assert: pushed to paddle.x + paddle.width + 0.5
    EXPECT_DOUBLE_EQ(ball.getX(), paddle.getX() + paddle.getWidth() + 0.5);

    // vx should be mirrored (now positive)
    EXPECT_GT(ball.getVelocityX(), 0.0);
}

// ----------------------------------------------------------------------------
// Bottom collisions are intentionally ignored by handler
// ----------------------------------------------------------------------------
TEST_F(PaddleCollisionHandlerTest, BottomCollision_IsIgnored)
{
    // Arrange
    ball.setVelocityX(10.0);
    ball.setVelocityY(30.0); // downward
    const qreal beforeX = ball.getX();
    const qreal beforeY = ball.getY();
    const qreal beforeVx = ball.getVelocityX();
    const qreal beforeVy = ball.getVelocityY();

    // Act
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Bottom);

    // Assert: nothing changed
    EXPECT_DOUBLE_EQ(beforeX, ball.getX());
    EXPECT_DOUBLE_EQ(beforeY, ball.getY());
    EXPECT_DOUBLE_EQ(beforeVx, ball.getVelocityX());
    EXPECT_DOUBLE_EQ(beforeVy, ball.getVelocityY());
}

// ----------------------------------------------------------------------------
// Idempotency: after a top bump, calling handler again is a no-op because the ball is moving upward
// ----------------------------------------------------------------------------
TEST_F(PaddleCollisionHandlerTest, AfterTopBounce_SecondCallIsNoOp)
{
    // Arrange: falling ball
    ball.setVelocityX(5.0);
    ball.setVelocityY(20.0);
    // place ball for a top collision center
    const qreal paddleCenterX = paddle.getX() + paddle.getWidth() * 0.5;
    ball.setX(paddleCenterX - ball.getDiameter() * 0.5);
    ball.setY(paddle.getY() - ball.getDiameter() + 1.0);

    // Act: first collision -> should bounce upward
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Top);
    EXPECT_LT(ball.getVelocityY(), 0.0); // upward now

    // Record velocities after first bounce
    const qreal vxAfter = ball.getVelocityX();
    const qreal vyAfter = ball.getVelocityY();
    const qreal xAfter = ball.getX();
    const qreal yAfter = ball.getY();

    // Act again (should early-return because vy < 0)
    handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Top);

    // Assert: unchanged
    EXPECT_DOUBLE_EQ(vxAfter, ball.getVelocityX());
    EXPECT_DOUBLE_EQ(vyAfter, ball.getVelocityY());
    EXPECT_DOUBLE_EQ(xAfter, ball.getX());
    EXPECT_DOUBLE_EQ(yAfter, ball.getY());
}

TEST_F(PaddleCollisionHandlerTest, RepeatedTopCollision_WhenAlreadyMovingUpButOverlapping_PushesOut)
{
    // Simulate the ball already moving upward but still overlapping the paddle.

    // Arrange: set the ball overlapping slightly and already moving upward
    ball.setVelocityX(0.0);
    ball.setVelocityY(-10.0); // already moving up
    const qreal paddleCenterX = paddle.getX() + paddle.getWidth() * 0.5;
    ball.setX(paddleCenterX - ball.getDiameter() * 0.5);
    ball.setY(paddle.getY() - ball.getDiameter() + 0.1); // slight overlap

    const qreal beforeY = ball.getY();

    // Act: multiple handler invocations that mimic repeated detection calls
    for (int i = 0; i < 5; ++i)
    {
        handler.handleBallCollisionWithPaddle(ball, paddle, HitSide::Top);
    }

    // Assert: after handling, the ball must be pushed fully above the paddle and remain moving upward
    EXPECT_LT(ball.getY(), beforeY - 0.01); // moved up
    EXPECT_DOUBLE_EQ(ball.getY(), paddle.getY() - ball.getDiameter() - 0.5); // pushed out by verticalPushOutFactor
    EXPECT_LT(ball.getVelocityY(), 0.0); // still moving upward
}
