#include "input_handler.h"
#include "controller.h"
#include "window_manager.h"

InputHandler::InputHandler(Controller* controller, WindowManager* windowManager) : controller_(controller), windowManager_(windowManager)
{}

void InputHandler::processMouseMove(const qreal mouseX, const qreal gameAreaWidth)
{
    controller_->getStateManager().handleMouseMove(mouseX, gameAreaWidth);
}

void InputHandler::processMouseClick(const Qt::MouseButton button)
{
    if (button == Qt::LeftButton)
    {
        qDebug() << "Left mouse button clicked";
    }
    else if (button == Qt::RightButton)
    {
        qDebug() << "Right mouse button clicked";
    }

    controller_->getStateManager().handleMouseClick(button);
}

void InputHandler::processKeyPress(const int key)
{
    constexpr bool logKeyPressed = false;

    switch (key)
    {
        case Qt::Key_Escape:
            if (logKeyPressed) qDebug() << "Escape key pressed";
            controller_->getStateManager().handleKeyPress(key);
            break;

        // Those are for debugging purposes only
        // case Qt::Key_F1:
        //     if (logKeyPressed) qDebug() << "F1 key pressed";
        //     windowManager_->setResolutionHD();
        //     break;
        //
        // case Qt::Key_F2:
        //     if (logKeyPressed) qDebug() << "F2 key pressed";
        //     windowManager_->setResolutionFullHD();
        //     break;
        //
        // case Qt::Key_F3:
        //     if (logKeyPressed) qDebug() << "F3 key pressed";
        //     windowManager_->setResolutionQHD();
        //     break;
        //
        // case Qt::Key_F4:
        //     if (logKeyPressed) qDebug() << "F4 key pressed";
        //     windowManager_->setResolutionUHD();
        //     break;
        //
        // case Qt::Key_F11:
        //     if (logKeyPressed) qDebug() << "F11 key pressed";
        //     windowManager_->toggleFullScreen();
        //     break;

        // case Qt::Key_P:
        //     if (logKeyPressed) qDebug() << "P key pressed";
        //     controller_->getStateManager().handleKeyPress(key);
        //     break;

        default:
            if (logKeyPressed) qDebug() << "Unsupported key pressed";
            break;
    }
}
