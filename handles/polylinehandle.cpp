#include "polylinehandle.h"

namespace GraphView::Handles
{

PolylineHandle::PolylineHandle()
    : AbstractHandle{}
{
}

Widgets::PolylineItem *PolylineHandle::poly() const
{
    return _poly;
}

void PolylineHandle::setPoly(Widgets::PolylineItem *newPoly)
{
    _poly = newPoly;
}

SizeType PolylineHandle::index() const
{
    return _index;
}

void PolylineHandle::setIndex(SizeType newIndex)
{
    _index = newIndex;
}
}
