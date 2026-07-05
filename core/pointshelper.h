#pragma once

#include <QLineF>
#include <QPointF>

namespace GraphView::Core
{

bool equal(const QPointF &pt1, const QPointF &pt2);
double distance(const QPointF &pt1, const QPointF &pt2);
double distance(const QPointF &point, const QLineF &line);
bool isInOneLine(const QPointF &pt1, const QPointF &pt2, const QPointF &pt3);
Qt::Orientation orientation(const QPointF &pt1, const QPointF pt2);

}
