#pragma once

#include "abstracttool.h"

#include <QGraphicsEllipseItem>

namespace GraphView
{

class ResizeHandlesProviderTool:public AbstractTool
{
    Q_OBJECT
public:
    ResizeHandlesProviderTool(Scene *scene);
    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;

private:

};

}
