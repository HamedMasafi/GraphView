#include "relation.h"
#include "arrowhead.h"

#include "core/pointshelper.h"
#include "event.h"
#include "abstractwidget.h"
#include "handles/connectionhandle.h"
#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QtMath>

namespace GraphView
{

namespace
{

// void alignPoints(QPointF &pt1, QPointF pt2, const QPointF &newPoint)
// {
//     switch(GraphView::Core::orientation(pt1,pt2)){
//     case Qt::Horizontal:
//         pt2.setX(newPoint.x());
//         pt1.setX(newPoint.x());
//         pt1.setY(newPoint.y());
//         break;
//     case Qt::Vertical:
//         pt2.setY(newPoint.y());
//         pt1.setX(newPoint.x());
//         pt1.setY(newPoint.y());
//         break;
//     }
// }

QPointF nearestPoint(const QLineF &line, const QPointF &point)
{
    QLineF perpendicLine{point, QPointF(point.x(), 0.0)};
    perpendicLine.setAngle(90.0 + line.angle());
    QPointF result;
    auto r = line.intersects(perpendicLine, &result);

    if (r == QLineF::NoIntersection)
        return QPointF{};

    return result;
}
}

qreal average(qreal n1, qreal n2)
{
    return (n1 + n2) / 2;
}

QPointF point(ConnectionHandleWidget *handle)
{
    return handle->mapToScene(handle->connectionPoint());
    //    switch (handle->connectionEdge()) {
    //    case Qt::TopEdge:
    //        return handle->sceneEdgePoint(Qt::AlignTop);
    //    case Qt::RightEdge:
    //        return handle->sceneEdgePoint(Qt::AlignRight);
    //    case Qt::BottomEdge:
    //        return handle->sceneEdgePoint(Qt::AlignBottom);
    //    case Qt::LeftEdge:
    //        return handle->sceneEdgePoint(Qt::AlignLeft);
    //    }
    //    qDebug() << "FUCK";
    //    return handle->sceneEdgePoint(Qt::AlignCenter);
}
QPointF point(const QRectF &rc, Qt::Alignment align = Qt::AlignCenter)
{
    qreal x;
    qreal y;

    if (align & Qt::AlignTop)
        y = rc.top();
    else if (align & Qt::AlignBottom)
        y = rc.bottom();
    else
        y = rc.top() + (rc.height() / 2);

    if (align & Qt::AlignLeft)
        x = rc.left();
    else if (align & Qt::AlignRight)
        x = rc.right();
    else
        x = rc.left() + (rc.width() / 2);

    return QPointF{x, y};
}
QPointF point(AbstractWidget *w, Qt::Alignment align = Qt::AlignCenter)
{
    return point(w->childRect(), align);
}

bool findIntersects(const QLineF &line, const QRectF &rc, QPointF &intersectPoint)
{
    QLineF::IntersectionType intersectionType;
    intersectionType = line.intersects(QLineF{rc.topLeft(), rc.topRight()}, &intersectPoint);
    if (intersectionType == QLineF::BoundedIntersection)
        return true;
    intersectionType = line.intersects(QLineF{rc.topRight(), rc.bottomRight()}, &intersectPoint);
    if (intersectionType == QLineF::BoundedIntersection)
        return true;
    intersectionType = line.intersects(QLineF{rc.topLeft(), rc.bottomLeft()}, &intersectPoint);
    if (intersectionType == QLineF::BoundedIntersection)
        return true;
    intersectionType = line.intersects(QLineF{rc.bottomLeft(), rc.bottomRight()}, &intersectPoint);
    if (intersectionType == QLineF::BoundedIntersection)
        return true;
    return false;
}

RelationWidget::RelationWidget(QGraphicsItem *parent)
    : QObject{}
    , QGraphicsItem{parent}
//    , _arrow{new ArrowHeadWidget{this}}
{

}

RelationWidget::RelationWidget(ConnectionHandleWidget *from, ConnectionHandleWidget *to)
    : QObject{}
    , QGraphicsItem{}
//    , _from(from)
//    , _to(to)
//    , _arrow{new ArrowHeadWidget{this}}
{
//    _poly.reset(point(from), point(to));

//    if (from)
//        from->setRelation(this);
//    if(to)
//        to->setRelation(this);
//    auto w = dynamic_cast<AbstractWidget *>(from->parentItem());
//    if (w)
//        connect(w, &AbstractWidget::moving, this, &RelationWidget::widget_moving);
//    w = dynamic_cast<AbstractWidget *>(to->parentItem());
//    if (w)
//        connect(w, &AbstractWidget::moving, this, &RelationWidget::widget_moving);
    //    setFlag(QGraphicsItem::ItemIsSelectable);

    setFrom(from);
    setTo(to);

}

RelationWidget::PointHolder &RelationWidget::from()
{
    return _from;
}

void RelationWidget::setFrom(const PointHolder &newFrom)
{
    if (_from == newFrom)
        return;

    prepareGeometryChange();

    if (_from.handle()) {
        _from.handle()->setRelation(nullptr);
        disconnect(_from.handle(), nullptr, this, nullptr);
    }
    _from = newFrom;

    if (newFrom.handle()) {
        newFrom.handle()->setRelation(this);
        auto w = dynamic_cast<AbstractWidget *>(newFrom.handle()->parentItem());
        if (w)
            connect(w, &AbstractWidget::moving, this, &RelationWidget::widget_moving);
    }
    qDebug() << "Set from" << newFrom;
    _poly.reset(_from, _to);
}

RelationWidget::PointHolder &RelationWidget::to()
{
    return _to;
}

void RelationWidget::setTo(const PointHolder &newTo)
{
    if (_to == newTo)
        return;

    prepareGeometryChange();
    if (_to.handle()) {
        _to.handle()->setRelation(nullptr);
        disconnect(_to.handle(), nullptr, this, nullptr);
    }
    _to = newTo;

    if (newTo.handle()) {
        newTo.handle()->setRelation(this);
        auto w = dynamic_cast<AbstractWidget *>(newTo.handle()->parentItem());
        if (w)
            connect(w, &AbstractWidget::moving, this, &RelationWidget::widget_moving);
    }

    _poly.reset(_from, _to);
}

void RelationWidget::reset(const PointHolder &newFrom, const PointHolder &newTo)
{
    qDebug() << "reset to" << newFrom << newTo;
    if (_from.handle()) {
        disconnect(_from.handle(), nullptr, this, nullptr);
    }
    if (_to.handle()) {
        disconnect(_to.handle(), nullptr, this, nullptr);
    }
    prepareGeometryChange();
    _poly.reset(newFrom, newTo);

    _from = newFrom;
    _to = newTo;

    AbstractWidget *w;
    if (newFrom.handle()) {
        w = dynamic_cast<AbstractWidget *>(newFrom.handle()->parentItem());
        if (w)
            connect(w, &AbstractWidget::moving, this, &RelationWidget::widget_moving);
    }
    if (newTo.handle()) {
        w = dynamic_cast<AbstractWidget *>(newTo.handle()->parentItem());
        if (w)
            connect(w, &AbstractWidget::moving, this, &RelationWidget::widget_moving);
    }
}

//void RelationWidget::setTo(const QPointF &pt)
//{
//    prepareGeometryChange();
//    _toPoint = pt;
////    setTo(nullptr);

//    _poly.reset(_from, pt);
//    relocateLines();
////    _arrow->setPoint(pt);
////    update();
//}

void RelationWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    //    painter->setBrush(Qt::green);
    //    painter->drawRect(boundingRect());
    // constexpr qreal arrowSize = 20;
    // QPointF arrowPoint;
    // double angle{};
//    painter->setPen(_isActive ? _activePen : _pen);
    painter->setBrush(isActive() ? Qt::green : Qt::gray);
    //    if (_mode == Line) {
    //        //    painter->drawLine(0, 0, _size.width() - 1, _size.height() - 1);
    //        angle = std::atan2(-_line.dy(), _line.dx());

