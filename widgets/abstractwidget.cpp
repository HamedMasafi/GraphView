#include "abstractwidget.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneEvent>
#include <QJsonObject>
#include "core/jsonserialize.h"
#include "handles/connectionhandle.h"

namespace GraphView {
AbstractWidget::AbstractWidget(QGraphicsItem *parent)
    : QGraphicsObject{parent}
    , _resizeDirection{None}
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void AbstractWidget::setRect(const QRectF &rc)
{
    setPos(rc.topLeft());
    setSize(rc.size());
    resized();
}

QSizeF AbstractWidget::size() const
{
    return _size;
}

void AbstractWidget::setSize(const QSizeF &newSize)
{
    prepareGeometryChange();
    _size = newSize;
    resized();
}

void AbstractWidget::setSize(qreal x, qreal y)
{
    setSize(QSizeF{x, y});
}

void AbstractWidget::setWidth(qreal w)
{
    setSize(w, _size.height());
}

void AbstractWidget::setHeight(qreal h)
{
    setSize(_size.width(), h);
}

QRectF AbstractWidget::boundingRect() const
{
    return QRectF{0, 0, _size.width() - 1, _size.height() - 1};
}

QRectF AbstractWidget::childRect() const
{
    return QRectF(pos(), size());
}

void AbstractWidget::saveState(QJsonObject &map) const
{
    map.insert("__childRect", toJson(childRect()));
    map.insert("__objectName", toJson(objectName()));
    map.insert("__className", toJson(_className));
    map.insert("__z", toJson(zValue()));
}

void AbstractWidget::restoreState(const QJsonObject &map)
{
    setRect(fromJson<QRectF>(map["__childRect"]));
    setObjectName(fromJson<QString>(map["__objectName"]));
    // _className = fromJson<QString>(map["__className"]);
    setZValue(fromJson<double>(map["__z"]));
}

// void AbstractWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
//{
//     //    emit mouseMove(event);

//    if (mouseEvent->buttons() & Qt::LeftButton) {
//        qreal x, y;
//        QPointF oldPoint = this->pos();

//        x = mouseEvent->buttonDownPos(Qt::LeftButton).x();
//        y = mouseEvent->buttonDownPos(Qt::LeftButton).y();

//        Core::MoveEvent event(QPointF(this->pos().x() + mouseEvent->pos().x() - x, this->pos().y() + mouseEvent->pos().y() - y), oldPoint);

//        if (mouseEvent->pos().x() != x || mouseEvent->pos().y() != y) {
//            //            d->
//            wasMoved = true;
//            emit moving(&event);
//        } // if

//        if (event.isAccepted()) {
//            /*this->moveBy(
//            mouseEvent->pos().x() -  x,
//            mouseEvent->pos().y() -  y );*/
//            this->setPos(event.point());
//        }
//    } // if
//}

// void AbstractWidget::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
//{
//     //    Q_D(WidgetBase);
//     lastPos = pos();
//     emit selectedChanged();
//     //    emit mousePress(mouseEvent);
// }

// void AbstractWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//     //    Q_D(WidgetBase);
//     if (wasMoved) {
//         emit moved(lastPos);
//         wasMoved = false;
//     } // if
//       //    emit mouseRelease(mouseEvent);
// }

void AbstractWidget::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    Q_EMIT doubleClicked();
}

QString AbstractWidget::className() const
{
    return _className;
}

void AbstractWidget::initLayout()
{
    //    _leftLayout = new QGraphicsLinearLayout{this};
}

WidgetResizeMode AbstractWidget::resizeMode() const
{
    return _resizeMode;
}

void AbstractWidget::setResizeMode(WidgetResizeMode newResizeMode)
{
    _resizeMode = newResizeMode;
}

ResizeDirection AbstractWidget::resizeDirection() const
{
    return _resizeDirection;
}

void AbstractWidget::setResizeDirection(ResizeDirection newResizeDirection)
{
    _resizeDirection = newResizeDirection;
}

QPair<qreal, qreal> AbstractWidget::aspectRatio() const
{
    return _aspectRatio;
}

void AbstractWidget::setAspectRatio(const QPair<qreal, qreal> &newAspectRatio)
{
    _aspectRatio = newAspectRatio;
}

QUuid AbstractWidget::id() const
{
    return _uuid;
}

QSizeF AbstractWidget::minimumSize() const
{
    return m_minimumSize;
}

void AbstractWidget::setMinimumSize(const QSizeF &newMinimumSize)
{
    if (m_minimumSize == newMinimumSize)
        return;
    m_minimumSize = newMinimumSize;
    emit minimumSizeChanged();
}

QSizeF AbstractWidget::maximumSize() const
{
    return m_maximumSize;
}

void AbstractWidget::setMaximumSize(const QSizeF &newMaximumSize)
{
    if (m_maximumSize == newMaximumSize)
        return;
    m_maximumSize = newMaximumSize;
    emit maximumSizeChanged();
}

QPointF AbstractWidget::centerPoint() const
{
    return QPointF{_size.width() / 2, _size.height() / 2};
}

QPointF AbstractWidget::sceneCenterPoint() const
{
    return mapToScene(centerPoint());
}

void AbstractWidget::setCenterPos(const QPointF &pt)
{
    setPos(pt.x() - _size.width() / 2, pt.y() - _size.height() / 2);
}

QPointF AbstractWidget::edgePoint(Qt::Alignment align) const
{
    qreal x;
    qreal y;

    if (align & Qt::AlignTop)
        y = 0;
    else if (align & Qt::AlignBottom)
        y = size().height();
    else
        y = size().height() / 2;

    if (align & Qt::AlignLeft)
        x = 0;
    else if (align & Qt::AlignRight)
        x = size().width();
    else
        x = size().width() / 2;

    return QPointF{x, y};
}

QPointF AbstractWidget::sceneEdgePoint(Qt::Alignment align) const
{
    return mapToScene(edgePoint(align));
}

qreal AbstractWidget::height() const
{
    return _size.height();
}

qreal AbstractWidget::width() const
{
    return _size.width();
}

} // namespace GraphView
