#pragma once

#include "state_machine/abstract_state.h"

class ReadyToStartState : public AbstractState
{
public:
    ReadyToStartState() : AbstractState("ReadyToStartState", GameStateType::ReadyToStart)
    {}

    void transitionTo(AbstractState* newState) override
    {
        switch (newState->type())
        {
            case GameStateType::Running:
                stateActions_->launchBall();
                stateActions_->startGameLoop();
                stateActions_->enablePaddleMovement();
                stateActions_->hideTextOverlay();
                break;

            case GameStateType::EscapeMenu:
                stateActions_->disablePaddleMovement();
                stateActions_->showEscapeMenuOverlay();
                break;

            default:
                handleIllegalTransition(newState);
        }

        printTransition(newState);
    }

    void onMouseMove(const qreal mouseX, const qreal gameAreaWidth) override
    {
        stateActions_->setPaddlePosition(mouseX, gameAreaWidth);
        /* Ball follows paddle */
        stateActions_->setBallPositionOnPaddle();
    }

    void onMouseClick(const Qt::MouseButton button) override
    {
        if (button == Qt::LeftButton)
        {
            qDebug() << "Left mouse button accepted";

            // Player clicked to start the game
            stateActions_->setGameState(GameStateType::Running);
        }
    }

    void onKeyPress(const int key) override
    {
        switch (key)
        {
            case Qt::Key_Escape:
                stateActions_->setGameState(GameStateType::EscapeMenu);
                break;

            default:
                break;
        }
    }
};
