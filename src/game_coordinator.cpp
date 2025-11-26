#include "game_coordinator.h"

GameCoordinator::GameCoordinator(const GameConfig& gameConfig, Model& model, GameLoop& gameLoop)
    : gameArenaSize_(gameConfig.getGameArenaSize()), model_(model), gameLoop_(gameLoop)
{}

void GameCoordinator::setQmlHelper(QmlHelper* qmlHelper)
{
    qmlHelper_ = qmlHelper;
}

void GameCoordinator::setPaddlePosition(const qreal mouseX, const qreal gameAreaWidth)
{
    // Convert mouseX render coordinates to logical coordinates
    const qreal logicalX = mouseX * gameArenaSize_.width() / gameAreaWidth;

    // Center the paddle around the cursor
    model_.getPaddle().setX(logicalX - model_.getPaddle().getWidth() / 2.0);

    // Clamp the paddle to logical game boundaries
    if (getPaddleX() < 0)
    {
        model_.getPaddle().setX(0);
    }
    else if (getPaddleX() + getPaddleWidth() > gameArenaSize_.width())
    {
        model_.getPaddle().setX(gameArenaSize_.width() - getPaddleWidth());
    }
}

void GameCoordinator::setBallPositionOnPaddle()
{
    model_.getBall().setX(getPaddleX() + getPaddleWidth() / 2.0 - getBallDiameter() / 2.0);
    /* Ball Y position stays unchanged */
}

void GameCoordinator::launchBall()
{
    gameLoop_.launchBall();
}

void GameCoordinator::enablePaddleMovement()
{
    if (!qmlHelper_)
    {
        qWarning() << "QmlHelper not initialized yet, cannot enable paddle movement";
        return;
    }

    auto mouseArea = qmlHelper_->getMouseArea(QmlMouseArea::MainWindowMouseArea);
    mouseArea->setProperty("enabled", true);
}

void GameCoordinator::disablePaddleMovement()
{
    if (!qmlHelper_)
    {
        qWarning() << "QmlHelper not initialized yet, cannot disable paddle movement";
        return;
    }

    auto mouseArea = qmlHelper_->getMouseArea(QmlMouseArea::MainWindowMouseArea);
    mouseArea->setProperty("enabled", false);
}

void GameCoordinator::startGameLoop()
{
    gameLoop_.start();
}

void GameCoordinator::stopGameLoop()
{
    gameLoop_.stop();
}

void GameCoordinator::restartGame()
{
    qInfo() << "Restarting game";

    if (qmlHelper_)
    {
        // Restore keyboard focus to GameInput
        QMetaObject::invokeMethod(qmlHelper_->getGameInput(), "refocus");
    }

    model_.reset();
}
