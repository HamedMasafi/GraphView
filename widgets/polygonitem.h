#pragma once

#include "polylineitem.h"
#include <QPolygonF>

namespace GraphView::Widgets
{

class PolygonItem : public PolylineItem
{
    Q_OBJECT

public:
    enum {
        Type = UserType + 16
    };

    int type() const override
    {
        return Type;
    }
    explicit PolygonItem(QGraphicsItem *parent = nullptr);

    void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *) override;
};

} // namespace
