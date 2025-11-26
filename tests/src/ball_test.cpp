#include <gtest/gtest.h>
#include "entities/ball.h"

TEST(BallTest, ConstructorAndGetters)
{
    const Ball ball(10.0, 20.0, 5.0, 1.0, -1.0);

    EXPECT_DOUBLE_EQ(ball.getX(), 10.0);
    EXPECT_DOUBLE_EQ(ball.getY(), 20.0);
    EXPECT_DOUBLE_EQ(ball.getDiameter(), 5.0);
    EXPECT_DOUBLE_EQ(ball.getVelocityX(), 1.0);
    EXPECT_DOUBLE_EQ(ball.getVelocityY(), -1.0);
}

TEST(BallTest, Setters)
{
    Ball ball(0.0, 0.0, 5.0, 0.0, 0.0);

    ball.setX(15.0);
    ball.setY(25.0);
    ball.setVelocityX(2.0);
    ball.setVelocityY(-2.0);

    EXPECT_DOUBLE_EQ(ball.getX(), 15.0);
    EXPECT_DOUBLE_EQ(ball.getY(), 25.0);
    EXPECT_DOUBLE_EQ(ball.getVelocityX(), 2.0);
    EXPECT_DOUBLE_EQ(ball.getVelocityY(), -2.0);
}

TEST(BallTest, Reset)
{
    Ball ball(5.0, 10.0, 3.0, 1.0, 1.0);

    // Change position and velocity
    ball.setX(50.0);
    ball.setY(100.0);
    ball.setVelocityX(5.0);
    ball.setVelocityY(5.0);

    // Reset should return to initial values
    ball.reset();

    EXPECT_DOUBLE_EQ(ball.getX(), 5.0);
    EXPECT_DOUBLE_EQ(ball.getY(), 10.0);
    EXPECT_DOUBLE_EQ(ball.getVelocityX(), 1.0);
    EXPECT_DOUBLE_EQ(ball.getVelocityY(), 1.0);
}
