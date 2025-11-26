#pragma once

#include <QColor>

class Brick
{
public:
    Brick(qreal x, qreal y, qreal width, qreal height, const QColor& color) : x_(x), y_(y), width_(width), height_(height), color_(color),
                                                                              alive_(true)
    {
        radius_ = (std::min(width_, height_) * 0.05);
    }

    qreal x() const
    {
        return x_;
    }

    qreal y() const
    {
        return y_;
    }

    qreal width() const
    {
        return width_;
    }

    qreal height() const
    {
        return height_;
    }

    qreal radius() const
    {
        return radius_;
    }

    QColor color() const
    {
        return color_;
    }

    bool isAlive() const
    {
        return alive_;
    }

    void destroy()
    {
        alive_ = false;
    }

    void setAlive(bool alive)
    {
        alive_ = alive;
    }

private:
    qreal x_;
    qreal y_;
    qreal width_;
    qreal height_;
    qreal radius_;
    QColor color_;
    bool alive_;
};
