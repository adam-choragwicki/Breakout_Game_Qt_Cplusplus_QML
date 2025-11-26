#include "controller.h"

Controller::Controller(const GameConfig& gameConfig, Model& model, QQmlApplicationEngine& view) : gameArenaSize_(gameConfig.getGameArenaSize()), model_(model), view_(view),
                                                                                                  gameStateManager_(this)
{
    gameLoop_ = std::make_unique<GameLoop>(gameConfig, model_);

    // Initialize these early so Q_PROPERTY getters work when QML loads
    gameCoordinator_ = std::make_unique<GameCoordinator>(gameConfig, model_, *gameLoop_);
    inputHandler_ = std::make_unique<InputHandler>(this, &windowManager_);

    connect(gameLoop_.get(), &GameLoop::ballUpdated, this, &Controller::ballChanged);
    connect(gameLoop_.get(), &GameLoop::endGame, this, &Controller::gameEnded);
}

void Controller::onQmlEngineFullyInitialized()
{
    qDebug() << "QML engine fully initialized";

    windowManager_.setWindow(qmlHelper_.getMainWindow());
    overlayManager_ = std::make_unique<OverlayManager>(qmlHelper_);

    // Now that QML is loaded, provide QmlHelper to GameCoordinator
    gameCoordinator_->setQmlHelper(&qmlHelper_);

    qInfo() << "Game started. Resolution =" << windowManager_.getInitialRenderResolution().width() << "x" << windowManager_.getInitialRenderResolution().height();

    setGameState(GameStateType::ReadyToStart);
}

void Controller::setGameState(const GameStateType newGameState)
{
    gameStateManager_.setGameState(newGameState);
}

void Controller::restorePreviousState()
{
    gameStateManager_.restorePreviousGameState();
}

void Controller::processMouseMove(const qreal mouseX, const qreal gameAreaWidth)
{
    inputHandler_->processMouseMove(mouseX, gameAreaWidth);
}

void Controller::processMouseClick(const Qt::MouseButton button)
{
    inputHandler_->processMouseClick(button);
}

void Controller::processKeyPress(const int key)
{
    inputHandler_->processKeyPress(key);
}

void Controller::startGameLoop()
{
    // qDebug() << "Controller::startGameLoop()";
    gameCoordinator_->startGameLoop();
}

void Controller::stopGameLoop()
{
    // qDebug() << "Controller::stopGameLoop()";
    gameCoordinator_->stopGameLoop();
}

void Controller::setPaddlePosition(const qreal mouseX, const qreal gameAreaWidth)
{
    gameCoordinator_->setPaddlePosition(mouseX, gameAreaWidth);
    emit paddleChanged();
}

void Controller::setBallPositionOnPaddle()
{
    gameCoordinator_->setBallPositionOnPaddle();
    emit ballChanged();
}

void Controller::launchBall()
{
    gameCoordinator_->launchBall();
}

void Controller::enablePaddleMovement()
{
    gameCoordinator_->enablePaddleMovement();
}

void Controller::disablePaddleMovement()
{
    gameCoordinator_->disablePaddleMovement();
}

void Controller::showTextOverlay(const QString& text)
{
    overlayManager_->showTextOverlay(text);
}

void Controller::hideTextOverlay()
{
    overlayManager_->hideTextOverlay();
}

void Controller::showEscapeMenuOverlay()
{
    overlayManager_->showEscapeMenuOverlay();
}

void Controller::hideEscapeMenuOverlay()
{
    overlayManager_->hideEscapeMenuOverlay();
}

void Controller::showEndGameOverlay(const GameResult gameResult)
{
    overlayManager_->showEndGameOverlay(gameResult, model_.getScoreManager().getScoreAsString());
}

void Controller::hideEndGameOverlay()
{
    overlayManager_->hideEndGameOverlay();
}

qreal Controller::getPaddleX() const
{
    return gameCoordinator_->getPaddleX();
}

qreal Controller::getPaddleY() const
{
    return gameCoordinator_->getPaddleY();
}

qreal Controller::getPaddleWidth() const
{
    return gameCoordinator_->getPaddleWidth();
}

qreal Controller::getPaddleHeight() const
{
    return gameCoordinator_->getPaddleHeight();
}

qreal Controller::getBallX() const
{
    return gameCoordinator_->getBallX();
}

qreal Controller::getBallY() const
{
    return gameCoordinator_->getBallY();
}

qreal Controller::getBallDiameter() const
{
    return gameCoordinator_->getBallDiameter();
}

void Controller::gameEnded(const GameResult gameResult)
{
    qDebug() << "Controller received endGame signal";

    if (gameResult == GameResult::VICTORY)
    {
        qInfo() << "Game ended. Result = victory";
        setGameState(GameStateType::StoppedWin);
    }
    else if (gameResult == GameResult::DEFEAT)
    {
        qInfo() << "Game ended. Result = defeat";
        setGameState(GameStateType::StoppedLose);
    }
    else
    {
        throw std::runtime_error("Unsupported game result");
    }
}

void Controller::onResumeClicked()
{
    qInfo() << "Resume button clicked";
    gameStateManager_.restorePreviousGameState();
}

void Controller::onRestartClicked()
{
    qInfo() << "Restart button clicked";
    gameCoordinator_->restartGame();
    gameStateManager_.setGameState(GameStateType::ReadyToStart);
}

void Controller::onQuitClicked()
{
    qInfo() << "Quit button clicked";
    emit applicationShutdownRequested();
}

void Controller::onPlayAgainClicked()
{
    gameCoordinator_->restartGame();
    gameStateManager_.setGameState(GameStateType::ReadyToStart);
}
