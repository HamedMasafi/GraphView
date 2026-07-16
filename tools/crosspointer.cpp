#include "crosspointer.h"

#include <QGraphicsSceneDragDropEvent>
#include <QPainter>
#include <scene.h>

namespace GraphView::Tools
{

CrossPointer::CrossPointer(Scene *scene)
    : AbstractTool{scene}
{
    _verticalLine = _scene->addLine(0, 0, 0, 0);
    _horizontalLine = _scene->addLine(0, 0, 0, 0);

    QPen p{QBrush{Qt::gray}, 1, Qt::DotLine};
    _verticalLine->setPen(p);
    _horizontalLine->setPen(p);
}

void CrossPointer::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    auto rc = _scene->sceneRect();

    _verticalLine->setLine(mouseEvent->scenePos().x(), rc.top(), mouseEvent->scenePos().x(), rc.bottom());
    _horizontalLine->setLine(rc.left(), mouseEvent->scenePos().y(), rc.right(), mouseEvent->scenePos().y());
}

AbstractTool::ToolType CrossPointer::toolType() const
{
    return ToolType::AlwaysListener;
}

}
