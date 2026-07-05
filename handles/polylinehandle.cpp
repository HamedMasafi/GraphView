#include "polylinehandle.h"

namespace GraphView
{

PolylineHandle::PolylineHandle()
    : AbstractHandle{}
{
}

PolylineItem *PolylineHandle::poly() const
{
    return _poly;
}

void PolylineHandle::setPoly(PolylineItem *newPoly)
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