    //        arrowPoint = _line.p2();
    //        QPointF arrowP1 = _line.p2() + QPointF(sin(angle - M_PI / 3) * arrowSize, cos(angle - M_PI / 3) * arrowSize);
    //        QPointF arrowP2 = _line.p2() + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize, cos(angle - M_PI + M_PI / 3) * arrowSize);

    //        painter->drawLine(_line);

    //    } else {
    //        for (auto i = 0; i < _poly.size() - 1; i++) {
    //            painter->drawLine(_poly.at(i), _poly.at(i + 1));
    //        }

    //        arrowPoint = _poly.last();
    //        auto &beforLast = _poly.at(_poly.count() - 2);
    //        angle = std::atan2(arrowPoint.y() - beforLast.y(), arrowPoint.x() - beforLast.x());
    //    }

    //    QPointF arrowP1 = arrowPoint + QPointF(sin(angle - M_PI / 3) * arrowSize, cos(angle - M_PI / 3) * arrowSize);
    //    QPointF arrowP2 = arrowPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize, cos(angle - M_PI + M_PI / 3) * arrowSize);
    //    _arrowHead.clear();
    //    _arrowHead << arrowPoint << arrowP1 << arrowP2;

//    painter->drawPolygon(_arrowHead);
    _poly.paint(painter, _pen, _activePen);
    //    qDebug() << _poly;
}

