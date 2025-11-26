#pragma once

#include "state_machine/abstract_state.h"

class EscapeMenuState : public AbstractState
{
public:
    EscapeMenuState() : AbstractState("EscapeMenuState", GameStateType::EscapeMenu)
    {}

    void transitionTo(AbstractState* newState) override
    {
        switch (newState->type())
        {
            case GameStateType::ReadyToStart:
                stateActions_->stopGameLoop();
                stateActions_->enablePaddleMovement();
                stateActions_->hideEscapeMenuOverlay();
                break;

            case GameStateType::Running:
                stateActions_->startGameLoop();
                stateActions_->enablePaddleMovement();
                stateActions_->hideEscapeMenuOverlay();
                break;

            case GameStateType::PausedForDebug:
                stateActions_->stopGameLoop();
                stateActions_->disablePaddleMovement();
                stateActions_->hideEscapeMenuOverlay();
                break;

            default:
                handleIllegalTransition(newState);
        }

        printTransition(newState);
    }

    void onKeyPress(int key) override
    {
        switch (key)
        {
            case Qt::Key_Escape:
                stateActions_->restorePreviousState();
                break;

            default:
                break;
        }
    }
};
