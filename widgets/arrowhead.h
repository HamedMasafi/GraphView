#pragma once

#include "abstractrectwidget.h"

#include <QGraphicsItem>

namespace GraphView
{

class ArrowHeadWidget : public AbstractRectWidget
{

public:
    enum { Type = UserType + 3 };

    int type() const override
    {
        return Type;
    }
    ArrowHeadWidget(QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    Qt::Edge edge() const;
    void setEdge(Qt::Edge newEdge);
    QPointF point() const;
    void setPoint(QPointF newPoint);
    qreal length() const;
    void setLength(qreal newLength);
    qreal thickness() const;
    void setThickness(qreal newThickness);
    QPolygonF arrowPoly() const;
    void setArrowPoly(const QPolygonF &newArrowPoly);

private:
    void calculateSize();

    Qt::Edge _edge{Qt::TopEdge};
    QPointF _point;
    qreal _length{10};
    qreal _thickness{10};
    QPolygonF _arrowPoly;
};

}
