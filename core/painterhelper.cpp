#include "painterhelper.h"
#include "recthelper.h"

#include <QPainter>
#include <QPolygon>

namespace GraphView::Core
{

void paintArrow(QPainter *painter, Qt::Edge direction, const QRectF &rc)
{
    QPointF pt;
    QPolygonF arrowPoly;
    switch (direction) {
    case Qt::TopEdge:
        pt = edgePoint(rc, Qt::AlignTop);
        painter->drawLine(pt, rc.bottomLeft());
        painter->drawLine(pt, rc.bottomRight());
        break;
    case Qt::BottomEdge:
        pt = edgePoint(rc, Qt::AlignBottom);
        painter->drawLine(pt, rc.topLeft());
        painter->drawLine(pt, rc.topRight());
        break;
    case Qt::LeftEdge:
        pt = edgePoint(rc, Qt::AlignLeft);
        painter->drawLine(pt, rc.topRight());
        painter->drawLine(pt, rc.bottomRight());
        break;
    case Qt::RightEdge:
        pt = edgePoint(rc, Qt::AlignRight);
        painter->drawLine(pt, rc.topLeft());
        painter->drawLine(pt, rc.bottomLeft());
        break;
    }
}

void paintFilledArrow(QPainter *painter, Qt::Edge direction, const QRectF &rc)
{
    QPolygonF arrowPoly;
    switch (direction) {
    case Qt::TopEdge:
        arrowPoly << edgePoint(rc, Qt::AlignTop) << rc.bottomLeft() << rc.bottomRight();
        break;
    case Qt::BottomEdge:
        arrowPoly << edgePoint(rc, Qt::AlignBottom) << rc.topLeft() << rc.topRight();
        break;
    case Qt::LeftEdge:
        arrowPoly << edgePoint(rc, Qt::AlignLeft) << rc.topRight() << rc.bottomRight();
        break;
    case Qt::RightEdge:
        arrowPoly << edgePoint(rc, Qt::AlignRight) << rc.topLeft() << rc.bottomLeft();
        break;
    }

    painter->drawPolygon(arrowPoly);
}

}
