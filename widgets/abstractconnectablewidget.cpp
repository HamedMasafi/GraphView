#include "abstractconnectablewidget.h"

#include "handles/connectionhandle.h"

#include <QGraphicsLinearLayout>

namespace GraphView {

AbstractConnectableWidget::AbstractConnectableWidget(QGraphicsItem *parent)
    : AbstractRectWidget{parent}
{}

void AbstractConnectableWidget::addConnectionNode(Qt::Edge edge)
{
    auto handle = new ConnectionHandleWidget{this};
   _connectionHandles.insert(edge, handle);
}

void AbstractConnectableWidget::resized() {

}

} // namespace GraphView
