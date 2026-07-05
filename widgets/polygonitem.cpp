#include "polygonitem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace GraphView::Widgets
{

PolygonItem::PolygonItem(QGraphicsItem *parent)
    : PolylineItem(parent)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
}





void PolygonItem::paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *)
{
    if (_poly.size() < 2)
        return;

    p->save();
    p->setRenderHint(QPainter::Antialiasing);
    // p->fillRect(boundingRect(), Qt::yellow);
    QPen pen(Qt::blue, m_lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    if (opt->state & QStyle::State_Selected)
        pen.setColor(Qt::red);

    p->translate(-_cachedRect.x(), -_cachedRect.y());
    p->setPen(pen);
    p->drawPolygon(_poly);
    p->restore();
}

} // namespace
