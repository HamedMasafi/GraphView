#pragma once

#include "abstracttool.h"

class QGraphicsRectItem;
namespace GraphView
{
class View;
class Scene;
namespace Tools
{
class Magnifier : public AbstractTool
{
public:
    explicit Magnifier(Scene *scene);

    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    QGraphicsRectItem *_zoomArea;
    QPointF _clickPos;
};

}
}
