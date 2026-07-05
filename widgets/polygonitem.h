#pragma once

#include "polylineitem.h"
#include <QPolygonF>

namespace GraphView
{

class PolygonItemWidget : public PolylineItemWidget
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
    explicit PolygonItemWidget(QGraphicsItem *parent = nullptr);

    void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *) override;
};

} // namespace
