#pragma once

#include "state_machine/abstract_state.h"

class RunningState : public AbstractState
{
public:
    RunningState() : AbstractState("RunningState", GameStateType::Running)
    {}

    void transitionTo(AbstractState* newState) override
    {
        switch (newState->type())
        {
            case GameStateType::EscapeMenu:
                stateActions_->stopGameLoop();
                stateActions_->disablePaddleMovement();
                stateActions_->showEscapeMenuOverlay();
                break;

            case GameStateType::PausedForDebug:
                stateActions_->stopGameLoop();
                stateActions_->disablePaddleMovement();
                break;

            case GameStateType::StoppedWin:
                stateActions_->stopGameLoop();
                stateActions_->disablePaddleMovement();
                stateActions_->showEndGameOverlay(GameResult::VICTORY);
                break;

            case GameStateType::StoppedLose:
                stateActions_->stopGameLoop();
                stateActions_->disablePaddleMovement();
                stateActions_->showEndGameOverlay(GameResult::DEFEAT);
                break;

            default:
                handleIllegalTransition(newState);
        }

        printTransition(newState);
    }

    void onMouseMove(const qreal mouseX, const qreal gameAreaWidth) override
    {
        stateActions_->setPaddlePosition(mouseX, gameAreaWidth);
    }

    void onKeyPress(const int key) override
    {
        switch (key)
        {
            case Qt::Key_P:
                stateActions_->setGameState(GameStateType::PausedForDebug);
                break;

            case Qt::Key_Escape:
                stateActions_->setGameState(GameStateType::EscapeMenu);
                break;

            default:
                break;
        }
    }
};
