#pragma once

#include "state_machine/abstract_state.h"

class InitializationState : public AbstractState
{
public:
    InitializationState() : AbstractState("InitializationState", GameStateType::Initialization)
    {}

    void transitionTo(AbstractState* newState) override
    {
        switch (newState->type())
        {
            case GameStateType::ReadyToStart:
                stateActions_->enablePaddleMovement();
                stateActions_->showTextOverlay("PRESS LMB TO START");
                break;

            default:
                handleIllegalTransition(newState);
        }

        printTransition(newState);
    }
};
