#include "crossconnection.h"

#include <QPainter>
#include <QPen>

namespace GraphView
{

CrossConnectionWidget::CrossConnectionWidget()
    : ConnectionHandleWidget{}
{
    setSize(20, 20);
}

void CrossConnectionWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
//    painter->drawRect(0, 0, _size.width() - 1, _size.height() - 1);
    constexpr qreal dotSize{4};
    QPen pen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(centerPoint(), dotSize, dotSize);
}

}
