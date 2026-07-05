#include "resizewidget.h"
#include "scene.h"
#include "widgets/abstractwidget.h"

#include <QCursor>
#include <QGraphicsSceneEvent>
#include <QPainter>
#include <event.h>

#include <widgets/abstractwidget.h>

// #define CIRCLER 10

namespace GraphView
{

namespace {

constexpr int ResizeHandleSize {15};

class ResizeHandle :public QGraphicsEllipseItem {

public:
    ResizeHandle()
        : QGraphicsEllipseItem{-ResizeHandleSize / 2, -ResizeHandleSize / 2, ResizeHandleSize, ResizeHandleSize}
    {
        setFlag(QGraphicsItem::ItemIgnoresTransformations);
        setPen(QPen{Qt::darkGray});
        setBrush(Qt::green);
    }

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

OldResizeHandle::OldResizeHandle()
    : QGraphicsEllipseItem{-5, -5, 5, 5}
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

ResizeDirection OldResizeHandle::resizeDirection() const
{
    return m_resizeDirection;
}

void OldResizeHandle::setResizeDirection(ResizeDirection newResizeDirection)
{
    m_resizeDirection = newResizeDirection;
}

ResizeWidgetTool::ResizeWidgetTool(Scene *scene)
    : AbstractTool{scene}
{
    initHandles();
}

bool ResizeWidgetTool::accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
    if (!item) {
        setVisible(false);
        return false;
    }

    if (!(item->flags() & QGraphicsItem::ItemIsSelectable)) {
        setVisible(false);
        return false;
    }

    auto widget = dynamic_cast<AbstractWidget *>(item);
    if (widget) {
        if (_selectedWidget)
            disconnect(_selectedWidget, nullptr, this, nullptr);
        _selectedWidget = widget;
        widget->setSelected(true);
        setHandlesOnItem(widget);
    }

    setVisible(_selectedWidget);
    if (!_selectedWidget)
        return false;

    connect(_selectedWidget, &AbstractWidget::moving, this, &ResizeWidgetTool::widget_moving);
    connect(_selectedWidget, &QObject::destroyed, this, &ResizeWidgetTool::widget_destroyed);

    resizeRect = _selectedWidget->childRect();
    _selectedHandle = dynamic_cast<ResizeHandle *>(item);
    return _selectedHandle;
}

void ResizeWidgetTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
}

void ResizeWidgetTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton) {
        if (!_selectedHandle)
            return;
        QPointF point{mouseEvent->scenePos().x() - 5, mouseEvent->scenePos().y() - 5};

        QRectF rc = resizeRect;

        if (_selectedHandle->resizeDirection() & Top)
            rc.setTop(point.y());
        if (_selectedHandle->resizeDirection() & Left)
            rc.setLeft(point.x());
        if (_selectedHandle->resizeDirection() & Right)
            rc.setRight(point.x());
        if (_selectedHandle->resizeDirection() & Bottom)
            rc.setBottom(point.y());

        if (rc.isValid()) {
            _selectedWidget->setRect(rc);
            setHandlesOnItem(_selectedWidget);
        } // if
    }
}

void ResizeWidgetTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
    Q_EMIT finished();
}

AbstractTool::ToolType ResizeWidgetTool::toolType() const
{
    return AbstractTool::ToolType::RequireActivation;
}

void ResizeWidgetTool::widget_moving(MoveEvent *)
{
    setHandlesOnItem(_selectedWidget);
}

void ResizeWidgetTool::widget_destroyed(QObject *)
{
    setVisible(false);
}

void ResizeWidgetTool::initHandles()
{
    resizerTL = new ResizeHandle;
    resizerT = new ResizeHandle;
    resizerTR = new ResizeHandle;
    resizerL = new ResizeHandle;
    resizerR = new ResizeHandle;
    resizerBL = new ResizeHandle;
    resizerB = new ResizeHandle;
    resizerBR = new ResizeHandle;

    handles.append(resizerTL);
    handles.append(resizerT);
    handles.append(resizerTR);
    handles.append(resizerL);
    handles.append(resizerR);
    handles.append(resizerBL);
    handles.append(resizerB);
    handles.append(resizerBR);

    resizerTL->setCursor(Qt::SizeFDiagCursor);
    resizerT->setCursor(Qt::SizeVerCursor);
    resizerTR->setCursor(Qt::SizeBDiagCursor);
    resizerL->setCursor(Qt::SizeHorCursor);
    resizerR->setCursor(Qt::SizeHorCursor);
    resizerBL->setCursor(Qt::SizeBDiagCursor);
    resizerB->setCursor(Qt::SizeVerCursor);
    resizerBR->setCursor(Qt::SizeFDiagCursor);

    resizerT->setResizeDirection(Top);
    resizerL->setResizeDirection(Left);
    resizerR->setResizeDirection(Right);
    resizerB->setResizeDirection(Bottom);

    resizerTR->setResizeDirection(Top | Right);
    resizerTL->setResizeDirection(Top | Left);
    resizerBR->setResizeDirection(Bottom | Right);
    resizerBL->setResizeDirection(Bottom | Left);

    for (auto &handle : handles) {
        _scene->addItem(handle);
        handle->setZValue(999999);
        handle->hide();
    } // for
}

void ResizeWidgetTool::setHandlesOnItem(AbstractWidget *widget)
{
    auto rc = widget->childRect();

    setResezeHandlePos(resizerTL, rc.topLeft());
    setResezeHandlePos(resizerT, rc.topRight(), rc.topLeft());
    setResezeHandlePos(resizerTR, rc.topRight());

    setResezeHandlePos(resizerL, rc.topLeft(), rc.bottomLeft());
    setResezeHandlePos(resizerR, rc.topRight(), rc.bottomRight());

    setResezeHandlePos(resizerBL, rc.bottomLeft());
    setResezeHandlePos(resizerB, rc.bottomRight(), rc.bottomLeft());
    setResezeHandlePos(resizerBR, rc.bottomRight());

    for (int i = 0; i < handles.count(); i++)
        handles.at(i)->setZValue(9999);
}

void ResizeWidgetTool::setVisible(bool visible)
{
    for (auto &h : handles)
        h->setVisible(visible);
}

void ResizeWidgetTool::setResezeHandlePos(QGraphicsEllipseItem *handle, QPointF pos)
{
    // qreal scaleHalf = m_scale * CIRCLER / 2;
    handle->setPos(pos); //_selectedWidgets.at(0)->parentItem()->mapToParent(
                    ////_parent->mapToParent (
        // pos.x() - scaleHalf,
        // pos.y() - scaleHalf);
}

void ResizeWidgetTool::setResezeHandlePos(QGraphicsEllipseItem *handle, QPointF pos1, QPointF pos2)
{
    // qreal scaleHalf = m_scale * CIRCLER / 2;
    handle->setPos( //_selectedWidgets.at(0)->parentItem()->mapToParent(
                    ////_parent->mapToParent (
        (pos2.x() + pos1.x()) / 2,
        (pos2.y() + pos1.y()) / 2);
}

}
