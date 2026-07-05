#include "crosspointer.h"

#include <QGraphicsSceneDragDropEvent>
#include <QPainter>
#include <scene.h>

namespace GraphView
{

CrossPointerTool::CrossPointerTool(Scene *scene)
    : AbstractTool{scene}
{
    _verticalLine = _scene->addLine(0, 0, 0, 0);
    _horizontalLine = _scene->addLine(0, 0, 0, 0);

    QPen p{QBrush{Qt::gray}, 1, Qt::DotLine};
    _verticalLine->setPen(p);
    _horizontalLine->setPen(p);
}

void CrossPointerTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    auto rc = _scene->sceneRect();

    _verticalLine->setLine(mouseEvent->scenePos().x(), rc.top(), mouseEvent->scenePos().x(), rc.bottom());
    _horizontalLine->setLine(rc.left(), mouseEvent->scenePos().y(), rc.right(), mouseEvent->scenePos().y());
}

AbstractTool::ToolType CrossPointerTool::toolType() const
{
    return ToolType::AlwaysListener;
}

}
