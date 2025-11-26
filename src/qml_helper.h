#pragma once

#include <QQmlApplicationEngine>
#include <QQuickWindow>

enum class QmlOverlay
{
    TextOverlay,
    EndGameOverlay,
    EscapeMenuOverlay,
};

enum class QmlMouseArea
{
    MainWindowMouseArea
};

inline QString toString(QmlOverlay overlay)
{
    switch (overlay)
    {
        case QmlOverlay::TextOverlay: return "textOverlay";
        case QmlOverlay::EndGameOverlay: return "endGameOverlay";
        case QmlOverlay::EscapeMenuOverlay: return "escapeMenuOverlay";
    }
    Q_UNREACHABLE();
}

inline QString toString(QmlMouseArea mouseArea)
{
    switch (mouseArea)
    {
        case QmlMouseArea::MainWindowMouseArea: return "mainWindowMouseArea";
    }
    Q_UNREACHABLE();
}

class QmlHelper
{
public:
    explicit QmlHelper(QQmlApplicationEngine& view) : view_(view)
    {
    }

    QObject* getOverlay(QmlOverlay overlayEnum) const
    {
        QObject* root = getRootObject();

        auto overlay = root->findChild <QObject*>(toString(overlayEnum));
        if (!overlay)
        {
            const QString msg = QString("Overlay '%1' not found").arg(toString(overlayEnum));
            qCritical() << msg;
            throw std::runtime_error(msg.toStdString());
        }

        return overlay;
    }

    QObject* getMouseArea(QmlMouseArea mouseAreaEnum) const
    {
        QObject* root = getRootObject();

        auto mouseArea = root->findChild <QObject*>(toString(mouseAreaEnum));
        if (!mouseArea)
        {
            const QString msg = QString("MouseArea '%1' not found").arg(toString(mouseAreaEnum));
            qCritical() << msg;
            throw std::runtime_error(msg.toStdString());
        }

        return mouseArea;
    }

    QQuickWindow* getMainWindow() const
    {
        QObject* root = getRootObject();

        auto window = qobject_cast <QQuickWindow*>(root);

        if (!window)
        {
            qCritical() << "Root object is not a QQuickWindow";
            throw std::runtime_error("Root object is not a QQuickWindow");
        }

        return window;
    }

    QObject* getGameInput() const
    {
        QObject* root = getRootObject();

        auto gameInput = root->findChild<QObject*>("gameInput");

        if (!gameInput)
        {
            throw std::runtime_error("gameInput not found");
        }

        return gameInput;
    }

private:
    QQmlApplicationEngine& view_;

    QObject* getRootObject() const
    {
        if (view_.rootObjects().isEmpty())
        {
            qCritical() << "Root object not found";
            throw std::runtime_error("Root object not found");
        }

        QObject* root = view_.rootObjects().first();
        if (!root)
        {
            qCritical() << "Root object is nullptr";
            throw std::runtime_error("Root object is nullptr");
        }

        return root;
    }
};
