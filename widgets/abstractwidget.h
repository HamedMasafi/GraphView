#pragma once

#include <QGraphicsObject>
#include <QUuid>
#include "itemresizer.h"
#include "core/graphview_global.h"

class QGraphicsLinearLayout;

namespace GraphView {

class ScenePrivate;

namespace Commands {
class AddWidget;
}

class Scene;
namespace Core {
class MoveEvent;
class ResizeEvent;
} // namespace Core
namespace Widgets {
class ConnectionHandle;

class AbstractWidget : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QSizeF minimumSize READ minimumSize WRITE setMinimumSize NOTIFY minimumSizeChanged FINAL)
    Q_PROPERTY(QSizeF maximumSize READ maximumSize WRITE setMaximumSize NOTIFY maximumSizeChanged FINAL)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged FINAL USER true)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged FINAL USER true)
public:
    AbstractWidget(QGraphicsItem *parent = nullptr);


    enum { Type = UserType + 1 };

    int type() const override { return Type; }
    void setRect(const QRectF &rc);
    QSizeF size() const;
    void setSize(const QSizeF &newSize);
    void setSize(qreal x, qreal y);
    void setWidth(qreal w);
    void setHeight(qreal h);

    QRectF boundingRect() const override;

    QRectF childRect() const;

    virtual void saveState(QJsonObject &map) const;
    virtual void restoreState(const QJsonObject &map);

    QSizeF minimumSize() const;
    void setMinimumSize(const QSizeF &newMinimumSize);

    QSizeF maximumSize() const;
    void setMaximumSize(const QSizeF &newMaximumSize);

    QPointF centerPoint() const;
    QPointF sceneCenterPoint() const;
    void setCenterPos(const QPointF &pt);

    QPointF edgePoint(Qt::Alignment align = Qt::AlignCenter) const;
    QPointF sceneEdgePoint(Qt::Alignment align = Qt::AlignCenter) const;

    qreal height() const;

    qreal width() const;

    QUuid id() const;

    QPair<qreal, qreal> aspectRatio() const;
    void setAspectRatio(const QPair<qreal, qreal> &newAspectRatio);

    ResizeDirection resizeDirection() const;
    void setResizeDirection(ResizeDirection newResizeDirection);

    WidgetResizeMode resizeMode() const;
    void setResizeMode(WidgetResizeMode newResizeMode);

    QString className() const;

protected:
    //    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    //    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void resized() {}

Q_SIGNALS:
    void selectedChanged();
    void doubleClicked();
    void moved(QPointF lastPos);
    void moving(GraphView::Core::MoveEvent *);
    void resizing(GraphView::Core::ResizeEvent *);

    void minimumSizeChanged();

    void maximumSizeChanged();

    void heightChanged();

    void widthChanged();

protected:
    QSizeF _size{40, 20};
    bool wasMoved{false};
    QPointF lastPos;
    QString _className;

private:
    void initLayout();

    QSizeF m_minimumSize;
    QSizeF m_maximumSize;
    QPair<qreal, qreal> _aspectRatio;
    QUuid _uuid;
    ResizeDirection _resizeDirection;
    WidgetResizeMode _resizeMode;

    friend class GraphView::ScenePrivate;
    friend class Commands::AddWidget;
};

} // namespace Widgets
} // namespace GraphView
