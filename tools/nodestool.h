#pragma once

#include "abstracttool.h"

class QGraphicsRectItem;
namespace GraphView
{

namespace Handles
{
class AbstractHandle;
class PolylineHandle;
}

namespace Tools
{

class NodesTool : public AbstractTool
{
    Q_OBJECT
public:
    NodesTool(GraphView::Scene *scene);

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
    QList<Handles::AbstractHandle *> _handles;
    Handles::PolylineHandle *_selectedHandle{nullptr};
};

}
}
