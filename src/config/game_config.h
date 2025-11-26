#pragma once

#include "resolution.h"

class GameConfig
{
public:
    GameConfig();

    struct PaddleConfig
    {
        int WIDTH{};
        int HEIGHT{};
        int INITIAL_POSITION_X{};
        int INITIAL_POSITION_Y{};
    };

    struct BallConfig
    {
        int DIAMETER{};
        int INITIAL_POSITION_X{};
        int INITIAL_POSITION_Y{};
        double VELOCITY_RELATIVE_TO_SCREEN_SIZE{};
        int INITIAL_VELOCITY_X{};
        int INITIAL_VELOCITY_Y{};
    };

    [[nodiscard]] const QSize& getGameArenaSize() const { return gameArenaSize_; }
    [[nodiscard]] const PaddleConfig& getPaddleConfig() const { return paddle_; }
    [[nodiscard]] const BallConfig& getBallConfig() const { return ball_; }

    void setBallTouchingFloorEndingGameEnabled(const bool enable) { enableBallTouchingFloorEndsGame_ = enable; }
    [[nodiscard]] bool isBallTouchingFloorEndingGameEnabled() const { return enableBallTouchingFloorEndsGame_; }

private:
    // Fixed logical size for physics, collisions, etc.
    const QSize gameArenaSize_;

    PaddleConfig paddle_;
    BallConfig ball_;

    bool enableBallTouchingFloorEndsGame_ = true;
};
