#pragma once

#include <QtTypes>

class Paddle
{
public:
    Paddle(const qreal x, const qreal y, const qreal width, const qreal height) : x_(x), y_(y), width_(width), height_(height), initialX_(x), initialY_(y)
    {
        // qDebug() << "Paddle: X" << x_ << "Y" << y_ << "Width" << width_ << "Height" << height_;
    }

    // Position
    [[nodiscard]] qreal getX() const
    {
        return x_;
    }

    [[nodiscard]] qreal getY() const
    {
        return y_;
    }

    void setX(qreal x)
    {
        x_ = x;
    }

    void setY(qreal y)
    {
        y_ = y;
    }

    [[nodiscard]] qreal getWidth() const
    {
        return width_;
    }

    [[nodiscard]] qreal getHeight() const
    {
        return height_;
    }

    // Reset to the initial position
    void reset()
    {
        x_ = initialX_;
        y_ = initialY_;
    }

private:
    qreal x_;
    qreal y_;
    const qreal width_;
    const qreal height_;
    const qreal initialX_;
    const qreal initialY_;
};
