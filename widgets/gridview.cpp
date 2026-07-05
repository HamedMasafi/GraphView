#include "gridview.h"

#include <QPainter>

namespace GraphView::Widgets {

GridView::GridView(QGraphicsItem *parent)
    : AbstractRectWidget{parent}
{
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void GridView::createBackgroundBrush()
{
    QImage img{static_cast<int>(m_gridWidth), static_cast<int>(m_gridHeight), QImage::Format_ARGB32};
    QPainter p{&img};
    p.setPen(QPen(m_gridColor));

    p.fillRect(QRect{0, 0, static_cast<int>(m_gridWidth), static_cast<int>(m_gridHeight)},
               Qt::white);

    switch (_gridType) {
    case GridType::Grid:
        p.drawLine(QPointF{0., 0.}, QPointF{m_gridWidth, 0.});
        p.drawLine(QPointF{0., 0.}, QPointF{0., m_gridHeight});
        break;
    case GridType::Dot:
        p.drawPoint(0, 0);
        break;
    case GridType::None:
        break;
    }
    p.end();
    QBrush brush{img};
    _backgroundBrush = brush;
}

void GridView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QRectF rc{0, 0, size().width(), size().height()};
    painter->fillRect(rc, m_backgroundColor);
    rc.adjust(m_margins.left(), m_margins.top(), -m_margins.right() + 1, -m_margins.bottom() + 1);
    painter->save();
    painter->setOpacity(m_opacity);
    painter->fillRect(rc, _backgroundBrush);
    painter->restore();
}

qreal GridView::gridWidth() const
{
    return m_gridWidth;
}

void GridView::setGridWidth(qreal newGridWidth)
{
    if (qFuzzyCompare(m_gridWidth, newGridWidth))
        return;
    m_gridWidth = newGridWidth;
    createBackgroundBrush();
    emit gridWidthChanged();
}

qreal GridView::gridHeight() const
{
    return m_gridHeight;
}

void GridView::setGridHeight(qreal newGridHeight)
{
    if (qFuzzyCompare(m_gridHeight, newGridHeight))
        return;
    m_gridHeight = newGridHeight;
    createBackgroundBrush();
    emit gridHeightChanged();
}

qreal GridView::opacity() const
{
    return m_opacity;
}

void GridView::setOpacity(qreal newOpacity)
{
    if (qFuzzyCompare(m_opacity, newOpacity))
        return;
    m_opacity = newOpacity;
    emit opacityChanged();
}

QMarginsF GridView::margins() const
{
    return m_margins;
}

void GridView::setMargins(const QMarginsF &newMargins)
{
    if (m_margins == newMargins)
        return;
    m_margins = newMargins;
    emit marginsChanged();
}

QColor GridView::backgroundColor() const
{
    return m_backgroundColor;
}

void GridView::setBackgroundColor(const QColor &newBackgroundColor)
{
    if (m_backgroundColor == newBackgroundColor)
        return;
    m_backgroundColor = newBackgroundColor;
    update();
    emit backgroundColorChanged();
}

QColor GridView::gridColor() const
{
    return m_gridColor;
}

void GridView::setGridColor(const QColor &newGridColor)
{
    if (m_gridColor == newGridColor)
        return;
    m_gridColor = newGridColor;
    createBackgroundBrush();
    emit gridColorChanged();
}

GridView::GridType GridView::gridType() const
{
    return _gridType;
}

void GridView::setGridType(GridType newGridType)
{
    if (_gridType == newGridType)
        return;
    _gridType = newGridType;
    createBackgroundBrush();
    update();
    emit gridTypeChanged();
}

} // namespace GraphView::Widgets
