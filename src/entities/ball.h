#pragma once

#include <QtTypes>
#include <QDebug>

class Ball
{
public:
    Ball(const qreal x, const qreal y, const qreal diameter, const qreal velocityX, const qreal velocityY) : x_(x), y_(y), velocityX_(velocityX), velocityY_(velocityY), diameter_(diameter),
                                                                                                             initialX_(x), initialY_(y), initialVelocityX_(velocityX), initialVelocityY_(velocityY)
    {
        // qDebug() << "Paddle: X" << x_ << "Y" << y_ << "Diameter" << diameter;
    }

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

    [[nodiscard]] qreal getDiameter() const
    {
        return diameter_;
    }

    [[nodiscard]] qreal getVelocityX() const
    {
        return velocityX_;
    }

    [[nodiscard]] qreal getVelocityY() const
    {
        return velocityY_;
    }

    void setVelocityX(qreal velocityX)
    {
        velocityX_ = velocityX;
    }

    void setVelocityY(qreal velocityY)
    {
        velocityY_ = velocityY;
    }

    // Reset to initial position and velocity
    void reset()
    {
        x_ = initialX_;
        y_ = initialY_;
        velocityX_ = initialVelocityX_;
        velocityY_ = initialVelocityY_;
    }

private:
    qreal x_;
    qreal y_;
    qreal velocityX_;
    qreal velocityY_;
    const qreal diameter_;
    const qreal initialX_;
    const qreal initialY_;
    const qreal initialVelocityX_;
    const qreal initialVelocityY_;
};
