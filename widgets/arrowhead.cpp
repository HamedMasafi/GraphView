#include "arrowhead.h"

#include <QPainter>

namespace GraphView
{
ArrowHeadWidget::ArrowHeadWidget(QGraphicsItem *parent)
    : AbstractRectWidget{parent}
{
}

void ArrowHeadWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawPolygon(_arrowPoly);
}

void ArrowHeadWidget::calculateSize()
{
    _arrowPoly.clear();
    switch(_edge){
    case Qt::TopEdge:
        setSize(_thickness, _length);
        setPos(_point.x() - _thickness / 2, _point.y());
        _arrowPoly.append(edgePoint(Qt::AlignLeft | Qt::AlignBottom));
        _arrowPoly.append(edgePoint(Qt::AlignTop));
        _arrowPoly.append(edgePoint(Qt::AlignRight | Qt::AlignBottom));
        break;
    case Qt::LeftEdge:
        setSize(_length, _thickness);
        setPos(_point.x(), _point.y() + _thickness / 2);
        _arrowPoly.append(edgePoint(Qt::AlignRight | Qt::AlignTop));
        _arrowPoly.append(edgePoint(Qt::AlignLeft));
        _arrowPoly.append(edgePoint(Qt::AlignRight | Qt::AlignBottom));
        break;
    case Qt::RightEdge:
        setSize(_length, _thickness);
        setPos(_point.x() - _length, _point.y() + _thickness / 2);
        _arrowPoly.append(edgePoint(Qt::AlignLeft | Qt::AlignTop));
        _arrowPoly.append(edgePoint(Qt::AlignRight));
        _arrowPoly.append(edgePoint(Qt::AlignLeft | Qt::AlignBottom));
        break;
    case Qt::BottomEdge:
        setSize(_thickness, _length);
        setPos(_point.x() - _thickness / 2, _point.y() - _length);
        _arrowPoly.append(edgePoint(Qt::AlignLeft | Qt::AlignTop));
        _arrowPoly.append(edgePoint(Qt::AlignBottom));
        _arrowPoly.append(edgePoint(Qt::AlignRight | Qt::AlignTop));
        break;
    }
    update();
}

Qt::Edge ArrowHeadWidget::edge() const
{
    return _edge;
}

void ArrowHeadWidget::setEdge(Qt::Edge newEdge)
{
    _edge = newEdge;
    calculateSize();
}

QPointF ArrowHeadWidget::point() const
{
    return _point;
}

void ArrowHeadWidget::setPoint(QPointF newPoint)
{
    _point = newPoint;
    calculateSize();
}

qreal ArrowHeadWidget::length() const
{
    return _length;
}

void ArrowHeadWidget::setLength(qreal newLength)
{
    _length = newLength;
    calculateSize();
}

qreal ArrowHeadWidget::thickness() const
{
    return _thickness;
}

void ArrowHeadWidget::setThickness(qreal newThickness)
{
    _thickness = newThickness;
    calculateSize();
}

QPolygonF ArrowHeadWidget::arrowPoly() const
{
    return _arrowPoly;
}

void ArrowHeadWidget::setArrowPoly(const QPolygonF &newArrowPoly)
{
    _arrowPoly = newArrowPoly;
    calculateSize();
}
}
