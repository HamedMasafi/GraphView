#pragma once

#include <QRectF>

class QPainter;
namespace GraphView
{

void paintArrow(QPainter *painter, Qt::Edge direction, const QRectF &rc);
void paintFilledArrow(QPainter *painter, Qt::Edge direction, const QRectF &rc);
}
