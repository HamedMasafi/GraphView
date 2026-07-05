#include "abstracthandle.h"

#include <QApplication>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QPalette>

namespace GraphView {

constexpr int ResizeHandleSize{10};

qreal AbstractHandle::defaultSize{10};
QColor AbstractHandle::defaultColor{Qt::blue};

AbstractHandle::AbstractHandle(QGraphicsItem *parent)
    : QGraphicsObject{parent}
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setAcceptHoverEvents(true);

    // _selectedBrush = QBrush{Qt::yellow};
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    // setPen(QPen{Qt::darkGray});
    // setBrush(Qt::green);
}

AbstractHandleData *AbstractHandle::handleData() const
{
    return _handleData;
}

void AbstractHandle::setHandleData(AbstractHandleData *newHandleData)
{
    _handleData = newHandleData;
}

qsizetype AbstractHandle::index() const
{
    return _index;
}

void AbstractHandle::setIndex(qsizetype newIndex)
{
    _index = newIndex;
}

QColor AbstractHandle::selectedBorderColor() const
{
    return _selectedBorderColor;
}

void AbstractHandle::setSelectedBorderColor(const QColor &newSelectedBorderColor)
{
    _selectedBorderColor = newSelectedBorderColor;
    update();
}

QColor AbstractHandle::selectedBackgroundColor() const
{
    return _selectedBackgroundColor;
}

void AbstractHandle::setSelectedBackgroundColor(const QColor &newSelectedBackgroundColor)
{
    _selectedBackgroundColor = newSelectedBackgroundColor;
    update();
}

QColor AbstractHandle::borderColor() const
{
    return _borderColor;
}

void AbstractHandle::setBorderColor(const QColor &newBorderColor)
{
    _borderColor = newBorderColor;
    update();
}

QColor AbstractHandle::backgroundColor() const
{
    return _backgroundColor;
}

void AbstractHandle::setBackgroundColor(const QColor &newBackgroundColor)
{
    _backgroundColor = newBackgroundColor;
    update();
}

void AbstractHandle::setSize(qreal newSize)
{
    prepareGeometryChange();
    _size = newSize;
}

QRectF AbstractHandle::boundingRect() const
{
    return {-_size / 2, -_size / 2, _size, _size};
}

void AbstractHandle::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setPen(isSelected() ? _selectedBorderColor : _borderColor);
    painter->setBrush(isSelected() ? _selectedBackgroundColor : _backgroundColor);
    painter->drawEllipse(boundingRect());
}

} // namespace GraphView
