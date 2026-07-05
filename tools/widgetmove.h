#pragma once

#include "abstracttool.h"

namespace GraphView
{
namespace Widgets{
class AbstractWidget;
}
namespace Tools
{
class WidgetMove : public AbstractTool
{
    Q_OBJECT
public:
    Q_DECL_DEPRECATED
    WidgetMove(Scene *scene);

    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    Widgets::AbstractWidget *_selectedWidget{nullptr};
    QPointF _clickPos;
    bool _wasMoved{false};
    QRectF resizeRect;
    QPointF _lastPos;

};

}
}
