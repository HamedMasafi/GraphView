#include "resizehandlesprovider.h"
#include "scene.h"

#include <QCursor>

#define CIRCLER 10

namespace GraphView
{


ResizeHandlesProviderTool::ResizeHandlesProviderTool(Scene *scene)
    : AbstractTool{scene}
{
//    initHandles();
}

bool ResizeHandlesProviderTool::accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(item)
    Q_UNUSED(mouseEvent)
    return false;
}

}
