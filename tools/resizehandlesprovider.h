#pragma once

#include "abstracttool.h"

#include <QGraphicsEllipseItem>

namespace GraphView
{
namespace Widgets{

}
namespace Tools{
class ResizeHandlesProvider:public AbstractTool
{
    Q_OBJECT
public:
    ResizeHandlesProvider(Scene *scene);
    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;

private:

};

}

}

