#include "abstractconnectablewidget.h"

#include "handles/connectionhandle.h"

#include <QGraphicsLinearLayout>

namespace GraphView::Widgets {

AbstractConnectableWidget::AbstractConnectableWidget(QGraphicsItem *parent)
    : AbstractRectWidget{parent}
{}

void AbstractConnectableWidget::addConnectionNode(Qt::Edge edge)
{
    auto handle = new ConnectionHandle{this};
   _connectionHandles.insert(edge, handle);
}

void AbstractConnectableWidget::resized() {

}

} // namespace GraphView::Widgets
