#pragma once

#include "abstracttool.h"
#include "itemresizer.h"
#include <QGraphicsEllipseItem>
#include <event.h>

namespace GraphView{

class OldResizeHandle :public QGraphicsEllipseItem
{
public:
    OldResizeHandle();

    ResizeDirection resizeDirection() const;
    void setResizeDirection(ResizeDirection newResizeDirection);

private:
    ResizeDirection m_resizeDirection;

};

class ResizeWidgetTool : public AbstractTool
{
    Q_OBJECT
public:
    ResizeWidgetTool(Scene *scene);

    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;
    ToolType toolType() const override;

private Q_SLOTS:
    void widget_moving(MoveEvent *);
    void widget_destroyed(QObject * = nullptr);

private:
    void initHandles();
    void setHandlesOnItem(AbstractWidget *widget);
    void setVisible(bool visible);

    // ResizeHandle from anonymous namespace in cpp
    QGraphicsEllipseItem *resizerTL, *resizerT, *resizerTR;
    QGraphicsEllipseItem *resizerL, *resizerR;
    QGraphicsEllipseItem *resizerBL, *resizerB, *resizerBR;
    QList<QGraphicsEllipseItem *> handles;
    QGraphicsEllipseItem *_selectedHandle{nullptr};
    AbstractWidget *_selectedWidget{nullptr};

    void setResezeHandlePos(QGraphicsEllipseItem *handle, QPointF pos);
    void setResezeHandlePos(QGraphicsEllipseItem *handle, QPointF pos1, QPointF pos2);

    qreal m_scale{1};
    QRectF resizeRect;

};
}
