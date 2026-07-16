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

namespace Impl {

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
namespace Widgets
{
Widgets::ResizeHandle2::ResizeHandle2()
    : QGraphicsEllipseItem{-5, -5, 5, 5}
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

ResizeDirection ResizeHandle2::resizeDirection() const
{
    return m_resizeDirection;
}

void ResizeHandle2::setResizeDirection(ResizeDirection newResizeDirection)
{
    m_resizeDirection = newResizeDirection;
}

}

namespace Tools
{

ResizeWidget::ResizeWidget(Scene *scene)
    : AbstractTool{scene}
{
    initHandles();
}

bool ResizeWidget::accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent)
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

    auto widget = dynamic_cast<Widgets::AbstractWidget *>(item);
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

    connect(_selectedWidget, &Widgets::AbstractWidget::moving, this, &ResizeWidget::widget_moving);
    connect(_selectedWidget, &QObject::destroyed, this, &ResizeWidget::widget_destroyed);

    resizeRect = _selectedWidget->childRect();
    _selectedHandle = dynamic_cast<Impl::ResizeHandle *>(item);
    return _selectedHandle;
}

void ResizeWidget::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
}

void ResizeWidget::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton) {
        if (!_selectedHandle)
            return;
        QPointF point{mouseEvent->scenePos().x() - 5, mouseEvent->scenePos().y() - 5};

        QRectF rc = resizeRect;

        if (_selectedHandle->resizeDirection() & Widgets::Top)
            rc.setTop(point.y());
        if (_selectedHandle->resizeDirection() & Widgets::Left)
            rc.setLeft(point.x());
        if (_selectedHandle->resizeDirection() & Widgets::Right)
            rc.setRight(point.x());
        if (_selectedHandle->resizeDirection() & Widgets::Bottom)
            rc.setBottom(point.y());

        if (rc.isValid()) {
            _selectedWidget->setRect(rc);
            setHandlesOnItem(_selectedWidget);
        } // if
    }
}

void ResizeWidget::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
    Q_EMIT finished();
}

AbstractTool::ToolType ResizeWidget::toolType() const
{
    return AbstractTool::ToolType::RequireActivation;
}

void ResizeWidget::widget_moving(Core::MoveEvent *)
{
    setHandlesOnItem(_selectedWidget);
}

void ResizeWidget::widget_destroyed(QObject *)
{
    setVisible(false);
}

void ResizeWidget::initHandles()
{
    resizerTL = new Impl::ResizeHandle;
    resizerT = new Impl::ResizeHandle;
    resizerTR = new Impl::ResizeHandle;
    resizerL = new Impl::ResizeHandle;
    resizerR = new Impl::ResizeHandle;
    resizerBL = new Impl::ResizeHandle;
    resizerB = new Impl::ResizeHandle;
    resizerBR = new Impl::ResizeHandle;

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

    resizerT->setResizeDirection(Widgets::Top);
    resizerL->setResizeDirection(Widgets::Left);
    resizerR->setResizeDirection(Widgets::Right);
    resizerB->setResizeDirection(Widgets::Bottom);

    resizerTR->setResizeDirection(Widgets::Top | Widgets::Right);
    resizerTL->setResizeDirection(Widgets::Top | Widgets::Left);
    resizerBR->setResizeDirection(Widgets::Bottom | Widgets::Right);
    resizerBL->setResizeDirection(Widgets::Bottom | Widgets::Left);

    for (auto &handle : handles) {
        _scene->addItem(handle);
        handle->setZValue(999999);
        handle->hide();
    } // for
}

void ResizeWidget::setHandlesOnItem(Widgets::AbstractWidget *widget)
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

void ResizeWidget::setVisible(bool visible)
{
    for (auto &h : handles)
        h->setVisible(visible);
}

void ResizeWidget::setResezeHandlePos(Impl::ResizeHandle *handle, QPointF pos)
{
    // qreal scaleHalf = m_scale * CIRCLER / 2;
    handle->setPos(pos); //_selectedWidgets.at(0)->parentItem()->mapToParent(
                    ////_parent->mapToParent (
        // pos.x() - scaleHalf,
        // pos.y() - scaleHalf);
}

void ResizeWidget::setResezeHandlePos(Impl::ResizeHandle *handle, QPointF pos1, QPointF pos2)
{
    // qreal scaleHalf = m_scale * CIRCLER / 2;
    handle->setPos( //_selectedWidgets.at(0)->parentItem()->mapToParent(
                    ////_parent->mapToParent (
        (pos2.x() + pos1.x()) / 2,
        (pos2.y() + pos1.y()) / 2);
}

}
}
