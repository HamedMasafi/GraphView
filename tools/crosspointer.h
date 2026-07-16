#pragma once

#include "abstracttool.h"

#include <QGraphicsItem>

namespace GraphView::Tools
{

class CrossPointer : public AbstractTool
{
    Q_OBJECT

public:
    explicit CrossPointer(Scene *scene);

    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    ToolType toolType() const override;

private:
    QGraphicsLineItem *_verticalLine;
    QGraphicsLineItem *_horizontalLine;
};
}
