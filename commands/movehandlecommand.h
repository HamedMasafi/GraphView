#pragma once

#include <QPointF>
#include <QUndoCommand>
#include "core/graphview_global.h"

namespace GraphView {
class PolylineItem;

class MoveHandleCommand : public QUndoCommand
{
public:
    MoveHandleCommand(PolylineItem *poly, SizeType index, QPointF oldPos, QPointF newPos);

    void undo();
    void redo();

private:
    PolylineItem *_poly;
    SizeType _index;
    QPointF _oldPos;
    QPointF _newPos;
};
}
