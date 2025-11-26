#pragma once

#include "game_result.h"
#include "game_state_type.h"

class IStateActions
{
public:
    virtual ~IStateActions() = default;

    // Game loop control
    virtual void startGameLoop() = 0;
    virtual void stopGameLoop() = 0;

    // Paddle
    virtual void enablePaddleMovement() = 0;
    virtual void disablePaddleMovement() = 0;
    virtual void setPaddlePosition(qreal mouseX, qreal gameAreaWidth) = 0;

    // Ball
    virtual void launchBall() = 0;
    virtual void setBallPositionOnPaddle() = 0;

    // Overlays
    virtual void showTextOverlay(const QString& text) = 0;
    virtual void hideTextOverlay() = 0;
    virtual void showEscapeMenuOverlay() = 0;
    virtual void hideEscapeMenuOverlay() = 0;
    virtual void showEndGameOverlay(GameResult result) = 0;
    virtual void hideEndGameOverlay() = 0;

    // State transitions
    virtual void setGameState(GameStateType newType) = 0;
    virtual void restorePreviousState() = 0;
};
