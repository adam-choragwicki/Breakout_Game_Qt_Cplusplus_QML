#pragma once

#include <QRectF>

inline bool isRectOverlappingOrTouchingRect(const QRectF& rect1, const QRectF& rect2)
{
    return !(rect1.right() < rect2.left() ||
             rect1.left() > rect2.right() ||
             rect1.bottom() < rect2.top() ||
             rect1.top() > rect2.bottom());
}

inline bool isRectOverlappingRect(const QRectF& rect1, const QRectF& rect2)
{
    return rect1.intersects(rect2);
}
