#pragma once

#include "model.h"
#include "game_result.h"
#include "sound_player.h"
#include "ball_manager.h"
#include "collision_handling/arena_edge_collision_handler.h"
#include "collision_handling/paddle_collision_handler.h"
#include "collision_handling/brick_collision_handler.h"
#include <QTimer>

class GameLoop : public QObject
{
    Q_OBJECT

signals:
    void ballUpdated();
    void brickUpdated(int index);
    void endGame(GameResult gameResult);

public:
    explicit GameLoop(const GameConfig& gameConfig, Model& model, QObject* parent = nullptr);

    void start();
    void stop();

    void launchBall();

private slots:
    void execute();

private:
    void checkAndHandleBallCollisionWithArenaTopEdge(Ball& ball);
    void checkAndHandleBallCollisionWithArenaSideEdge(Ball& ball);
    void checkAndHandleBallCollisionWithPaddle(Ball& ball, const Paddle& paddle);

    const QSize& gameArenaSize_;
    Model& model_;
    std::unique_ptr<QTimer> timer_;
    qint64 lastTimeMs_ = 0;
    SoundPlayer soundPlayer_;

    std::unique_ptr<BallManager> ballManager_;
    std::unique_ptr<ArenaEdgeCollisionHandler> arenaEdgeCollisionHandler_;
    std::unique_ptr<PaddleCollisionHandler> paddleCollisionHandler_;
    std::unique_ptr<BrickCollisionHandler> brickCollisionHandler_;
};
