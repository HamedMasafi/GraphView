#pragma once

#include "handles/abstracthandle.h"

namespace GraphView {

namespace Widgets {
class PolylineItem;
}
namespace Handles {

class PolylineHandle : public AbstractHandle
{
    Q_OBJECT

public:
    PolylineHandle();

    Widgets::PolylineItem *poly() const;
    void setPoly(Widgets::PolylineItem *newPoly);

    SizeType index() const;
    void setIndex(SizeType newIndex);

private:
    Widgets::PolylineItem *_poly;
    SizeType _index{};
};

} // namespace Handles

} // namespace GraphView
