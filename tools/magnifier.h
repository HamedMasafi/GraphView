#pragma once

#include "abstracttool.h"

class QGraphicsRectItem;
namespace GraphView
{
class View;
class Scene;

class MagnifierTool : public AbstractTool
{
public:
    explicit MagnifierTool(Scene *scene);

    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    QGraphicsRectItem *_zoomArea;
    QPointF _clickPos;
};

}
