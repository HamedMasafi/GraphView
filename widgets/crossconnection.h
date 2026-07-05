#pragma once

#include "handles/connectionhandle.h"

namespace GraphView
{

class CrossConnectionWidget : public ConnectionHandleWidget
{
    Q_OBJECT

public:
    enum { Type = UserType + 5 };

    int type() const override
    {
        return Type;
    }
    CrossConnectionWidget();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

}
