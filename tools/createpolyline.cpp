#include "createpolyline.h"

#include "../widgets/polylineitem.h"
#include "scene.h"

#include <QGraphicsSceneMouseEvent>

namespace GraphView
{

CreatePolylineTool::CreatePolylineTool(Scene *scene)
    : AbstractTool{scene}
{
}

void CreatePolylineTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (Q_UNLIKELY(!_item)) {
        if (_creator)
            _item = _creator();
        else {
            _item = new PolylineItem;
            _scene->addItem(_item);
        }
    }

    _item->addNode(mouseEvent->scenePos());

    if (_item->maxNodesCount() && _item->maxNodesCount() <= _item->count()) {

    }
}

QString CreatePolylineTool::text() const
{
    return "Create polyline";
}

void CreatePolylineTool::setCreator(const std::function<PolylineItem *()> &newCreator)
{
    _creator = newCreator;
}

AbstractTool::ToolType CreatePolylineTool::toolType() const
{
    return AbstractTool::ToolType::RequireActivation;
}

}
