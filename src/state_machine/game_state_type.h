#pragma once

#include <QDebug>

enum class GameStateType
{
    Initialization, // App just started — loading assets, creating bricks/paddle/ball.
    ReadyToStart, // Everything ready, waiting for the player's input to start. The text overlay is displayed.
    Running, // Main game loop active.
    EscapeMenu, // Escape menu overlay is displayed.
    StoppedWin, // Game loop stopped because all bricks were destroyed. The end game overlay is displayed.
    StoppedLose, // Game loop stopped because ball fell below paddle. The end game overlay is displayed.

    PausedForDebug, // Game loop stopped; only for debug.
};

inline QDebug operator<<(QDebug debug, GameStateType state)
{
    QDebugStateSaver saver(debug);
    debug.nospace();

    switch (state)
    {
        case GameStateType::Initialization: return debug << "Initialization";
        case GameStateType::ReadyToStart: return debug << "ReadyToStart";
        case GameStateType::Running: return debug << "Running";
        case GameStateType::EscapeMenu: return debug << "EscapeMenu";
        case GameStateType::StoppedWin: return debug << "StoppedWin";
        case GameStateType::StoppedLose: return debug << "StoppedLose";
        case GameStateType::PausedForDebug: return debug << "Paused (debug)";

        default:
            throw std::runtime_error("Unknown game state");
    }
}

// | From             | Action/Event                | To               | Notes                           |
// | :--------------- | :-------------------------- | :--------------- | :------------------------------ |
// | **Initialization** | load complete               | **ReadyToStart** | Start screen ready.             |
// | **ReadyToStart** | Player presses Start        | **Running**      | Begin game loop.                |
// | **Running**      | Player presses *P*          | **Paused**       | Pause toggle.                   |
// | **Paused**       | Player presses *P* again    | **Running**      | Resume.                         |
// | **Running**      | Player presses *Esc*        | **EscapeMenu**   | Open menu.                      |
// | **EscapeMenu**   | Select “RESUME”             | **Running**      | Return to play.                 |
// | **EscapeMenu**   | Select “QUIT”               | **Exiting**      | Exit to desktop (or main menu). |
// | **Running**      | All getBricks destroyed        | **StoppedWin**   | Show win overlay.               |
// | **Running**      | Ball lost                   | **StoppedLose**  | Show lose overlay.              |
// | **StoppedWin**   | Player presses “PLAY AGAIN” | **Restarting**   |                                 |
// | **StoppedLose**  | Player presses “PLAY AGAIN” | **Restarting**   |                                 |
// | **Restarting**   | Reset complete              | **ReadyToStart** | Clean restart.                  |
// | **Any**          | Application closed          | **Exiting**      | Safety cleanup.                 |
