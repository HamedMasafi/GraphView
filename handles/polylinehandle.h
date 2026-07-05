#pragma once

#include "handles/abstracthandle.h"

namespace GraphView
{

class PolylineItem;

class PolylineHandle : public AbstractHandle
{
    Q_OBJECT

public:
    PolylineHandle();

    PolylineItem *poly() const;
    void setPoly(PolylineItem *newPoly);

    SizeType index() const;
    void setIndex(SizeType newIndex);

private:
    PolylineItem *_poly;
    SizeType _index{};
};

}
