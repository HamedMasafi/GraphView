#include "createpolyline.h"

#include "../widgets/polylineitem.h"
#include "scene.h"

#include <QGraphicsSceneMouseEvent>

namespace GraphView::Tools
{

CreatePolyline::CreatePolyline(Scene *scene)
    : AbstractTool{scene}
{
}

void CreatePolyline::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (Q_UNLIKELY(!_item)) {
        if (_creator)
            _item = _creator();
        else {
            _item = new Widgets::PolylineItem;
            _scene->addItem(_item);
        }
    }

    _item->addNode(mouseEvent->scenePos());

    if (_item->maxNodesCount() && _item->maxNodesCount() <= _item->count()) {

    }
}

QString CreatePolyline::text() const
{
    return "Create polyline";
}

void CreatePolyline::setCreator(const std::function<Widgets::PolylineItem *()> &newCreator)
{
    _creator = newCreator;
}

AbstractTool::ToolType CreatePolyline::toolType() const
{
    return AbstractTool::ToolType::RequireActivation;
}

}
