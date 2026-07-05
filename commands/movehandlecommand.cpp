#include "movehandlecommand.h"
#include "widgets/polylineitem.h"
#include <assert.h>

namespace GraphView {

MoveHandleCommand::MoveHandleCommand(PolylineItem *poly,
                                     SizeType index,
                                     QPointF oldPos,
                                     QPointF newPos)
    : _poly(poly)
    , _index(index)
    , _oldPos(oldPos)
    , _newPos(newPos)
{}

void MoveHandleCommand::undo()
{
    Q_ASSERT(_poly->count() > _index && _index >= 0);
    _poly->setNode(_index, _oldPos);
}

void MoveHandleCommand::redo()
{
    Q_ASSERT(_poly->count() > _index && _index >= 0);
    _poly->setNode(_index, _newPos);
}

}
