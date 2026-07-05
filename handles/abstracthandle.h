#pragma once

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QObject>

#include "core/graphview_global.h"

namespace GraphView::Handles
{

struct AbstractHandleData{};

class AbstractHandle : public QGraphicsObject
{
    Q_OBJECT

public:
    static qreal defaultSize;
    static QColor defaultColor;
    AbstractHandle(QGraphicsItem *parent = nullptr);

    int type() const override
    {
        return UserType + 145;
    }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setSize(qreal newSize);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    QColor borderColor() const;
    void setBorderColor(const QColor &newBorderColor);

    QColor selectedBackgroundColor() const;
    void setSelectedBackgroundColor(const QColor &newSelectedBackgroundColor);

    QColor selectedBorderColor() const;
    void setSelectedBorderColor(const QColor &newSelectedBorderColor);

    qsizetype index() const;
    void setIndex(qsizetype newIndex);

    AbstractHandleData *handleData() const;
    void setHandleData(AbstractHandleData *newHandleData);

protected:

Q_SIGNALS:
    void removeRequested();
    void moving(QPointF *point);
    void moved(QPointF from, QPointF to);

private:
    qreal _size{10};

    QColor _backgroundColor{150, 150, 255};
    QColor _borderColor{Qt::blue};

    QColor _selectedBackgroundColor{Qt::yellow};
    QColor _selectedBorderColor{Qt::darkYellow};

    AbstractHandleData *_handleData{};
    // QBrush _brush;
    // QBrush _selectedBrush;

    qsizetype _index{-1};
    bool mousePressed;
    QPointF _lastPoint;
    QPointF _clickPos;
};

}
