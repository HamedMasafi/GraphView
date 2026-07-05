#include "widgetviewproxy.h"

namespace GraphView::Widgets {

WidgetViewProxy::WidgetViewProxy()
    : QGraphicsProxyWidget{}
{}

bool WidgetViewProxy::sceneEvent(QEvent *event)
{
    Q_UNUSED(event)
    return false;
}

} // namespace GraphView::Widgets
