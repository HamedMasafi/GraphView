#include "magnifier.h"
#include "graphview.h"
#include "scene.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

namespace GraphView{

MagnifierTool::MagnifierTool(Scene *scene)
    : AbstractTool{scene}
{
    _zoomArea = scene->addRect(0, 0, 0, 0);
    _zoomArea->setPen(QPen{Qt::blue, 1, Qt::DotLine});
}

void MagnifierTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    _clickPos = mouseEvent->scenePos();
    _zoomArea->setRect(QRectF{_clickPos, QSizeF{0, 0}});
}

void MagnifierTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    _zoomArea->setRect(QRectF{_clickPos, mouseEvent->scenePos() - _clickPos});
}

void MagnifierTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
    _zoomArea->hide();
    _view->ensureVisible(_zoomArea->rect());
}

}
