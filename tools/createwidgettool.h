#pragma once

#include "abstracttool.h"

class QGraphicsRectItem;
namespace GraphView
{
class CreateWidgetTool : public AbstractTool
{
    Q_OBJECT

public:
    CreateWidgetTool(Scene *scene);

    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;
    QString text() const override;
    ToolType toolType() const override;

Q_SIGNALS:
    void createObjectRequested(QRectF rect);

private:
    QGraphicsRectItem *_dragRect;

};
}
