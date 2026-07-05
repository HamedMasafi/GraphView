#pragma once

#include "abstractrectwidget.h"

namespace GraphView {

class ConnectionHandleWidget;
class AbstractConnectableWidget : public AbstractRectWidget
{
    Q_OBJECT

public:
    AbstractConnectableWidget(QGraphicsItem *parent = nullptr);

    void addConnectionNode(Qt::Edge edge);

    QMap<Qt::Edge, QGraphicsLinearLayout *> _edgeNodesLayout;
    QMultiMap<Qt::Edge, ConnectionHandleWidget *> _connectionHandles;

protected:
    void resized();
};

} // namespace GraphView
