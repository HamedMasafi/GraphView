#include "zoom.h"
#include "graphview.h"
#include "scene.h"

#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>
#include <QTransform>
#include <QVariant>

namespace GraphView {

ZoomTool::ZoomTool(Scene *scene)
    : AbstractTool{scene}
{}

void ZoomTool::setZoomLevel(int newZoomLevel)
{
    if (newZoomLevel < 0 || newZoomLevel > 200)
        return;

    _zoomLevel = newZoomLevel;

    int zoomValue;
    if (newZoomLevel < 100)
        zoomValue = (newZoomLevel * 75 / 100) + 25;
    else
        zoomValue = ((newZoomLevel - 100) * 3) + 100;

    qreal scale = zoomValue / 100.00;
    QTransform t;
    t.scale(scale, scale);

    for (auto &view : _scene->views())
        if (!view->property("__noZoom").toBool())
            view->setTransform(t);

    emit zoomLevelChanged(_zoomLevel);
}

void ZoomTool::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (Q_LIKELY(_view)) {
        if (event->modifiers() & Qt::ControlModifier) {
            setZoomLevel(_zoomLevel + (event->delta() > 0 ? 5 : -5));
            event->accept();
        }
    }
}

AbstractTool::ToolType ZoomTool::toolType() const
{
    return AbstractTool::ToolType::AlwaysListener;
}

int ZoomTool::zoomLevel() const
{
    return _zoomLevel;
}

} // namespace GraphView
