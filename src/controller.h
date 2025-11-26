#pragma once

#include "model.h"
#include "game_loop.h"
#include "qml_helper.h"
#include "window_manager.h"
#include "overlay_manager.h"
#include "state_machine/game_state_manager.h"
#include "input_handler.h"
#include "game_coordinator.h"
#include "state_machine/i_state_actions.h"

class Controller final : public QObject, public IStateActions
{
    Q_OBJECT

signals:
    void paddleChanged();
    void ballChanged();
    void bricksModelChanged();
    void applicationShutdownRequested();

public slots:
    void onQmlEngineFullyInitialized();
    void processMouseMove(qreal mouseX, qreal gameAreaWidth);
    void processMouseClick(Qt::MouseButton button);
    void processKeyPress(int key);
    void onResumeClicked();
    void onRestartClicked();
    void onQuitClicked();
    void onPlayAgainClicked();

public:
    explicit Controller(const GameConfig& gameConfig, Model& model, QQmlApplicationEngine& view);

    // IStateActions implementation
    void startGameLoop() override;
    void stopGameLoop() override;

    void enablePaddleMovement() override;
    void disablePaddleMovement() override;
    void setPaddlePosition(qreal mouseX, qreal gameAreaWidth) override;

    void launchBall() override;
    void setBallPositionOnPaddle() override;

    void showTextOverlay(const QString& text) override;
    void hideTextOverlay() override;
    void showEscapeMenuOverlay() override;
    void hideEscapeMenuOverlay() override;
    void showEndGameOverlay(GameResult result) override;
    void hideEndGameOverlay() override;

    void setGameState(GameStateType newGameState) override;
    void restorePreviousState() override;

    [[nodiscard]] GameStateType getGameStateType() const { return gameStateManager_.getCurrentGameStateType(); }
    [[nodiscard]] GameStateManager& getStateManager() { return gameStateManager_; }

    // Paddle properties
    [[nodiscard]] qreal getPaddleX() const;
    Q_PROPERTY(qreal paddleX READ getPaddleX NOTIFY paddleChanged)

    [[nodiscard]] qreal getPaddleY() const;
    Q_PROPERTY(qreal paddleY READ getPaddleY CONSTANT)

    [[nodiscard]] qreal getPaddleWidth() const;
    Q_PROPERTY(qreal paddleWidth READ getPaddleWidth CONSTANT)

    [[nodiscard]] qreal getPaddleHeight() const;
    Q_PROPERTY(qreal paddleHeight READ getPaddleHeight CONSTANT)

    // Ball properties
    [[nodiscard]] qreal getBallX() const;
    Q_PROPERTY(qreal ballX READ getBallX NOTIFY ballChanged)

    [[nodiscard]] qreal getBallY() const;
    Q_PROPERTY(qreal ballY READ getBallY NOTIFY ballChanged)

    [[nodiscard]] qreal getBallDiameter() const;
    Q_PROPERTY(qreal ballDiameter READ getBallDiameter CONSTANT)

    // Other properties
    [[nodiscard]] QSize getGameArenaSize() const { return gameArenaSize_; }
    Q_PROPERTY(QSize gameArenaSize READ getGameArenaSize CONSTANT)

    [[nodiscard]] bool isStartInFullScreenEnabled() const { return windowManager_.isStartInFullScreenEnabled(); }
    Q_PROPERTY(bool startInFullScreenEnabled READ isStartInFullScreenEnabled CONSTANT)

    [[nodiscard]] QSize getInitialRenderResolution() const { return windowManager_.getInitialRenderResolution(); }
    Q_PROPERTY(QSize initialRenderResolution READ getInitialRenderResolution CONSTANT)

    QObject* getBricksModel() { return &model_.getBricksModel(); }
    Q_PROPERTY(QObject* bricksModel READ getBricksModel CONSTANT)

private slots:
    void gameEnded(GameResult gameResult);

private:
    const QSize& gameArenaSize_;
    Model& model_;
    QQmlApplicationEngine& view_;
    std::unique_ptr<GameLoop> gameLoop_;

    QmlHelper qmlHelper_{view_};
    GameStateManager gameStateManager_;
    WindowManager windowManager_;
    std::unique_ptr<OverlayManager> overlayManager_;
    std::unique_ptr<InputHandler> inputHandler_;
    std::unique_ptr<GameCoordinator> gameCoordinator_;
};
