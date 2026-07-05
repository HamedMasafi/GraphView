#pragma once

#include "abstracttool.h"

namespace GraphView
{

class WidgetMoveTool : public AbstractTool
{
    Q_OBJECT
public:
    Q_DECL_DEPRECATED
    WidgetMoveTool(Scene *scene);

    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    AbstractWidget *_selectedWidget{nullptr};
    QPointF _clickPos;
    bool _wasMoved{false};
    QRectF resizeRect;
    QPointF _lastPos;

};

}
