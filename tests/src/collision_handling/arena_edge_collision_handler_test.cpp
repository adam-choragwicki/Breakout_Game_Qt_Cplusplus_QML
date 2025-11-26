#include <gtest/gtest.h>

#include "collision_handling/arena_edge_collision_handler.h"
#include "config/game_config.h"
#include "entities/ball.h"
#include "ball_manager.h"
#include <QSize>

// ======================================================================================
// Test Fixture (Production mode by default: touching floor ends game)
// ======================================================================================

class ArenaEdgeCollisionHandlerTest : public ::testing::Test
{
public:
    ArenaEdgeCollisionHandlerTest()
    {
        handler_ = std::make_unique<ArenaEdgeCollisionHandler>(config_, ballManager_);
    }

protected:
    static constexpr QSize arenaSize{100, 100};

    Ball ball_{0, 0, 10, 0, 0};
    BallManager ballManager_{ball_};
    GameConfig config_{}; // ENABLE_BALL_TOUCHING_FLOOR_ENDS_GAME = true
    std::unique_ptr<ArenaEdgeCollisionHandler> handler_;
};

// ======================================================================================
// PRODUCTION MODE TESTS  (touching floor always ends the game)
// ======================================================================================

// Ball touching bottom → clamp + GameLost
TEST_F(ArenaEdgeCollisionHandlerTest, BallExactlyTouchingBottom_IsClampedAndReturnsGameLost)
{
    ball_.setY(arenaSize.height() - ball_.getDiameter());

    const auto result = handler_->handleBallCollisionWithArenaBottomEdge(ball_, arenaSize);

    EXPECT_EQ(result, ArenaEdgeCollisionHandler::ArenaBottomEdgeCollisionResult::GameLost);
    EXPECT_DOUBLE_EQ(ball_.getY(), arenaSize.height() - ball_.getDiameter());
}

// Ball slightly below bottom → clamp + GameLost
TEST_F(ArenaEdgeCollisionHandlerTest, BallSlightlyOverlappingBottom_IsClampedAndReturnsGameLost)
{
    ball_.setY(arenaSize.height() - ball_.getDiameter() + 3.5);

    const auto result = handler_->handleBallCollisionWithArenaBottomEdge(ball_, arenaSize);

    EXPECT_EQ(result, ArenaEdgeCollisionHandler::ArenaBottomEdgeCollisionResult::GameLost);
    EXPECT_DOUBLE_EQ(ball_.getY(), arenaSize.height() - ball_.getDiameter());
}

// Ball far beyond bottom → clamp + GameLost
TEST_F(ArenaEdgeCollisionHandlerTest, BallDeeplyOverlappingBottom_IsClampedAndReturnsGameLost)
{
    ball_.setY(arenaSize.height() + 100.0);

    const auto result = handler_->handleBallCollisionWithArenaBottomEdge(ball_, arenaSize);

    EXPECT_EQ(result, ArenaEdgeCollisionHandler::ArenaBottomEdgeCollisionResult::GameLost);
    EXPECT_DOUBLE_EQ(ball_.getY(), arenaSize.height() - ball_.getDiameter());
}

// Already clamped → stays clamped + GameLost
TEST_F(ArenaEdgeCollisionHandlerTest, CallingHandlerWhenAlreadyClampedDoesNotMoveBallAndReturnsGameLost)
{
    ball_.setY(arenaSize.height() - ball_.getDiameter());

    const auto result = handler_->handleBallCollisionWithArenaBottomEdge(ball_, arenaSize);

    EXPECT_EQ(result, ArenaEdgeCollisionHandler::ArenaBottomEdgeCollisionResult::GameLost);
    EXPECT_DOUBLE_EQ(ball_.getY(), arenaSize.height() - ball_.getDiameter());
}

// Small arena → still clamp + GameLost
TEST_F(ArenaEdgeCollisionHandlerTest, TinyArenaStillClampAndReturnGameLost)
{
    constexpr QSize tinyArena{5, 5};
    ball_.setY(0);

    const auto result = handler_->handleBallCollisionWithArenaBottomEdge(ball_, tinyArena);

    EXPECT_EQ(result, ArenaEdgeCollisionHandler::ArenaBottomEdgeCollisionResult::GameLost);
    EXPECT_DOUBLE_EQ(ball_.getY(), tinyArena.height() - ball_.getDiameter());
}


// ======================================================================================
// BOUNCE MODE TESTS (These behave DIFFERENTLY: touching floor does NOT end game)
// ======================================================================================

// In bounce mode: overlapping → clamp + bounce + returns Bounce
TEST_F(ArenaEdgeCollisionHandlerTest, BallOverlappingBottom_ClampedThenBounced_ReturnsBounce)
{
    config_.setBallTouchingFloorEndingGameEnabled(false);

    // recreate the handler so it picks up the updated config
    handler_ = std::make_unique<ArenaEdgeCollisionHandler>(config_, ballManager_);

    ball_.setY(arenaSize.height() - ball_.getDiameter() + 5);

    const auto result = handler_->handleBallCollisionWithArenaBottomEdge(ball_, arenaSize);

    EXPECT_EQ(result, ArenaEdgeCollisionHandler::ArenaBottomEdgeCollisionResult::BallBouncedOffArenaBottomEdge);
    EXPECT_DOUBLE_EQ(ball_.getY(), arenaSize.height() - ball_.getDiameter());
    EXPECT_LE(ball_.getVelocityY(), 0.0); // bounce: velocity must be upward (≤ 0)
}

// In bounce mode: already clamped → stay clamped + still bounce
TEST_F(ArenaEdgeCollisionHandlerTest, AlreadyClampedBall_StillBouncesAndReturnsBounce)
{
    config_.setBallTouchingFloorEndingGameEnabled(false);

    // recreate the handler so it picks up the updated config
    handler_ = std::make_unique<ArenaEdgeCollisionHandler>(config_, ballManager_);

    ball_.setY(arenaSize.height() - ball_.getDiameter());

    const auto result = handler_->handleBallCollisionWithArenaBottomEdge(ball_, arenaSize);

    EXPECT_EQ(result, ArenaEdgeCollisionHandler::ArenaBottomEdgeCollisionResult::BallBouncedOffArenaBottomEdge);
    EXPECT_DOUBLE_EQ(ball_.getY(), arenaSize.height() - ball_.getDiameter());
    EXPECT_LE(ball_.getVelocityY(), 0.0);
}
