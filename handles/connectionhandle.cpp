#include "connectionhandle.h"
#include "core/painterhelper.h"
#include "scene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

namespace GraphView
{

ConnectionHandleWidget::ConnectionHandleWidget(QGraphicsItem *parent)
    :  AbstractRectWidget{parent}
{
    setSize(30, 20);
    setAcceptHoverEvents(true);
}

QRectF ConnectionHandleWidget::boundingRect() const
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

void ConnectionHandleWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
            paintFilledArrow(painter, Qt::RightEdge, rc);
        }
    } else {
//        painter->fillRect(QRectF{QPointF{0,0},_size }, Qt::yellow);
        paintArrow(painter, Qt::RightEdge, rc);
    }
}

QPointF ConnectionHandleWidget::handleScenePos() const
{
    return mapToScene(QPointF{0, size().height() / 2});
}

void ConnectionHandleWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    auto sc = qobject_cast<Scene *>(scene());
    if (!sc)
        return;

    QGraphicsItem::mousePressEvent(event);
}

void ConnectionHandleWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) { }
    QGraphicsItem::mouseMoveEvent(event);
}

RelationWidget *ConnectionHandleWidget::relation() const
{
    return _relation;
}

void ConnectionHandleWidget::setRelation(RelationWidget *newRelation)
{
    if (!newRelation) {
        qDebug() << "NULL";
    }
    _relation = newRelation;
    update();
}

QPointF ConnectionHandleWidget::connectionPoint() const
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

QPointF ConnectionHandleWidget::sceneConnectionPoint() const
{
    return mapToScene(connectionPoint());
}

int ConnectionHandleWidget::index() const
{
    return _index;
}

void ConnectionHandleWidget::setIndex(int newIndex)
{
    _index = newIndex;
}

ConnectionType ConnectionHandleWidget::connectionType() const
{
    return _connectionType;
}

void ConnectionHandleWidget::setConnectionType(ConnectionType newConnectionType)
{
    _connectionType = newConnectionType;
}

Qt::Edge ConnectionHandleWidget::connectionEdge() const
{
    return _connectionEdge;
}

void ConnectionHandleWidget::setConnectionEdge(Qt::Edge newConnectionEdge)
{
    prepareGeometryChange();
    _connectionEdge = newConnectionEdge;
}

bool ConnectionHandleWidget::isSelected() const
{
    return _isSelected;
}

void ConnectionHandleWidget::setIsSelected(bool newIsSelected)
{
    _isSelected = newIsSelected;
    update();
}

}
