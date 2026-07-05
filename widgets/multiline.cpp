#include "multiline.h"
#include "core/pointshelper.h"

#include <QLineF>
#include <QDebug>
#include <QPainter>

namespace GraphView
{

namespace {
qreal average(qreal n1, qreal n2)
{
    return (n1 + n2) / 2;
}
}

void MultiLineF::findNearest(const QPointF &pt)
{
    if (_data.size() == 1) {
        _nearIndex = 0;
        return;
    }
    int n = 0;
    qreal bd{};
    bool isFirstLoop{true};
    for (auto i = 0; i < _data.size() - 1; i++) {
        auto dist = distance(pt, QLineF{at(i), at(i + 1)});
        if (Q_UNLIKELY(isFirstLoop)) {
            bd = dist;
            isFirstLoop = false;
            n = 0;
        } else if (dist < bd) {
            bd = dist;
            n = i;
        }
    }
    if (n == 0) {
        _data.prepend(_data.first());
        n = 1;
    }
    if (n == _data.size() - 2) {
        _data.append(_data.last());
        n--;
    }
    _nearIndex = n;
    qDebug() << Q_FUNC_INFO << n;
}

void MultiLineF::moveNearest(const QPointF &pt)
{
    if (Q_UNLIKELY(_nearIndex == -1))
        return;

    qDebug() << "moved";
    if (_data.at(_nearIndex).x() == _data.at(_nearIndex + 1).x()) {
        setPointX(_nearIndex, pt.x());
        setPointX(_nearIndex + 1, pt.x());
    } else {
        setPointY(_nearIndex, pt.y());
        setPointY(_nearIndex + 1, pt.y());
    }
}

void MultiLineF::setPoint(int index, qreal x, qreal y)
{
    _data[index] = QPointF(x, y);
}

void MultiLineF::setPointX(int index, qreal x)
{
    setPoint(index, x, _data.at(index).y());
}

void MultiLineF::setPointY(int index, qreal y)
{
    setPoint(index, _data.at(index).x(), y);
}

void MultiLineF::reset(const QPointF &from, const QPointF &to)
{
    _data.clear();
    if (equal(from, to)) {
        _data.append(from);
        _data.append(to);
        return;
    }

    _data.append(from);
    _data.append(QPointF{average(to.x(), from.x()), from.y()});
    _data.append(QPointF{average(to.x(), from.x()), to.y()});
    _data.append(to);
}

void MultiLineF::paint(QPainter *painter, const QPen &pen, const QPen &activePen)
{
    for (auto i = 0; i < _data.size() - 1; i++) {
        if (_nearIndex == i)
            painter->setPen(activePen);
        else
            painter->setPen(pen);
        painter->drawLine(_data.at(i), _data.at(i + 1));
    }
}

void MultiLineF::setData(const QList<QPointF> &newData)
{
    _data = newData;
}

void foo() {
//    MultiLineF l;
//    MultiLineIterator it(l);
//    for (auto &line : it) { }
}
}