QRectF RelationWidget::boundingRect() const
{
    if (!_poly.size())
        return QRectF{0, 0, 0, 0};
    qreal extra = (_pen.width() + 20) / 2.0;
    return _poly.boundingRect().normalized().adjusted(-extra, -extra, extra, extra);
}

void RelationWidget::relocateLines()
{
//    if (_poly.size() - 1 > _lines.size())
//        while (_poly.size() - 1 != _lines.size())
//            _lines << new QGraphicsLineItem{this};

//    if (_poly.size() - 1 < _lines.size())
//        while (_poly.size() - 1 != _lines.size())
//            delete _lines.takeLast();

//    for (auto i = 0; i < _poly.size() - 2; i++)
    //        _lines.at(i)->setLine(_poly.at(i).x(), _poly.at(i).y(), _poly.at(i + 1).x(), _poly.at(i + 1).y());
}

void RelationWidget::normalizePoints()
{
    for (int i = 0; i < _poly.size(); ++i) {

    }
}

void RelationWidget::reset()
{
    if (_to.handle()) {
        _to.handle()->setRelation(nullptr);
        disconnect(_to.handle(), nullptr, this, nullptr);
    }
    if (_from.handle()) {
        _from.handle()->setRelation(nullptr);
        disconnect(_from.handle(), nullptr, this, nullptr);
    }
    _to = QPointF{0, 0};
    _from = QPointF{0, 0};
}

void RelationWidget::setPoly(const MultiLineF &newPoly)
{
    prepareGeometryChange();
    _poly = newPoly;
    update();
}

MultiLineF RelationWidget::poly() const
{
    return _poly;
}

qreal RelationWidget::distance(const QPointF &pt) const
{
    qreal bd{0};
    bool isFirstLoop{true};
    for (auto i = 0; i < _poly.size() - 1; i++) {
        auto dist = distance(pt, QLineF{_poly.at(i), _poly.at(i + 1)});
        if (Q_UNLIKELY(isFirstLoop)) {
            bd = dist;
            isFirstLoop = false;
        } else if (dist < bd) {
            bd = dist;
        }
    }
    return bd;
}

bool RelationWidget::intersect(const QPointF &pt, QPointF *intersectPoint, qreal maxDist) const
{
    for (auto i = 0; i < _poly.size() - 1; i++) {
        QLineF line{_poly.at(i), _poly.at(i + 1)};
        auto dist = distance(pt, line);
        if (dist <= maxDist) {
            if (intersectPoint)
                *intersectPoint = nearestPoint(line, pt);
            return true;
        }
    }
    return false;
}

bool RelationWidget::isActive() const
{
    return _isActive;
}

void RelationWidget::setActive(bool newIsActive)
{
    _isActive = newIsActive;
    update();
}

void RelationWidget::processMousePressEvent(const QPointF &pt)
{
    //    if (_poly.size() > 1) {
    //        for (auto i = 0; i < _poly.size() - 1; i++) {
    //            dist(event->scenePos(), QLineF{_poly.at(i), _poly.at(i + 1)});
    //        }
    //    }
    _poly.findNearest(pt);
    //        event->accept();
    //        QGraphicsItem::mousePressEvent(event);
    //        grabMouse();
}

void RelationWidget::processMouseMoveEvent(const QPointF &pt)
{
    prepareGeometryChange();
    _poly.moveNearest(pt);
    relocateLines();

    //        QGraphicsItem::mouseMoveEvent(event);
}

void RelationWidget::processMouseReleaseEvent(const QPointF &pt)
{
    Q_UNUSED(pt)
    prepareGeometryChange();
    _poly.normalize();
    relocateLines();
    //        ungrabMouse();
    //        QGraphicsItem::mouseReleaseEvent(event);
}

