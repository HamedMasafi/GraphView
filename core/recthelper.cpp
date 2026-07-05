#include "recthelper.h"

namespace GraphView::Core
{

QPointF edgePoint(const QRectF &rc, Qt::Alignment align)
{
    qreal x;
    qreal y;

    if (align & Qt::AlignTop)
        y = rc.top();
    else if (align & Qt::AlignBottom)
        y = rc.top() + rc.height();
    else
        y = rc.top() + rc.height() / 2;

    if (align & Qt::AlignLeft)
        x = rc.left();
    else if (align & Qt::AlignRight)
        x = rc.left() + rc.width();
    else
        x = rc.left() + rc.width() / 2;

    return QPointF{x, y};
}

}
