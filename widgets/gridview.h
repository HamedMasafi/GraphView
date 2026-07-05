#pragma once

#include <QBrush>

#include "abstractrectwidget.h"

namespace GraphView
{

class GridViewWidget : public AbstractRectWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged FINAL)
    Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor NOTIFY gridColorChanged FINAL)
    Q_PROPERTY(qreal gridWidth READ gridWidth WRITE setGridWidth NOTIFY gridWidthChanged FINAL)
    Q_PROPERTY(qreal gridHeight READ gridHeight WRITE setGridHeight NOTIFY gridHeightChanged FINAL)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)
    Q_PROPERTY(QMarginsF margins READ margins WRITE setMargins NOTIFY marginsChanged FINAL)
    Q_PROPERTY(GridType gridType READ gridType WRITE setGridType NOTIFY gridTypeChanged FINAL)

public:
    enum class GridType { None, Dot, Grid };

    GridViewWidget(QGraphicsItem *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    qreal gridWidth() const;
    void setGridWidth(qreal newGridWidth);

    qreal gridHeight() const;
    void setGridHeight(qreal newGridHeight);

    qreal opacity() const;
    void setOpacity(qreal newOpacity);

    QMarginsF margins() const;
    void setMargins(const QMarginsF &newMargins);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    QColor gridColor() const;
    void setGridColor(const QColor &newGridColor);

    GridType gridType() const;
    void setGridType(GridType newGridType);

signals:
    void colorChanged();
    void gridWidthChanged();
    void gridHeightChanged();
    void opacityChanged();
    void marginsChanged();

    void backgroundColorChanged();

    void gridColorChanged();

    void gridTypeChanged();

private:
    GridType _gridType{GridType::None};
    QBrush _backgroundBrush;
    void createBackgroundBrush();
    qreal m_gridWidth{20};
    qreal m_gridHeight{20};
    qreal m_opacity{1};
    QMarginsF m_margins;

    QColor m_backgroundColor{Qt::white};
    QColor m_gridColor{Qt::black};
};
}
