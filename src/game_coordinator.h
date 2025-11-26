#pragma once

#include "model.h"
#include "game_loop.h"
#include "qml_helper.h"

class GameCoordinator
{
public:
    explicit GameCoordinator(const GameConfig& gameConfig, Model& model, GameLoop& gameLoop);

    void setQmlHelper(QmlHelper* qmlHelper);

    void setPaddlePosition(qreal mouseX, qreal gameAreaWidth);
    void setBallPositionOnPaddle();
    void launchBall();

    void enablePaddleMovement();
    void disablePaddleMovement();

    void startGameLoop();
    void stopGameLoop();

    void restartGame();

    [[nodiscard]] qreal getPaddleX() const { return model_.getPaddle().getX(); }
    [[nodiscard]] qreal getPaddleY() const { return model_.getPaddle().getY(); }
    [[nodiscard]] qreal getPaddleWidth() const { return model_.getPaddle().getWidth(); }
    [[nodiscard]] qreal getPaddleHeight() const { return model_.getPaddle().getHeight(); }

    [[nodiscard]] qreal getBallX() const { return model_.getBall().getX(); }
    [[nodiscard]] qreal getBallY() const { return model_.getBall().getY(); }
    [[nodiscard]] qreal getBallDiameter() const { return model_.getBall().getDiameter(); }

signals:
    void paddleChanged();
    void ballChanged();

private:
    const QSize& gameArenaSize_;
    Model& model_;
    GameLoop& gameLoop_;
    QmlHelper* qmlHelper_{nullptr};
};
