#include "resizehandlesprovider.h"
#include "scene.h"

#include <QCursor>

#define CIRCLER 10

namespace GraphView
{


namespace Tools
{

ResizeHandlesProvider::ResizeHandlesProvider(Scene *scene)
    : AbstractTool{scene}
{
//    initHandles();
}

bool ResizeHandlesProvider::accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(item)
    Q_UNUSED(mouseEvent)
    return false;
}

}

}
