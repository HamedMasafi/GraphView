#include "editrelation.h"
#include "widgets/relation.h"
#include "scene.h"

#include <QGraphicsSceneEvent>

namespace GraphView
{
EditRelationTool::EditRelationTool(Scene *scene) : AbstractTool{scene}
{
}

bool EditRelationTool::accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(item)
    auto relations = _scene->relations();
    for (auto &r:relations)
        if (r->intersect(mouseEvent->scenePos(), nullptr)){
            _relation = r;
            return true;
        }
    _relation = nullptr;
    return false;
}

void EditRelationTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton) {
        _relation->setActive(true);
        _relation->processMousePressEvent(_scene->snapPoint(mouseEvent->scenePos()));
    }
}

void EditRelationTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton)
        _relation->processMouseMoveEvent(_scene->snapPoint(mouseEvent->scenePos()));
}

void EditRelationTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton)
        _relation->processMouseReleaseEvent(_scene->snapPoint(mouseEvent->scenePos()));
    Q_EMIT finished();
}

}
