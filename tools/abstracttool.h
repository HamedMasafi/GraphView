#pragma once

#include <QCursor>
#include <QIcon>
#include <QObject>
#include <QPointF>

class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QKeyEvent;
class QAction;

namespace GraphView
{
class Scene;
class View;

namespace Widgets
{
class AbstractWidget;
}

namespace Tools
{
class AbstractTool : public QObject
{
    Q_OBJECT
public:
    enum class ToolType {
        RequireActivation,
        AlwaysListener
    };
    Q_ENUM(ToolType)

    explicit AbstractTool(Scene *scene);
    explicit AbstractTool(View *view);

    virtual void activate();
    virtual void deactivate();

    virtual bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mousePressed(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseDoubleClicked(QGraphicsSceneMouseEvent *mouseEvent);

    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual QIcon icon() const;
    virtual QString text() const;
    virtual ToolType toolType() const;

    Scene *scene() const;
    void setScene(Scene *newScene);
    virtual void sceneChanged(Scene *newScene);

    View *view() const;
    void setView(View *newView);

    bool isFinished() const;

    QAction *action();

private Q_SLOTS:
    void actionTriggered();

Q_SIGNALS:
    void finished();

protected:
    void finish();

    void setTempTool(bool enabled);

    bool _isFinished{false};

    QAction *_action{nullptr};
    QCursor _cursor{Qt::ArrowCursor};
    Scene *_scene{nullptr};
    View *_view{nullptr};
};

}
}
