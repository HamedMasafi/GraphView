#pragma once

#include "abstracttool.h"
#include "itemresizer.h"
#include <QGraphicsEllipseItem>
#include <event.h>


namespace GraphView{

namespace Impl
{
class ResizeHandle;
};

namespace Widgets{

class ResizeHandle2 :public QGraphicsEllipseItem
{
public:
    ResizeHandle2();

    ResizeDirection resizeDirection() const;
    void setResizeDirection(ResizeDirection newResizeDirection);

private:
    ResizeDirection m_resizeDirection;

};

}

namespace Tools{

class ResizeWidget : public AbstractTool
{
    Q_OBJECT
public:
    ResizeWidget(Scene *scene);

    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;
    ToolType toolType() const override;

private Q_SLOTS:
    void widget_moving(GraphView::Core::MoveEvent *);
    void widget_destroyed(QObject * = nullptr);

private:
    void initHandles();
    void setHandlesOnItem(Widgets::AbstractWidget *widget);
    void setVisible(bool visible);

    Impl::ResizeHandle *resizerTL, *resizerT, *resizerTR;
    Impl::ResizeHandle *resizerL, *resizerR;
    Impl::ResizeHandle *resizerBL, *resizerB, *resizerBR;
    QList<Impl::ResizeHandle *> handles;
    Impl::ResizeHandle *_selectedHandle{nullptr};
    Widgets::AbstractWidget *_selectedWidget{nullptr};

    void setResezeHandlePos(Impl::ResizeHandle *handle, QPointF pos);
    void setResezeHandlePos(Impl::ResizeHandle *handle, QPointF pos1, QPointF pos2);

    qreal m_scale{1};
    QRectF resizeRect;

};
}
}
