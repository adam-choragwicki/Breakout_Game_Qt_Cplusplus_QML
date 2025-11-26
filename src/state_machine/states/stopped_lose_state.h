#pragma once

#include "state_machine/abstract_state.h"

class StoppedLoseState : public AbstractState
{
public:
    StoppedLoseState() : AbstractState("StoppedLoseState", GameStateType::StoppedLose)
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
