#include "polylineitem.h"

#include "commands/movehandlecommand.h"
#include "graphviewglobal.h"
#include "handles/polylinehandle.h"
#include "scene.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace GraphView {

PolylineItemWidget::PolylineItemWidget(QGraphicsItem *parent)
    : AbstractWidget(parent)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
}

PolylineItemWidget::~PolylineItemWidget()
{
    for (auto &handle : _handles) {
        scene()->removeItem(handle);
        handle->deleteLater();
    }
}

void PolylineItemWidget::addNode(const QPointF &pt)
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

void PolylineItemWidget::insertNode(SizeType index, const QPointF &pt)
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

void PolylineItemWidget::removeNode(SizeType index)
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

void PolylineItemWidget::setNode(SizeType index, const QPointF &pt)
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

QPointF PolylineItemWidget::nodeAt(SizeType index) const
{
    return _poly.value(index);
}

void PolylineItemWidget::normalize()
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

void PolylineItemWidget::createHandles()
{
    auto scene = this->scene();
    while (_handles.size() > _poly.size()) {
        auto handle = _handles.takeFirst();
        if (Q_LIKELY(scene))
            scene->removeItem(handle);
        delete handle;
    }
    while (_handles.size() < _poly.size()) {
        auto handle = new PolylineHandle;
        handle->setZValue(Z::handle);
        if (Q_LIKELY(scene))
            scene->addItem(handle);
        _handles << handle;

        // connect(handle, &PolylineHandle::moving, this, &PolylineItemWidget::handleMoving);
        // connect(handle, &PolylineHandle::moved, this, &PolylineItemWidget::handleMoved);
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

QVariant PolylineItemWidget::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == GraphicsItemChange::ItemSceneHasChanged) {
        auto s = scene();
        if (Q_LIKELY(s))
            for (auto &h : _handles)
                s->addItem(h);
    }
    return AbstractWidget::itemChange(change, value);
}

QList<PolylineHandle *> PolylineItemWidget::handles() const
{
    return _handles;
}

void PolylineItemWidget::setHandleSize(int size)
{
    for (auto &handle : _handles)
        handle->setSize(size);
}

void PolylineItemWidget::setHandlesBackgroundColor(const QColor &color)
{
    for (auto &handle : _handles)
        handle->setBackgroundColor(color);
}

void PolylineItemWidget::setHandlesBorderColor(const QColor &color)
{
    for (auto &handle : _handles)
        handle->setBorderColor(color);
}

void PolylineItemWidget::setHandlesSelectedBackgroundColor(const QColor &color)
{
    for (auto &handle : _handles)
        handle->setSelectedBackgroundColor(color);
}

void PolylineItemWidget::setHandlesSelectedBorderColor(const QColor &color)
{
    for (auto &handle : _handles)
        handle->setSelectedBorderColor(color);
}

void PolylineItemWidget::setPoly(const QPolygonF &newPoly)
{
    _poly = newPoly;
    normalize();
    createHandles();
    update();
}

void PolylineItemWidget::handleMoving(QPointF *pt)
{
    auto handle = qobject_cast<PolylineHandle *>(sender());
    if (!handle)
        return;

    _poly[handle->index()] = *pt;
    normalize();
    update();
}

void PolylineItemWidget::handleMoved(QPointF from, QPointF to)
{
    auto handle = qobject_cast<PolylineHandle *>(sender());
    if (!handle)
        return;
    auto cmd = new MoveHandleCommand{this, handle->index(), from, to};
    dynamic_cast<Scene *>(scene())->undoStack()->push(cmd);
}

int PolylineItemWidget::minNodesCount() const
{
    return _minNodesCount;
}

void PolylineItemWidget::setMinNodesCount(int newMinNodesCount)
{
    _minNodesCount = newMinNodesCount;
}

QPolygonF PolylineItemWidget::poly() const
{
    return _poly;
}

SizeType PolylineItemWidget::count() const
{
    return _poly.size();
}

int PolylineItemWidget::maxNodesCount() const
{
    return _maxNodesCount;
}

void PolylineItemWidget::setMaxNodesCount(int newMaxNodesCount)
{
    _maxNodesCount = newMaxNodesCount;
}

QRectF PolylineItemWidget::boundingRect() const
{
    const qreal pad = m_lineWidth * 0.5 + 2.0;
    QRectF rc{QPointF{0, 0}, _cachedRect.size()};
    return rc.adjusted(-pad, -pad, pad, pad);
}

// QPainterPath PolylineItemWidget::shape() const
// {
//     QPainterPath path;
//     if (_poly.size() < 2)
//         return path;

//     path.addPolygon(_poly);

//     QPainterPathStroker stroker;
//     stroker.setWidth(m_lineWidth + 6.0); // easier mouse hit
//     return stroker.createStroke(path);
// }

void PolylineItemWidget::paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *)
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

qreal PolylineItemWidget::lineWidth() const
{
    return m_lineWidth;
}

void PolylineItemWidget::setLineWidth(qreal newLineWidth)
{
    if (qFuzzyCompare(m_lineWidth, newLineWidth))
        return;
    m_lineWidth = newLineWidth;
    update();
    emit lineWidthChanged();
}

QColor PolylineItemWidget::lineColor() const
{
    return m_lineColor;
}

void PolylineItemWidget::setLineColor(const QColor &newLineColor)
{
    if (m_lineColor == newLineColor)
        return;
    m_lineColor = newLineColor;
    update();
    emit lineColorChanged();
}

} // namespace GraphView
