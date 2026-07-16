#include "movebymodiftool.h"
#include "graphview.h"
#include "scene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QScrollBar>

namespace GraphView::Tools {

MoveByModifTool::MoveByModifTool(Scene *scene)
    : AbstractTool{scene}
{}

QString MoveByModifTool::text() const
{
    return "Scroll drag";
}

AbstractTool::ToolType MoveByModifTool::toolType() const
{
    return ToolType::AlwaysListener;
}

void MoveByModifTool::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        view()->setDragMode(QGraphicsView::ScrollHandDrag);
        m_spacePressed = true;
        event->setAccepted(false);
        setTempTool(true);
    }
}

void MoveByModifTool::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        view()->setDragMode(QGraphicsView::NoDrag);
        m_spacePressed = false;
        setTempTool(false);
    }
}

void MoveByModifTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_spacePressed || mouseEvent->buttons() & Qt::MiddleButton) {
        m_lastPos = mouseEvent->scenePos();
        view()->setCursor(Qt::ClosedHandCursor);
        m_dragging = true;

        _horiScrollValue = view()->horizontalScrollBar()->value();
        _vertScrollValue = view()->verticalScrollBar()->value();
    } else {
        m_dragging = false;
    }
}

void MoveByModifTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_dragging) {
        auto delta = mouseEvent->scenePos() - m_lastPos;

        // حرکت دادن اسکرول‌بارها = جابجایی صحنه
        view()->horizontalScrollBar()->setValue(view()->horizontalScrollBar()->value()
                                                - static_cast<int>(delta.x()));
        view()->verticalScrollBar()->setValue(view()->verticalScrollBar()->value()
                                              - static_cast<int>(delta.y()));

        // m_lastPos = mouseEvent->scenePos();
    }
}

void MoveByModifTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
    if (m_dragging) {
        m_dragging = false;
        view()->setCursor(Qt::ArrowCursor);
    }
}

} // namespace GraphView::Tools
