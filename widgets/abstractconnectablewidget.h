#pragma once

#include "abstractrectwidget.h"

namespace GraphView::Widgets {

class ConnectionHandle;
class AbstractConnectableWidget : public AbstractRectWidget
{
    Q_OBJECT

public:
    AbstractConnectableWidget(QGraphicsItem *parent = nullptr);

    void addConnectionNode(Qt::Edge edge);

    QMap<Qt::Edge, QGraphicsLinearLayout *> _edgeNodesLayout;
    QMultiMap<Qt::Edge, ConnectionHandle *> _connectionHandles;

protected:
    void resized();
};

} // namespace GraphView::Widgets
