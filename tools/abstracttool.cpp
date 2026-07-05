#include "abstracttool.h"

#include "graphview.h"
#include "scene.h"
#include "scene_p.h"

#include <QAction>

namespace GraphView
{
AbstractTool::AbstractTool(Scene *scene)
    : QObject{scene}
    , _scene{scene}
{
}

AbstractTool::AbstractTool(View *view)
    : QObject{view}
    , _view{view}
{
}

void AbstractTool::activate()
{
    if (_action)
        _action->setChecked(true);
    if (_view)
        _view->setCursor(_cursor);
}

void AbstractTool::deactivate()
{
    if (_action)
        _action->setChecked(false);
    if (_view)
        _view->setCursor(Qt::ArrowCursor);
}

bool AbstractTool::accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
    Q_UNUSED(item)
    return false;
}

void AbstractTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
}

void AbstractTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
}

void AbstractTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
}

void AbstractTool::mouseDoubleClicked(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
}

void AbstractTool::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    Q_UNUSED(event)
}

void AbstractTool::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}

void AbstractTool::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}

QIcon AbstractTool::icon() const
{
    return QIcon{};
}

QString AbstractTool::text() const
{
    return QString{};
}

AbstractTool::ToolType AbstractTool::toolType() const
{
    return ToolType::RequireActivation;
}

Scene *AbstractTool::scene() const
{
    return _scene;
}

void AbstractTool::setScene(Scene *newScene)
{
    sceneChanged(newScene);
    _scene = newScene;
}

void AbstractTool::sceneChanged(Scene *newScene)
{
    Q_UNUSED(newScene)
}

View *AbstractTool::view() const
{
    return _view;
}

void AbstractTool::setView(View *newView)
{
    _view = newView;
}

void AbstractTool::finish()
{
    _isFinished = true;
    emit finished();
}

void AbstractTool::setTempTool(bool enabled)
{
    _scene->dd->tempTool = enabled ? this : nullptr;
}

bool AbstractTool::isFinished() const
{
    return _isFinished;
}

QAction *AbstractTool::action()
{
    if (!_action) {
        _action = new QAction{this};
        _action->setText(text());
        _action->setCheckable(true);
        connect(_action, &QAction::triggered, this, &AbstractTool::actionTriggered);
    }
    return _action;
}

void AbstractTool::actionTriggered()
{
    _scene->setTool(this);
}

}
