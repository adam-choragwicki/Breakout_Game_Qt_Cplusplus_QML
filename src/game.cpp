#include "game.h"
#include "asset_management/asset_loader.h"
#include "log_manager.h"
#include <QQmlContext>
#include <QCoreApplication>

Game::Game()
{
    printAppInfo();

    gameConfig_ = std::make_unique<GameConfig>();

    loadGameAssets();

    model_ = std::make_unique<Model>(*gameConfig_);
    view_ = std::make_unique<QQmlApplicationEngine>();
    controller_ = std::make_unique<Controller>(*gameConfig_, *model_, *view_);

    initializeQmlEngine();

    connect(controller_.get(),
            &Controller::applicationShutdownRequested,
            this,
            []()
            {
                QMetaObject::invokeMethod(QCoreApplication::instance(),
                                          []()
                                          {
                                              QCoreApplication::quit();
                                          },
                                          Qt::QueuedConnection);
            });

    connect(QCoreApplication::instance(),
            &QCoreApplication::aboutToQuit,
            []()
            {
                // TODO Future cleanup (save configs, close files, etc.)
                qInfo() << "Exiting application";
            });
}

void Game::printAppInfo()
{
    qInfo() << "Application version:" << APP_VERSION;
    qInfo() << "Qt version:" << QT_VERSION_STR;
    qInfo() << "Build type:" << BUILD_TYPE;

#if defined(Q_OS_WIN)
    qInfo() << "OS: Windows";
#elif defined(Q_OS_LINUX)
    qInfo() << "OS: Linux";
#endif
}

void Game::loadGameAssets()
{
    if (AssetLoader::loadAssets())
    {
        qInfo() << "Game assets loaded successfully";
    }
    else
    {
        // Defensive programming
        qCritical() << "Failed to load game assets. Exiting.";
        exit(1);
    }
}

void Game::initializeQmlEngine()
{
    view_->rootContext()->setContextProperty("Controller", controller_.get());

    /*
     * Rationale for the below QML setup:
     * - QQmlApplicationEngine::load() is effectively asynchronous; rootObjects() may be empty immediately.
     * - We rely on objectCreated() as the reliable success/failure notification.
     * - On failure we queue QCoreApplication::exit(1) (via a queued invoke) so the exit code is honored even
     *   if this runs before the event loop starts. Calling exit/quit directly here can be ignored otherwise.
     * - Qt::SingleShotConnection ensures the handler runs exactly once (avoids double-calls on reloads).
     */
    connect(view_.get(),
            &QQmlApplicationEngine::objectCreated,
            controller_.get(),
            [this](QObject* obj, const QUrl& url)
            {
                if (!obj)
                {
                    qCritical() << "QML root creation failed for" << url;

                    QMetaObject::invokeMethod(QCoreApplication::instance(),
                                              []()
                                              {
                                                  QCoreApplication::exit(1);
                                              },
                                              Qt::QueuedConnection);

                    return;
                }

                controller_->onQmlEngineFullyInitialized();
            },
            Qt::SingleShotConnection);

    view_->loadFromModule("BreakoutQml", "Main");
}
