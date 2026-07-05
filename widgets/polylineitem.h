#pragma once

#include <QPolygonF>
#include "abstractwidget.h"

namespace GraphView {

namespace Handles {
class PolylineHandle;
}

namespace Widgets {
class PolylineItem : public AbstractWidget
{
    Q_OBJECT

    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged FINAL)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged FINAL)

public:
    enum { Type = UserType + 6 };

    int type() const override { return Type; }
    explicit PolylineItem(QGraphicsItem *parent = nullptr);
    ~PolylineItem();

    void addNode(const QPointF &pt);
    void insertNode(SizeType index, const QPointF &pt);
    void removeNode(SizeType index);
    void setNode(SizeType index, const QPointF &pt);
    QPointF nodeAt(SizeType index) const;

    QRectF boundingRect() const override;
    // QPainterPath shape() const override;
    void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *widget) override;

    qreal lineWidth() const;
    void setLineWidth(qreal newLineWidth);

    QColor lineColor() const;
    void setLineColor(const QColor &newLineColor);

    int maxNodesCount() const;
    void setMaxNodesCount(int newMaxNodesCount);

    int minNodesCount() const;
    void setMinNodesCount(int newMinNodesCount);

    SizeType count() const;

    QPolygonF poly() const;

    void setPoly(const QPolygonF &newPoly);

    QList<Handles::PolylineHandle *> handles() const;

    void setHandlesBackgroundColor(const QColor &color);
    void setHandlesBorderColor(const QColor &color);
    void setHandlesSelectedBackgroundColor(const QColor &color);
    void setHandlesSelectedBorderColor(const QColor &color);
    void setHandleSize(int size);

private Q_SLOTS:
    void handleMoving(QPointF *pt);
    void handleMoved(QPointF from, QPointF to);

Q_SIGNALS:
    void lineWidthChanged();
    void lineColorChanged();
    void tooltipText(GraphView::Handles::PolylineHandle *handle, QString *text);

private:
    void normalize();
    void createHandles();

protected:
    QPolygonF _poly;
    qreal m_lineWidth{2.0};
    QColor m_lineColor{Qt::black};
    QList<Handles::PolylineHandle *> _handles;
    QRectF _cachedRect;
    int _maxNodesCount{0};
    int _minNodesCount{0};
    SizeType _count{0};

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

} // namespace Widgets
} // namespace GraphView
