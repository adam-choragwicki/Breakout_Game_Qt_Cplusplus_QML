#include <gtest/gtest.h>

#include "collision_handling/brick_collision_handler.h"
#include "entities/brick.h"
#include "entities/ball.h"
#include "ball_manager.h"
#include "collision_common.h"
#include "bricks_model.h"

// -----------------------------------------------------------------------------
// Minimal BricksModel test double
// -----------------------------------------------------------------------------
struct TestBricksModel final : BricksModel
{
    int lastChangedIndex = -1;

    TestBricksModel() : BricksModel(nullptr) {}

    void brickChanged(const int index) override
    {
        lastChangedIndex = index;
    }
};

// -----------------------------------------------------------------------------
// Test helpers
// -----------------------------------------------------------------------------
Ball createBallForSide(const HitSide side)
{
    Ball ball(0, 0, 10, 50, 0);

    switch (side)
    {
        case HitSide::Left:
            ball.setX(90);
            ball.setY(100);
            ball.setVelocityX(50); // moving right -> will be reflected to negative
            break;
        case HitSide::Right:
            ball.setX(160);
            ball.setY(100);
            ball.setVelocityX(-50); // moving left -> will be reflected to positive
            break;
        case HitSide::Top:
            ball.setX(120);
            ball.setY(85);
            ball.setVelocityY(50); // moving down -> will be reflected to negative
            break;
        case HitSide::Bottom:
            ball.setX(120);
            ball.setY(135);
            ball.setVelocityY(-50); // moving up -> will be reflected to positive
            break;
    }
    return ball;
}

Brick createBrick() { return {100, 100, 50, 20, QColor("red")}; }

// -----------------------------------------------------------------------------
// LEFT side tests
// -----------------------------------------------------------------------------
TEST(BrickCollisionHandlerLeftTest, PositionAfterCollision_IsLeftOfBrick)
{
    Ball ball = createBallForSide(HitSide::Left);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Left, 0);

    // Not overlapping; left of brick
    EXPECT_LT(ball.getX() + ball.getDiameter(), brick.x());
    // Not teleported absurdly far
    EXPECT_GT(ball.getX(), brick.x() - 2 * ball.getDiameter());
}

TEST(BrickCollisionHandlerLeftTest, VelocityAfterCollision_IsReflectedNegative)
{
    Ball ball = createBallForSide(HitSide::Left);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Left, 0);

    EXPECT_LT(ball.getVelocityX(), 0); // reflected to negative
}

TEST(BrickCollisionHandlerLeftTest, BrickDestroyedAndModelNotified)
{
    Ball ball = createBallForSide(HitSide::Left);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Left, 0);

    EXPECT_FALSE(bricks[0].isAlive());
    EXPECT_EQ(model.lastChangedIndex, 0);
}

// -----------------------------------------------------------------------------
// RIGHT side tests
// -----------------------------------------------------------------------------
TEST(BrickCollisionHandlerRightTest, PositionAfterCollision_IsRightOfBrick)
{
    Ball ball = createBallForSide(HitSide::Right);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Right, 0);

    EXPECT_GT(ball.getX(), brick.x() + brick.width());
    EXPECT_LT(ball.getX(), brick.x() + brick.width() + 2 * ball.getDiameter());
}

TEST(BrickCollisionHandlerRightTest, VelocityAfterCollision_IsReflectedPositive)
{
    Ball ball = createBallForSide(HitSide::Right);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Right, 0);

    EXPECT_GT(ball.getVelocityX(), 0); // reflected to positive
}

TEST(BrickCollisionHandlerRightTest, BrickDestroyedAndModelNotified)
{
    Ball ball = createBallForSide(HitSide::Right);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Right, 0);

    EXPECT_FALSE(bricks[0].isAlive());
    EXPECT_EQ(model.lastChangedIndex, 0);
}

// -----------------------------------------------------------------------------
// TOP side tests
// -----------------------------------------------------------------------------
TEST(BrickCollisionHandlerTopTest, PositionAfterCollision_IsAboveBrick)
{
    Ball ball = createBallForSide(HitSide::Top);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Top, 0);

    EXPECT_LT(ball.getY() + ball.getDiameter(), brick.y());
}

TEST(BrickCollisionHandlerTopTest, VelocityAfterCollision_IsReflectedNegative)
{
    Ball ball = createBallForSide(HitSide::Top);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Top, 0);

    EXPECT_LT(ball.getVelocityY(), 0); // reflected upward (negative)
}

