#include "zoom.h"
#include "graphview.h"
#include "scene.h"

#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>
#include <QTransform>
#include <QVariant>

namespace GraphView::Tools {

Zoom::Zoom(Scene *scene)
    : AbstractTool{scene}
{}

void Zoom::setZoomLevel(int newZoomLevel)
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

void Zoom::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (Q_LIKELY(_view)) {
        if (event->modifiers() & Qt::ControlModifier) {
            setZoomLevel(_zoomLevel + (event->delta() > 0 ? 5 : -5));
            event->accept();
        }
    }
}

AbstractTool::ToolType Zoom::toolType() const
{
    return AbstractTool::ToolType::AlwaysListener;
}

int Zoom::zoomLevel() const
{
    return _zoomLevel;
}

} // namespace GraphView::Tools
