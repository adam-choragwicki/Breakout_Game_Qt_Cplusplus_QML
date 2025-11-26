#include "game.h"
#include "log_manager.h"

int main(int argc, char* argv[])
{
    try
    {
#if defined(QT_DEBUG)
        LogManager::initialize(LogManager::Mode::LogToFileAndConsole, LogManager::Verbosity::Debug);
#else
        LogManager::initialize(LogManager::Mode::LogToFileOnly, LogManager::Verbosity::Info);
#endif

        QGuiApplication app(argc, argv);

        Game game;

        return QGuiApplication::exec();
    }
    catch (const std::exception& e)
    {
        qCritical() << "Unhandled exception:" << e.what();
        return 1;
    }
    catch (...)
    {
        qCritical() << "Unhandled unknown exception";
        return 1;
    }
}

//TODO FEATURE Add custom cursor visible only inside game window?
//TODO FEATURE Add more sound effects
//TODO FEATURE Add score display

//TODO FEATURE OPTIONAL Collectible bonuses and debuffs (faster ball etc.)
//TODO FEATURE OPTIONAL Add additional graphic effects when breaking brick
//TODO FEATURE OPTIONAL Add music

//TODO OPTIMIZATION OPTIONAL For maximum correctness and no wasted CPU: connect update() to QQuickWindow::beforeRendering.

//TODO BUGFIX Fix bug when ball occasionally hangs during collision handling
