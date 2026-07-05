#pragma once

#include "abstracttool.h"

#include <QGraphicsItem>

namespace GraphView
{

class CrossPointerTool : public AbstractTool
{
    Q_OBJECT

public:
    explicit CrossPointerTool(Scene *scene);

    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    ToolType toolType() const override;

private:
    QGraphicsLineItem *_verticalLine;
    QGraphicsLineItem *_horizontalLine;
};
}
