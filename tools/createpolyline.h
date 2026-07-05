#pragma once

#include "abstracttool.h"

namespace GraphView::Widgets {
class PolylineItem;
}
namespace GraphView::Tools
{

class CreatePolyline : public AbstractTool
{
    Q_OBJECT

public:
    explicit CreatePolyline(Scene *scene);
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;
    QString text() const override;
    ToolType toolType() const override;

    void setCreator(const std::function<Widgets::PolylineItem *()> &newCreator);

private:
    std::function<Widgets::PolylineItem *()> _creator;
    Widgets::PolylineItem *_item{nullptr};
    QPolygonF _poly;
};

}
