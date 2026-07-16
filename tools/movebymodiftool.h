#pragma once

#include "abstracttool.h"

namespace GraphView::Tools {

class MoveByModifTool : public AbstractTool
{
public:
    MoveByModifTool(Scene *scene);

    QString text() const override;
    ToolType toolType() const override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QPointF m_lastPos;
    bool m_dragging{false};
    bool m_spacePressed{false};
    int _horiScrollValue{};
    int _vertScrollValue{};
};
}
