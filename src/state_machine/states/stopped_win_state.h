#pragma once

#include "state_machine/abstract_state.h"

class StoppedWinState : public AbstractState
{
public:
    StoppedWinState() : AbstractState("StoppedWinState", GameStateType::StoppedWin)
    {}

    void transitionTo(AbstractState* newState) override
    {
        switch (newState->type())
        {
            case GameStateType::ReadyToStart:
                stateActions_->enablePaddleMovement();
                stateActions_->hideEndGameOverlay();
                break;

            default:
                handleIllegalTransition(newState);
        }

        printTransition(newState);
    }
};
