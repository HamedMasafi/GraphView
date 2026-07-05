#pragma once

#include <QGraphicsEllipseItem>
#include "../itemresizer.h"
#include "abstracthandle.h"

namespace GraphView::Handles
{

class ResizeHandle : public AbstractHandle
{
public:
    ResizeHandle();

    Widgets::ResizeDirection resizeDirection() const{
        return m_resizeDirection;
    }
    void setResizeDirection(Widgets::ResizeDirection newResizeDirection)
    {
        m_resizeDirection = newResizeDirection;
    }

private:
    Widgets::ResizeDirection m_resizeDirection;
};

}
