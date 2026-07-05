#pragma once

#include "abstracttool.h"

class QGraphicsRectItem;
namespace GraphView
{

class AbstractHandle;
class PolylineHandle;

class NodesTool : public AbstractTool
{
    Q_OBJECT
public:
    NodesTool(Scene *scene);

    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClicked(QGraphicsSceneMouseEvent *mouseEvent) override;

    void keyReleaseEvent(QKeyEvent *event) override;
    ToolType toolType() const override;

private:
    enum class MouseMode {
        None,
        MoveHandle,
        DrawRect
    };
    MouseMode _mouseMode{MouseMode::None};

    QGraphicsRectItem *_selectArea;
    QPointF _clickPos;
    QPointF _startPos;
    QList<AbstractHandle *> _handles;
    PolylineHandle *_selectedHandle{nullptr};
};

}
