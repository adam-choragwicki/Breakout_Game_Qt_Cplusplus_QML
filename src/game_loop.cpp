#include "game_loop.h"
#include "collision_detection/arena_edge_collision_detector.h"
#include "collision_detection/brick_collision_detector.h"
#include "collision_detection/paddle_collision_detector.h"
#include <QDateTime>

GameLoop::GameLoop(const GameConfig& gameConfig, Model& model, QObject* parent) : QObject(parent), gameArenaSize_(gameConfig.getGameArenaSize()), model_(model)
{
    timer_ = std::make_unique<QTimer>();
    timer_->setTimerType(Qt::PreciseTimer);
    timer_->setInterval(0);
    connect(timer_.get(), &QTimer::timeout, this, &GameLoop::execute);

    ballManager_ = std::make_unique<BallManager>(model_.getBall());
    arenaEdgeCollisionHandler_ = std::make_unique<ArenaEdgeCollisionHandler>(gameConfig, *ballManager_);
    paddleCollisionHandler_ = std::make_unique<PaddleCollisionHandler>(*ballManager_);
    brickCollisionHandler_ = std::make_unique<BrickCollisionHandler>(*ballManager_);
}

void GameLoop::execute()
{
    // TODO Move what happens before actual game start (ReadyToStart state) also here

    const qint64 now = QDateTime::currentMSecsSinceEpoch();

    if (lastTimeMs_ == 0)
    {
        lastTimeMs_ = now;
        return; // skip the first frame to initialize timing
    }

    qreal dt = (now - lastTimeMs_) / 1000.0; // seconds
    lastTimeMs_ = now;

    if (dt > 0.05) // clamp large frames
    {
        dt = 0.05;
    }

    constexpr int substeps = 5;
    const qreal dtStep = dt / substeps;

    for (int step = 0; step < substeps; ++step)
    {
        Ball& ball = model_.getBall();
        Paddle& paddle = model_.getPaddle();

        /* Move ball */
        ballManager_->moveBall(dtStep);

        // qDebug() << "Ball speed: " << ball.getVelocityX() << ", " << ball.getVelocityY();
        // qDebug() << "dtStep: " << dtStep;

        if (const bool ballCollisionWithArenaBottomEdgeOccurred = ArenaEdgeCollisionDetector::checkBallCollisionWithArenaBottomEdge(ball, gameArenaSize_); ballCollisionWithArenaBottomEdgeOccurred)
        {
            using ArenaBottomEdgeCollisionResult = ArenaEdgeCollisionHandler::ArenaBottomEdgeCollisionResult;

            if (const auto ballCollisionWithArenaBottomEdgeHandlingResult = arenaEdgeCollisionHandler_->handleBallCollisionWithArenaBottomEdge(ball, gameArenaSize_);
                ballCollisionWithArenaBottomEdgeHandlingResult == ArenaBottomEdgeCollisionResult::GameLost)
            {
                emit endGame(GameResult::DEFEAT);
                return;
            }
        }

        checkAndHandleBallCollisionWithArenaTopEdge(ball);
        checkAndHandleBallCollisionWithArenaSideEdge(ball);
        checkAndHandleBallCollisionWithPaddle(ball, paddle);

        auto& bricks = model_.getBricksManager().getBricks();

        if (const auto collisionOccurred = BrickCollisionDetector::checkBallCollisionWithBricks(ball, bricks); collisionOccurred)
        {
            soundPlayer_.playBrickCollision();

            model_.getScoreManager().increaseScore();

            brickCollisionHandler_->handleBallCollisionWithBrick(ball, bricks, model_.getBricksModel(), collisionOccurred->hitSide, collisionOccurred->brickIndex);

            if (model_.getBricksManager().getAliveBricksCount() == 0)
            {
                emit endGame(GameResult::VICTORY);
                return;
            }
        }
    }

    emit ballUpdated();
}

void GameLoop::start()
{
    qDebug() << "Starting game loop";
    lastTimeMs_ = QDateTime::currentMSecsSinceEpoch();
    timer_->start();
}

void GameLoop::stop()
{
    qDebug() << "Stopping game loop";
    timer_->stop();
}

void GameLoop::launchBall()
{
    ballManager_->launchBall();
}

void GameLoop::checkAndHandleBallCollisionWithArenaTopEdge(Ball& ball)
{
    if (ArenaEdgeCollisionDetector::checkBallCollisionWithArenaTopEdge(ball))
    {
        // Bounce vertically (downward)
        ball.setY(0);
        ballManager_->bounceBallVertically();
    }
}

void GameLoop::checkAndHandleBallCollisionWithArenaSideEdge(Ball& ball)
{
    /* Handle either left or right hitSide-edge collision, it is impossible for the ball to collide with both at once */
    if (ArenaEdgeCollisionDetector::checkBallCollisionWithArenaLeftEdge(ball))
    {
        // Bounce horizontally (rightward)
        ball.setX(0);
        ballManager_->bounceBallHorizontally();
    }
    else if (const qreal rightEdgeX = gameArenaSize_.width() - ball.getDiameter(); ArenaEdgeCollisionDetector::checkBallCollisionWithArenaRightEdge(ball, rightEdgeX))
    {
        // Bounce horizontally (leftward)
        ball.setX(rightEdgeX);
        ballManager_->bounceBallHorizontally();
    }
}

void GameLoop::checkAndHandleBallCollisionWithPaddle(Ball& ball, const Paddle& paddle)
{
    // check if a collision occurred and if so, from which hitSide the ball hit the paddle
    if (const auto collisionOccurred = PaddleCollisionDetector::checkBallCollisionWithPaddle(ball, paddle); collisionOccurred)
    {
        // Play SFX as soon as a collision is confirmed
        soundPlayer_.playPaddleCollision();
        paddleCollisionHandler_->handleBallCollisionWithPaddle(ball, paddle, collisionOccurred->hitSide);
    }
}
