#include "pointshelper.h"
#include "qmath.h"

namespace GraphView::Core
{

void getLine(const QLineF &line, double &a, double &b, double &c)
{
    a = line.y1() - line.y2();
    b = line.x2() - line.x1();
    c = line.x1() * line.y2() - line.x2() * line.y1();
}

double distance(const QPointF &point, const QLineF &line)
{
    double a, b, c;
    getLine(line, a, b, c);
    return std::abs(a * point.x() + b * point.y() + c) / sqrt(a * a + b * b);
}

bool isInOneLine(const QPointF &pt1, const QPointF &pt2, const QPointF &pt3)
{
    if (qFuzzyCompare(pt1.x(), pt2.x()) && qFuzzyCompare(pt2.x(), pt3.x()))
        return true;
    if (qFuzzyCompare(pt1.y(), pt2.y()) && qFuzzyCompare(pt2.y(), pt3.y()))
        return true;
    return false;
}

bool equal(const QPointF &pt1, const QPointF &pt2)
{
    return qFuzzyCompare(pt1.x(), pt2.x()) && qFuzzyCompare(pt1.y(), pt2.y());
}

Qt::Orientation orientation(const QPointF &pt1, const QPointF pt2)
{
    if (qFuzzyCompare(pt1.x(), pt2.x()))
        return Qt::Horizontal;
    return Qt::Vertical;
}

double distance(const QPointF &pt1, const QPointF &pt2)
{
    auto x = pt2.x() - pt1.x();
    auto y = pt2.y() - pt1.y();
    return qSqrt(x * x + y * y);
}
}
