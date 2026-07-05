#pragma once

#include <QList>
#include <QPen>
#include <QPointF>

#include "core/graphview_global.h"

class QPainter;
namespace GraphView
{

class MultiLineF// : public QList<QPointF>
{
public:

    void findNearest(const QPointF &pt);
    void moveNearest(const QPointF &pt);
    void setPoint(int index, qreal x, qreal y);
    void setPointX(int index, qreal x);
    void setPointY(int index, qreal y);
    void reset(const QPointF &from, const QPointF &to);
    void paint(QPainter *painter, const QPen &pen, const QPen &activePen);
    void normalize();

    SizeType size() const
    {
        return _data.size();
    }
    const QPointF &at(SizeType i) const noexcept
    {
        return _data.at(i);
    }
    void removeAt(SizeType i) { _data.removeAt(i); }

    const QPointF &first() const
    {
        return _data.first();
    }
    const QPointF &last() const
    {
        return _data.last();
    }
    void append(const QPointF &pt) { _data.append(pt); }

    QRectF boundingRect() const
    {
        QRectF rc{_data.first(), _data.first()};
        for (auto &pt : _data) {
            rc.setLeft(qMin(rc.left(), pt.x()));
            rc.setRight(qMax(rc.right(), pt.x()));

            rc.setTop(qMin(rc.top(), pt.y()));
            rc.setBottom(qMax(rc.bottom(), pt.y()));
        }
        return rc;
    }
    class iterator
    {
    private:
        int _index{0};
        MultiLineF *_parent;
        QLineF _line;

    public:
        explicit iterator(MultiLineF *parent)
            : _parent{parent}
        {
        }
        iterator()
            : _parent{nullptr}
        {
        }
        bool hasNext() const
        {
            return _parent && _index < _parent->_data.size() - 1;
        }
        inline bool operator==(const iterator &o) const noexcept { return hasNext() == o.hasNext(); }
        inline bool operator!=(const iterator &o) const noexcept { return hasNext() != o.hasNext(); }
        inline iterator &operator++()
        {
            _index++;
//            _line.setP1(_parent->_data.at(_index));
//            _line.setP2(_parent->_data.at(_index + 1));
            return *this;
        }
        inline const QLineF operator*() const
        {
            return QLineF{_parent->_data.at(_index), _parent->_data.at(_index + 1)};
        }
    };
    friend class iterator;

    inline QList<QPointF>::Iterator begin() { return _data.begin(); }
    inline QList<QPointF>::Iterator end() { return _data.end(); }

    inline iterator lineBegin() { return iterator(this); }
    inline iterator lineEnd() { return iterator(); }
    void setData(const QList<QPointF> &newData);

private:
    int _nearIndex{-1};
    QList<QPointF> _data;
};

}
