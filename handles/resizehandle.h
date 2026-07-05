#pragma once

#include <QGraphicsEllipseItem>
#include "../itemresizer.h"
#include "abstracthandle.h"

namespace GraphView
{

class ResizeHandle : public AbstractHandle
{
public:
    ResizeHandle();

    ResizeDirection resizeDirection() const{
        return m_resizeDirection;
    }
    void setResizeDirection(ResizeDirection newResizeDirection)
    {
        m_resizeDirection = newResizeDirection;
    }

private:
    ResizeDirection m_resizeDirection;
};

}
