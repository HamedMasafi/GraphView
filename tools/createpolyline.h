#pragma once

#include "abstracttool.h"

class PolylineItem;

namespace GraphView
{

class CreatePolylineTool : public AbstractTool
{
    Q_OBJECT

public:
    explicit CreatePolylineTool(Scene *scene);
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;
    QString text() const override;
    ToolType toolType() const override;

    void setCreator(const std::function<PolylineItem *()> &newCreator);

private:
    std::function<PolylineItem *()> _creator;
    PolylineItem *_item{nullptr};
    QPolygonF _poly;
};

}
