#include <gtest/gtest.h>
#include "entities/paddle.h"

TEST(PaddleTest, ConstructorAndGetters)
{
    const Paddle paddle(10.0, 20.0, 100.0, 20.0);

    EXPECT_DOUBLE_EQ(paddle.getX(), 10.0);
    EXPECT_DOUBLE_EQ(paddle.getY(), 20.0);
    EXPECT_DOUBLE_EQ(paddle.getWidth(), 100.0);
    EXPECT_DOUBLE_EQ(paddle.getHeight(), 20.0);
}

TEST(PaddleTest, Setters)
{
    Paddle paddle(0.0, 0.0, 50.0, 10.0);

    paddle.setX(30.0);
    paddle.setY(40.0);

    EXPECT_DOUBLE_EQ(paddle.getX(), 30.0);
    EXPECT_DOUBLE_EQ(paddle.getY(), 40.0);
    EXPECT_DOUBLE_EQ(paddle.getWidth(), 50.0); // Width should remain unchanged
    EXPECT_DOUBLE_EQ(paddle.getHeight(), 10.0); // Height should remain unchanged
}

TEST(PaddleTest, Reset)
{
    Paddle paddle(5.0, 10.0, 60.0, 15.0);

    // Move paddle to new position
    paddle.setX(100.0);
    paddle.setY(200.0);

    // Reset should restore the initial position
    paddle.reset();

    EXPECT_DOUBLE_EQ(paddle.getX(), 5.0);
    EXPECT_DOUBLE_EQ(paddle.getY(), 10.0);
    EXPECT_DOUBLE_EQ(paddle.getWidth(), 60.0);
    EXPECT_DOUBLE_EQ(paddle.getHeight(), 15.0);
}