void RelationWidget::widget_moving(MoveEvent *)
{
    prepareGeometryChange();
    _poly.reset(_from, _to);
}

void RelationWidget::calculateArrow()
{
    // constexpr qreal arrowSize = 20;

    // if (_poly.size() < 2)
    //     return;

    // QPointF from = _poly.at(_poly.size() - 2);
    // QPointF to = _poly.last();

    // double angle = std::atan2(-(to.y() - from.y()), to.x() - from.x());
    // QPointF arrowP1 = to + QPointF(sin(angle - M_PI / 3) * arrowSize, cos(angle - M_PI / 3) * arrowSize);
    // QPointF arrowP2 = to + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize, cos(angle - M_PI + M_PI / 3) * arrowSize);
//    _arrowHead.clear();
//    _arrowHead << to << arrowP1 << arrowP2;
}

bool isInOneLine(const QPointF &pt1, const QPointF &pt2, const QPointF &pt3)
{
    if (qFuzzyCompare(pt1.x(), pt2.x()) && qFuzzyCompare(pt2.x(), pt3.x()))
        return true;
    if (qFuzzyCompare(pt1.y(), pt2.y()) && qFuzzyCompare(pt2.y(), pt3.y()))
        return true;
    return false;
}
void MultiLineF::normalize()
{
    for (int i = 0; i < _data.size() - 2; ++i) {
        if (isInOneLine(_data.at(i), _data.at(i + 1), _data.at(i + 2))) {
            _data.removeAt(i + 1);
        }
    }
    //    for (int i = 0; i < size() - 1; ++i) {
    //        for (int j = size() - 1; j > i; --j) {
    //            if (qFuzzyCompare(at(i).x(), at(j).x()) || qFuzzyCompare(at(i).y(), at(j).y())) {
    //                for (int n = i; n < j; ++n) {
    //                    removeAt(i + 1);
    //                }
    //                break;
    //            }
    //        }
    //    }
}

RelationWidget::PointHolder::PointHolder(const QPointF &pt)
    : _pt{pt}
    , _handle{nullptr}
{
}

RelationWidget::PointHolder::PointHolder(ConnectionHandleWidget *handle)
    : _handle{handle}
{
}

RelationWidget::PointHolder RelationWidget::PointHolder::operator=(const QPointF &pt)
{
    PointHolder ret{pt};
    return ret;
}

RelationWidget::PointHolder RelationWidget::PointHolder::operator=(ConnectionHandleWidget *handle){
    return PointHolder{handle};
}

bool RelationWidget::PointHolder::operator==(ConnectionHandleWidget *handle){
    return _handle == handle;
}

bool RelationWidget::PointHolder::operator==(const RelationWidget::PointHolder &other){
    if (_handle || other.handle())
        return _handle == other._handle;
    return _pt == other._pt;
}

bool RelationWidget::PointHolder::operator==(const QPointF &pt){
    return !_handle && _pt == pt;
}

QPointF RelationWidget::PointHolder::point() const {
    if (_handle)
        return _handle->sceneConnectionPoint();
    return _pt;
}

RelationWidget::PointHolder::operator QPointF()
{
    return point();
}

RelationWidget::PointHolder::operator const QPointF() const
{
    return point();
}

QJsonObject RelationWidget::PointHolder::saveJson() const
{
    QJsonObject o;
    if (handle()){
        o.insert("type", "handle");
        o.insert("handle", handle()->objectName());
    }else{
        o.insert("type", "point");
        o.insert("point", QJsonObject{{"x", _pt.x()}, {"y", _pt.y()}});
    }
    return o;
}

bool RelationWidget::PointHolder::loadJson(const QJsonObject &object)
{
    Q_UNUSED(object)
    return true;
}

ConnectionHandleWidget *RelationWidget::PointHolder::handle() const
{
    return _handle;
}

}

QDebug operator<<(QDebug d, const GraphView::RelationWidget::PointHolder &p) {
    auto pt = p.point();
    if (p.handle()){
        d.nospace() << "PointHolder(" << (int *)p.handle() << " " << pt.x() << "," << pt.y() << ")";
    }else{
        d.nospace() << "PointHolder(nullptr " << pt.x() << "," << pt.y() << ")";
    }
    return d;
}