TEST(BrickCollisionHandlerTopTest, BrickDestroyedAndModelNotified)
{
    Ball ball = createBallForSide(HitSide::Top);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Top, 0);

    EXPECT_FALSE(bricks[0].isAlive());
    EXPECT_EQ(model.lastChangedIndex, 0);
}

// -----------------------------------------------------------------------------
// BOTTOM side tests
// -----------------------------------------------------------------------------
TEST(BrickCollisionHandlerBottomTest, PositionAfterCollision_IsBelowBrick)
{
    Ball ball = createBallForSide(HitSide::Bottom);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Bottom, 0);

    EXPECT_GT(ball.getY(), brick.y() + brick.height());
    EXPECT_LT(ball.getY(), brick.y() + brick.height() + 2 * ball.getDiameter());
}

TEST(BrickCollisionHandlerBottomTest, VelocityAfterCollision_IsReflectedPositive)
{
    Ball ball = createBallForSide(HitSide::Bottom);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Bottom, 0);

    EXPECT_GT(ball.getVelocityY(), 0); // reflected downward (positive)
}

TEST(BrickCollisionHandlerBottomTest, BrickDestroyedAndModelNotified)
{
    Ball ball = createBallForSide(HitSide::Bottom);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Bottom, 0);

    EXPECT_FALSE(bricks[0].isAlive());
    EXPECT_EQ(model.lastChangedIndex, 0);
}

// -----------------------------------------------------------------------------
// Repeated collision / idempotency (freeze-prevention) tests
// -----------------------------------------------------------------------------
TEST(BrickCollisionHandlerRepeatedCollisionTest, LeftSide_IdempotentAndNoFlipFlop)
{
    Ball ball = createBallForSide(HitSide::Left);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    // first (real) collision
    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Left, 0);

    // record state after first handling
    const qreal prevVx = ball.getVelocityX();
    const qreal prevX = ball.getX();

    // repeated calls should be no-op (brick already dead)
    for (int i = 0; i < 10; ++i)
    {
        handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Left, 0);
    }

    EXPECT_NE(prevVx, 0);
    EXPECT_LT(prevVx, 0); // should remain negative
    EXPECT_DOUBLE_EQ(ball.getVelocityX(), prevVx);
    EXPECT_DOUBLE_EQ(ball.getX(), prevX); // position unchanged (handler is idempotent)
}

TEST(BrickCollisionHandlerRepeatedCollisionTest, RightSide_IdempotentAndNoFlipFlop)
{
    Ball ball = createBallForSide(HitSide::Right);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Right, 0);
    const qreal prevVx = ball.getVelocityX();
    const qreal prevX = ball.getX();

    for (int i = 0; i < 10; ++i)
    {
        handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Right, 0);
    }

    EXPECT_NE(prevVx, 0);
    EXPECT_GT(prevVx, 0);
    EXPECT_DOUBLE_EQ(ball.getVelocityX(), prevVx);
    EXPECT_DOUBLE_EQ(ball.getX(), prevX);
}

TEST(BrickCollisionHandlerRepeatedCollisionTest, TopSide_IdempotentAndNoFlipFlop)
{
    Ball ball = createBallForSide(HitSide::Top);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Top, 0);
    const qreal prevVy = ball.getVelocityY();
    const qreal prevY = ball.getY();

    for (int i = 0; i < 10; ++i)
    {
        handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Top, 0);
    }

    EXPECT_NE(prevVy, 0);
    EXPECT_LT(prevVy, 0);
    EXPECT_DOUBLE_EQ(ball.getVelocityY(), prevVy);
    EXPECT_DOUBLE_EQ(ball.getY(), prevY);
}

TEST(BrickCollisionHandlerRepeatedCollisionTest, BottomSide_IdempotentAndNoFlipFlop)
{
    Ball ball = createBallForSide(HitSide::Bottom);
    BallManager manager(ball);
    BrickCollisionHandler handler(manager);

    const Brick brick = createBrick();
    std::vector<Brick> bricks{brick};
    TestBricksModel model;

    handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Bottom, 0);
    const qreal prevVy = ball.getVelocityY();
    const qreal prevY = ball.getY();

    for (int i = 0; i < 10; ++i)
    {
        handler.handleBallCollisionWithBrick(ball, bricks, model, HitSide::Bottom, 0);
    }

    EXPECT_NE(prevVy, 0);
    EXPECT_GT(prevVy, 0);
    EXPECT_DOUBLE_EQ(ball.getVelocityY(), prevVy);
    EXPECT_DOUBLE_EQ(ball.getY(), prevY);
}
