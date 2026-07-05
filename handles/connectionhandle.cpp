#include "connectionhandle.h"
#include "core/painterhelper.h"
#include "scene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

namespace GraphView::Widgets
{

ConnectionHandle::ConnectionHandle(QGraphicsItem *parent)
    :  AbstractRectWidget{parent}
{
    setSize(30, 20);
    setAcceptHoverEvents(true);
}

QRectF ConnectionHandle::boundingRect() const
{
    switch (_connectionEdge) {
    case Qt::TopEdge:
    case Qt::BottomEdge:
        return QRectF{0, 0, thikness, lenght};
        break;
    case Qt::LeftEdge:
    case Qt::RightEdge:
        return QRectF{0, 0, lenght, thikness};
        break;
    }
    return QRectF{};
}

void ConnectionHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    // constexpr int arrowSize{8};
    if (_isSelected) {
        painter->setPen(Qt::blue);
        painter->drawRect(QRectF{0., 0., size().width() - 1, size().height() - 1});
    }

    QRectF rc{0, (_size.height() - 17) / 2, 8, 17};
    switch (_connectionType) {
    case Unset:
    case Input:
        rc.moveRight(_size.width() / 2);
        break;
    case Output:
        rc.moveLeft(_size.width() / 2);
        break;
    case InputOutput:
        break;
    }
    painter->setPen(QPen{Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});
    if (_relation) {
        painter->drawLine(connectionPoint(), centerPoint());
        if (_connectionType == Input) {
            painter->setBrush(Qt::darkGray);
            Core::paintFilledArrow(painter, Qt::RightEdge, rc);
        }
    } else {
//        painter->fillRect(QRectF{QPointF{0,0},_size }, Qt::yellow);
        Core::paintArrow(painter, Qt::RightEdge, rc);
    }
}

QPointF ConnectionHandle::handleScenePos() const
{
    return mapToScene(QPointF{0, size().height() / 2});
}

void ConnectionHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    auto sc = qobject_cast<GraphView::Scene *>(scene());
    if (!sc)
        return;

    QGraphicsItem::mousePressEvent(event);
}

void ConnectionHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) { }
    QGraphicsItem::mouseMoveEvent(event);
}

Relation *ConnectionHandle::relation() const
{
    return _relation;
}

void ConnectionHandle::setRelation(Relation *newRelation)
{
    if (!newRelation) {
        qDebug() << "NULL";
    }
    _relation = newRelation;
    update();
}

QPointF ConnectionHandle::connectionPoint() const
{
    switch (_connectionEdge) {
    case Qt::TopEdge:
        return edgePoint(Qt::AlignTop);
    case Qt::RightEdge:
        return edgePoint(Qt::AlignRight);
    case Qt::BottomEdge:
        return edgePoint(Qt::AlignBottom);
    case Qt::LeftEdge:
        return edgePoint(Qt::AlignLeft);
    }
    return QPointF{};
}

QPointF ConnectionHandle::sceneConnectionPoint() const
{
    return mapToScene(connectionPoint());
}

int ConnectionHandle::index() const
{
    return _index;
}

void ConnectionHandle::setIndex(int newIndex)
{
    _index = newIndex;
}

GraphView::ConnectionType ConnectionHandle::connectionType() const
{
    return _connectionType;
}

void ConnectionHandle::setConnectionType(ConnectionType newConnectionType)
{
    _connectionType = newConnectionType;
}

Qt::Edge ConnectionHandle::connectionEdge() const
{
    return _connectionEdge;
}

void ConnectionHandle::setConnectionEdge(Qt::Edge newConnectionEdge)
{
    prepareGeometryChange();
    _connectionEdge = newConnectionEdge;
}

bool ConnectionHandle::isSelected() const
{
    return _isSelected;
}

void ConnectionHandle::setIsSelected(bool newIsSelected)
{
    _isSelected = newIsSelected;
    update();
}

}
