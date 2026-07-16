#pragma once

#include <QPointF>
#include <QUndoCommand>
#include "core/graphview_global.h"

namespace GraphView::Widgets {
class PolylineItem;
}

namespace GraphView::Commands {
class MoveHandleCommand : public QUndoCommand
{
public:
    MoveHandleCommand(Widgets::PolylineItem *poly, SizeType index, QPointF oldPos, QPointF newPos);

    void undo();
    void redo();

private:
    Widgets::PolylineItem *_poly;
    SizeType _index;
    QPointF _oldPos;
    QPointF _newPos;
};
} // namespace GraphView::Commands
