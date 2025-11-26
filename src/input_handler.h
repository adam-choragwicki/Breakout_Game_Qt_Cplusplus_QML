#pragma once

#include <QObject>
#include <Qt>

class Controller;
class WindowManager;

class InputHandler : public QObject
{
    Q_OBJECT

public:
    explicit InputHandler(Controller* controller, WindowManager* windowManager);

    void processMouseMove(qreal mouseX, qreal gameAreaWidth);
    void processMouseClick(Qt::MouseButton button);
    void processKeyPress(int key);

private:
    Controller* controller_{};
    WindowManager* windowManager_{};
};
