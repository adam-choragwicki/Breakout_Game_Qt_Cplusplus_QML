#include "game_config.h"

GameConfig::GameConfig() : gameArenaSize_(Resolution::HD)
{
    using std::round;

    // ----------------------------
    // Paddle configuration (derived from arena size)
    // ----------------------------
    paddle_.WIDTH = static_cast<int>(round(0.10 * gameArenaSize_.width())); // 10% of arena width
    paddle_.HEIGHT = static_cast<int>(round(gameArenaSize_.height() / 72.0)); // ~1/72 of arena height
    paddle_.INITIAL_POSITION_X = static_cast<int>(round(gameArenaSize_.width() / 2.0 - paddle_.WIDTH / 2.0));
    paddle_.INITIAL_POSITION_Y = static_cast<int>(round(0.95 * gameArenaSize_.height())); // near bottom

    // ----------------------------
    // Ball configuration (derived from paddle and arena)
    // ----------------------------
    ball_.DIAMETER = 2 * paddle_.HEIGHT;
    ball_.INITIAL_POSITION_X = static_cast<int>(round(paddle_.INITIAL_POSITION_X + paddle_.WIDTH / 2.0 - ball_.DIAMETER / 2.0));
    ball_.INITIAL_POSITION_Y = static_cast<int>(round(paddle_.INITIAL_POSITION_Y - ball_.DIAMETER));

    ball_.VELOCITY_RELATIVE_TO_SCREEN_SIZE = 1.00;
    ball_.INITIAL_VELOCITY_X = static_cast<int>(round(gameArenaSize_.width() * ball_.VELOCITY_RELATIVE_TO_SCREEN_SIZE));
    ball_.INITIAL_VELOCITY_Y = static_cast<int>(round(gameArenaSize_.height() * ball_.VELOCITY_RELATIVE_TO_SCREEN_SIZE));
}
