#pragma once

#include "abstract_state.h"
#include "states/initialization_state.h"
#include "states/ready_to_start_state.h"
#include "states/running_state.h"
#include "states/escape_menu_state.h"
#include "states/stopped_win_state.h"
#include "states/stopped_lose_state.h"
#include "states/paused_for_debug_state.h"
#include <ranges>
#include <stdexcept>

class GameStateManager
{
public:
    explicit GameStateManager(IStateActions* stateActions) : states_{
        {GameStateType::Initialization, &initializationState_},
        {GameStateType::ReadyToStart, &readyToStartState_},
        {GameStateType::Running, &runningState_},
        {GameStateType::EscapeMenu, &escapeMenuState_},
        {GameStateType::StoppedWin, &stoppedWinState_},
        {GameStateType::StoppedLose, &stoppedLoseState_},
        {GameStateType::PausedForDebug, &pausedForDebugState_}
    }
    {
        for (auto& state: states_ | std::views::values)
        {
            // Assign a state actions pointer to each state
            state->setStateActions(stateActions);
        }

        currentGameState_ = &initializationState_;
    }

    void setGameState(const GameStateType newType)
    {
        AbstractState* newState = getStateByType(newType);
        previousState_ = currentGameState_; // store state transition
        currentGameState_->transitionTo(newState);
        currentGameState_ = newState;
    }

    [[nodiscard]] AbstractState* getCurrentGameState() const { return currentGameState_; }

    [[nodiscard]] GameStateType getCurrentGameStateType() const
    {
        return currentGameState_->type();
    }

    [[nodiscard]] std::optional<GameStateType> getPreviousGameStateType() const
    {
        if (!previousState_)
        {
            return std::nullopt;
        }

        return previousState_->type();
    }

    void restorePreviousGameState()
    {
        if (!previousState_)
        {
            throw std::runtime_error("No previous state to restore");
        }

        qDebug() << "Restoring previous state: " << previousState_->type();

        setGameState(previousState_->type());
        previousState_ = nullptr; // clear after use
    }

    void handleMouseMove(const qreal mouseX, const qreal gameAreaWidth)
    {
        currentGameState_->onMouseMove(mouseX, gameAreaWidth);
    }

    void handleMouseClick(const Qt::MouseButton button)
    {
        currentGameState_->onMouseClick(button);
    }

    void handleKeyPress(const int key)
    {
        currentGameState_->onKeyPress(key);
    }

private:
    AbstractState* getStateByType(GameStateType type) const
    {
        return states_.at(type);
    }

    InitializationState initializationState_;
    ReadyToStartState readyToStartState_;
    RunningState runningState_;
    EscapeMenuState escapeMenuState_;
    StoppedWinState stoppedWinState_;
    StoppedLoseState stoppedLoseState_;
    PausedForDebugState pausedForDebugState_;

    AbstractState* currentGameState_{};
    AbstractState* previousState_{};

    std::map<GameStateType, AbstractState*> states_;
};
