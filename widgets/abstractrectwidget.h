#pragma once

#include "abstractwidget.h"

#include <QGraphicsItemGroup>

namespace GraphView::Widgets
{

class AbstractRectWidget : public AbstractWidget
{
    Q_OBJECT

    // Q_PROPERTY(QSizeF minimumSize READ minimumSize WRITE setMinimumSize NOTIFY minimumSizeChanged FINAL)
    // Q_PROPERTY(QSizeF maximumSize READ maximumSize WRITE setMaximumSize NOTIFY maximumSizeChanged FINAL)

public:
    enum { Type = UserType + 2 };

    int type() const override
    {
        return Type;
    }
    AbstractRectWidget(QGraphicsItem *parent = nullptr);
    // QRectF boundingRect() const override
    // {
    //     return QRectF{0, 0, _size.width() - 1, _size.height() - 1};
    // }

    // void setRect(const QRectF &rc)
    // {
    //     setPos(rc.topLeft());
    //     setSize(rc.size());
    // }
    // QSizeF size() const { return _size; }
    // void setSize(const QSizeF &newSize)
    // {
    //     prepareGeometryChange();
    //     _size = newSize;
    // }
    // void setSize(qreal x, qreal y) { setSize(QSizeF{x, y}); }
    // QRectF childRect() const { return QRectF(pos(), size()); }



signals:
    // void minimumSizeChanged();
    // void maximumSizeChanged();

protected:
    // QSizeF _size;
private:
    // QSizeF m_minimumSize;
    // QSizeF m_maximumSize;
};

}
