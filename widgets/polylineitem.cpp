#include "polylineitem.h"

#include "commands/movehandlecommand.h"
#include "graphviewglobal.h"
#include "handles/polylinehandle.h"
#include "scene.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace GraphView::Widgets {

PolylineItem::PolylineItem(QGraphicsItem *parent)
    : AbstractWidget(parent)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
}

PolylineItem::~PolylineItem()
{
    for (auto &handle : _handles) {
        scene()->removeItem(handle);
        handle->deleteLater();
    }
}

void PolylineItem::addNode(const QPointF &pt)
{
    // QPointF local = mapFromScene(pt);
    qDebug() << Q_FUNC_INFO << _poly.count() << _maxNodesCount;
    if (_maxNodesCount && _poly.count() >= _maxNodesCount)
        return;

    prepareGeometryChange();
    _poly << pt;
    normalize();
    update();
    createHandles();
}

void PolylineItem::insertNode(SizeType index, const QPointF &pt)
{
    qDebug() << Q_FUNC_INFO << _poly.count() << _maxNodesCount;

    if (_maxNodesCount && _poly.count() >= _maxNodesCount)
        return;

    prepareGeometryChange();
    _poly.insert(index, pt);
    normalize();
    update();
    createHandles();
}

void PolylineItem::removeNode(SizeType index)
{
    if (index < 0 || index >= _poly.size())
        return;

    if (_minNodesCount && _minNodesCount >= _poly.size())
        return;

    prepareGeometryChange();
    _poly.remove(index);
    normalize();
    update();
    createHandles();
}

void PolylineItem::setNode(SizeType index, const QPointF &pt)
{
    prepareGeometryChange();
    _poly[index] = pt;
    auto &handle = _handles[index];
    handle->setPos(pt);
    QString text;
    emit tooltipText(handle, &text);
    handle->setToolTip(text);
    normalize();
    update();
}

QPointF PolylineItem::nodeAt(SizeType index) const
{
    return _poly.value(index);
}

void PolylineItem::normalize()
{
    if (_poly.isEmpty())
        return;

    qreal minX = _poly[0].x();
    qreal minY = _poly[0].y();
    qreal maxX = minX;
    qreal maxY = minY;

    for (const auto &p : std::as_const(_poly)) {
        minX = std::min(minX, p.x());
        minY = std::min(minY, p.y());
        maxX = std::max(maxX, p.x());
        maxY = std::max(maxY, p.y());
    }

    const QPointF offset(minX, minY);

    // shift all points to start at (0,0)
    // for (auto &p : _poly)
    //     p -= offset;

    // move the item to preserve visual position
    // setPos(pos() + offset);

    setPos(minX, minY);
    setSize(maxX - minX, maxY - minY);
    _cachedRect = QRectF(minX, minY, maxX - minX, maxY - minY);
    // qDebug() << "_cachedRect" << _cachedRect;
}

void PolylineItem::createHandles()
{
    auto scene = this->scene();
    while (_handles.size() > _poly.size()) {
        auto handle = _handles.takeFirst();
        if (Q_LIKELY(scene))
            scene->removeItem(handle);
        delete handle;
    }
    while (_handles.size() < _poly.size()) {
        auto handle = new Handles::PolylineHandle;
        handle->setZValue(Z::handle);
        if (Q_LIKELY(scene))
            scene->addItem(handle);
        _handles << handle;

        // connect(handle, &PolylineHandle::moving, this, &PolylineItem::handleMoving);
        // connect(handle, &PolylineHandle::moved, this, &PolylineItem::handleMoved);
    }
    for (auto i = 0; i < _poly.size(); ++i) {
        _handles[i]->setIndex(i);
        _handles[i]->setPoly(this);
        _handles[i]->setPos(_poly[i]);

        QString text;
        emit tooltipText(_handles[i], &text);
        _handles[i]->setToolTip(text);
    }
}

QVariant PolylineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == GraphicsItemChange::ItemSceneHasChanged) {
        auto s = scene();
        if (Q_LIKELY(s))
            for (auto &h : _handles)
                s->addItem(h);
    }
    return AbstractWidget::itemChange(change, value);
}

