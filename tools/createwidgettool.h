#pragma once

#include "abstracttool.h"

class QGraphicsRectItem;
namespace GraphView::Tools
{
class CreateWidget : public AbstractTool
{
    Q_OBJECT

public:
    CreateWidget(Scene *scene);

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
