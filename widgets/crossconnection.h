#pragma once

#include "handles/connectionhandle.h"

namespace GraphView::Widgets
{

class CrossConnection : public ConnectionHandle
{
    Q_OBJECT

public:
    enum { Type = UserType + 5 };

    int type() const override
    {
        return Type;
    }
    CrossConnection();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

}
