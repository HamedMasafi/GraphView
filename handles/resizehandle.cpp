#include "resizehandle.h"

#include <QPen>

namespace GraphView::Handles {
constexpr int ResizeHandleSize{15};

ResizeHandle::ResizeHandle()
    : AbstractHandle{}
{
    setBorderColor(Qt::darkGray);
    setBackgroundColor(Qt::green);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

} // namespace GraphView::Handles
