#pragma once

#include <QEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QObject>

class QGraphicsScene;

namespace GraphView
{
namespace Core{
class ResizeEvent;
class MoveEvent;
}
namespace Widgets
{
class ResizeHandle;
class AbstractWidget;

enum ResizeDirectionFlag { None = 0, Top = 1, Left = 2, Right = 4, Bottom = 8 };
Q_DECLARE_FLAGS(ResizeDirection, ResizeDirectionFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(ResizeDirection)

class ResizeHandle : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 10 };

    int type() const override
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }
    ResizeHandle(qreal radius = 15, QGraphicsItem *parent = 0);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QPointF centerPoint() const;
    QPointF lastPoint() const;

    ResizeDirection resizeDirection() const;
    void setResizeDirection(ResizeDirection value);

    void setScale(qreal scale);

private:
    qreal _radius;
    bool mousePressed;
    QPointF _lastPoint;
    ResizeDirection m_resizeDirection;

signals:
    void moving(QPointF point);
    void moved();
};

class WidgetResizer : public QObject
{
    Q_OBJECT

public:
    WidgetResizer(QGraphicsScene *parent);
    ~WidgetResizer();

    void setVisible();
    void setVisible(bool visible);
    void setActiveItem(AbstractWidget *item);
    void setHandlesOnItem(QRectF rc);
    qreal scale();
    void setScale(qreal scale);
    void refreshHandles();
    void refresh();

    QList<AbstractWidget *> selectedWidgets();
    void clear();
    void clear(bool unselectWidgets);
    void addWidget(AbstractWidget *Widget);

    void refreshWidgets();

    bool isWidgetSelected(AbstractWidget *Widget);

    QList<AbstractWidget *> selectedWidgets() const;

private:
    QGraphicsScene parent;

    ResizeHandle *resizerTL, *resizerT, *resizerTR;
    ResizeHandle *resizerL, *resizerR;
    ResizeHandle *resizerBL, *resizerB, *resizerBR;
    QList<ResizeHandle *> handles;

    QGraphicsScene *parentScene;

    qreal m_scale;
    qreal _marginTop;
    qreal _marginLeft;
    qreal _gridSize;

    QRectF selectionRect;
    QRectF resizeRect;
    QHash<AbstractWidget *, QRectF> _widgetRects;

    QList<AbstractWidget *> _selectedWidgets;

    void setResezeHandlePos(ResizeHandle *handle, QPointF pos);
    void setResezeHandlePos(ResizeHandle *handle, QPointF pos1, QPointF pos2);

    void proccessNewRect(QRectF rc);

signals:
    void pointGridNeeded(GraphView::Core::MoveEvent *);
    void sizeGridNeeded(GraphView::Core::ResizeEvent *);
    void resized();

public slots:
    void handleMoving(QPointF point);
    void widgetMoving(GraphView::Core::MoveEvent *event);
};

}
}