QList<Handles::PolylineHandle *> PolylineItem::handles() const
{
    return _handles;
}

void PolylineItem::setHandleSize(int size)
{
    for (auto &handle : _handles)
        handle->setSize(size);
}

void PolylineItem::setHandlesBackgroundColor(const QColor &color)
{
    for (auto &handle : _handles)
        handle->setBackgroundColor(color);
}

void PolylineItem::setHandlesBorderColor(const QColor &color)
{
    for (auto &handle : _handles)
        handle->setBorderColor(color);
}

void PolylineItem::setHandlesSelectedBackgroundColor(const QColor &color)
{
    for (auto &handle : _handles)
        handle->setSelectedBackgroundColor(color);
}

void PolylineItem::setHandlesSelectedBorderColor(const QColor &color)
{
    for (auto &handle : _handles)
        handle->setSelectedBorderColor(color);
}

void PolylineItem::setPoly(const QPolygonF &newPoly)
{
    _poly = newPoly;
    normalize();
    createHandles();
    update();
}

void PolylineItem::handleMoving(QPointF *pt)
{
    auto handle = qobject_cast<Handles::PolylineHandle *>(sender());
    if (!handle)
        return;

    _poly[handle->index()] = *pt;
    normalize();
    update();
}

void PolylineItem::handleMoved(QPointF from, QPointF to)
{
    auto handle = qobject_cast<Handles::PolylineHandle *>(sender());
    if (!handle)
        return;
    auto cmd = new Commands::MoveHandleCommand{this, handle->index(), from, to};
    dynamic_cast<Scene *>(scene())->undoStack()->push(cmd);
}

int PolylineItem::minNodesCount() const
{
    return _minNodesCount;
}

void PolylineItem::setMinNodesCount(int newMinNodesCount)
{
    _minNodesCount = newMinNodesCount;
}

QPolygonF PolylineItem::poly() const
{
    return _poly;
}

SizeType PolylineItem::count() const
{
    return _poly.size();
}

int PolylineItem::maxNodesCount() const
{
    return _maxNodesCount;
}

void PolylineItem::setMaxNodesCount(int newMaxNodesCount)
{
    _maxNodesCount = newMaxNodesCount;
}

QRectF PolylineItem::boundingRect() const
{
    const qreal pad = m_lineWidth * 0.5 + 2.0;
    QRectF rc{QPointF{0, 0}, _cachedRect.size()};
    return rc.adjusted(-pad, -pad, pad, pad);
}

// QPainterPath PolylineItem::shape() const
// {
//     QPainterPath path;
//     if (_poly.size() < 2)
//         return path;

//     path.addPolygon(_poly);

//     QPainterPathStroker stroker;
//     stroker.setWidth(m_lineWidth + 6.0); // easier mouse hit
//     return stroker.createStroke(path);
// }

void PolylineItem::paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *)
{
    Q_UNUSED(opt)
    if (_poly.size() < 2)
        return;

    p->save();
    p->setRenderHint(QPainter::Antialiasing);

    QPen pen(m_lineColor, m_lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    // if (opt->state & QStyle::State_Selected)
    // pen.setColor(Qt::red);

    p->translate(-_cachedRect.x(), -_cachedRect.y());
    p->setPen(pen);
    p->drawPolyline(_poly);
    p->restore();
}

qreal PolylineItem::lineWidth() const
{
    return m_lineWidth;
}

void PolylineItem::setLineWidth(qreal newLineWidth)
{
    if (qFuzzyCompare(m_lineWidth, newLineWidth))
        return;
    m_lineWidth = newLineWidth;
    update();
    emit lineWidthChanged();
}

QColor PolylineItem::lineColor() const
{
    return m_lineColor;
}

void PolylineItem::setLineColor(const QColor &newLineColor)
{
    if (m_lineColor == newLineColor)
        return;
    m_lineColor = newLineColor;
    update();
    emit lineColorChanged();
}

} // namespace GraphView::Widgets
