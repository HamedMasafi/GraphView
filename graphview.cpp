#include <QAction>
#include <QDebug>

#include "graphview.h"
#include "commands/addwidget.h"
#include "commands/removewidget.h"
#include "scene.h"
#include "tools/abstracttool.h"
#include "tools/connectwidgets.h"
#include "tools/createwidgettool.h"
#include "tools/magnifier.h"
#include "widgets/abstractwidget.h"
#include "handles/connectionhandle.h"

namespace GraphView
{
View::View(QWidget *parent)
    : QGraphicsView{parent}
    // , _scene{new GraphView::Scene}
{
    setScene(new GraphView::Scene);

    setRenderHint(QPainter::Antialiasing, true);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void View::scene_createObjectRequested(QRectF rect)
{
    Q_UNUSED(rect)
    // if (rect.width() > 10 && rect.height() > 10) {
    //     auto widget = _creator();
    //     auto cmd = new AddWidgetCommand(_scene, rect, widget);
    //     _scene->undoStack()->push(cmd);
    // }
}

void View::scene_widgetMoved(AbstractWidget *widget, QPointF lastPos, QPointF newPos)
{
    Q_UNUSED(widget)
    Q_UNUSED(lastPos)
    Q_UNUSED(newPos)
    //    auto cmd = new MoveWidgetsCommand{widget, lastPos};
    //    _scene->undoStack()->push(cmd);
}

void View::scene_widgetDoubleClicked(AbstractWidget *widget)
{
    Q_UNUSED(widget)
}

void View::scene_widgetsConnectionRequested(ConnectionHandleWidget *from, ConnectionHandleWidget *to)
{
    Q_UNUSED(from)
    Q_UNUSED(to)
    qDebug() << "*************************" << Q_FUNC_INFO;
    //    auto relation = new RelationWidget{from, to};
    //    from->setRelation(relation);
    //    to->setRelation(relation);
    //    auto cmd = new AddRelationCommand{_scene, relation};
    //   _scene->undoStack()->push(cmd);
}

void View::scene_removeRequested(AbstractWidget *widget)
{
    auto cmd = new RemoveWidgetCommand{_scene, widget};
    _scene->undoStack()->push(cmd);
}

View::Mode View::mode() const
{
    return _mode;
}

void View::setMode(Mode newMode)
{
    _mode = newMode;
    switch (newMode) {
    case Mode::Pointer:
        setDragMode(QGraphicsView::NoDrag);
//        _scene->setMode(Scene::Mode::Normal);
//        setCursor(Qt::ArrowCursor);
//        _scene->setTool(nullptr);
        _scene->setTool(nullptr/*_selectTool*/);
        break;
    case Mode::Hand:
        setDragMode(QGraphicsView::ScrollHandDrag);
//        _scene->setMode(Scene::Mode::Normal);
//        setCursor(Qt::ArrowCursor);
        break;
    case Mode::Rect:
       _scene->setTool(_createWidgetTool);

//        setDragMode(QGraphicsView::NoDrag);
//        _scene->setMode(Scene::Mode::DragZone);
//        setCursor(Qt::CrossCursor);
        break;
    case Mode::Relation:
//        setDragMode(QGraphicsView::NoDrag);
//        _scene->setTool(_relationTool);
        break;
    }
}

void View::createWidget(CreatorFunction fn)
{
    _creator = fn;
    setMode(Mode::Rect);
}

void View::createTools()
{
}

void View::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
}

int View::zoomLevel() const
{
    return _zoomLevel;
}

void View::setZoomLevel(int newZoomLevel)
{
    if (newZoomLevel < 0 || newZoomLevel > 200)
        return;

    _zoomLevel = newZoomLevel;

    int zoomValue;
    if(newZoomLevel < 100)
        zoomValue = (newZoomLevel * 75 / 100) + 25;
    else
        zoomValue = ((newZoomLevel - 100) * 3) + 100;

    qreal scale = zoomValue / 100.00;
    QTransform t;
    t.scale(scale, scale);
    setTransform(t);
}

QAction *View::registerTool(AbstractTool *tool)
{
    tool->setView(this);
    _tools.insert(tool->metaObject()->className(), tool);

    if (tool->toolType() == AbstractTool::ToolType::RequireActivation) {
        auto action = new QAction{this};
        action->setText(tool->text());
        connect(action, &QAction::triggered, [tool, this]() {
            setTool(tool);
        });
        _actions << action;
        return action;
    }
    return nullptr;
}

QUndoStack *View::undoStack() const
{
    return _scene->undoStack();
}

Scene *View::scene() const
{
    return _scene;
}

void View::setScene(Scene *newScene)
{
    if (_scene == newScene)
        return;

    if (_scene) {
        disconnect(_scene, nullptr, this, nullptr);
    }
    QGraphicsView::setScene(newScene);
    _scene = newScene;
    if (_scene) {
        connect(_scene, &Scene::createObjectRequested, this, &View::scene_createObjectRequested);
        connect(_scene, &Scene::widgetMoved, this, &View::scene_widgetMoved);
        connect(_scene, &Scene::widgetDoubleClicked, this, &View::scene_widgetDoubleClicked);
        connect(_scene, &Scene::widgetsConnectionRequested, this, &View::scene_widgetsConnectionRequested);
        connect(_scene, &Scene::removeRequested, this, &View::scene_removeRequested);
        _scene->setView(this);
    }

}

AbstractTool *View::tool() const
{
    return _scene->activeTool();
}

void View::setTool(AbstractTool *tool)
{
    _scene->setTool(tool);
}

void View::createDefaultCommands()
{
    _scene->setView(this);
    _relationTool = registerTool<ConnectWidgetsTool>();
    _createWidgetTool = registerTool<CreateWidgetTool>();
    _magnifierTool = registerTool<MagnifierTool>();

    connect(_createWidgetTool, &GraphView::CreateWidgetTool::createObjectRequested, this, &View::scene_createObjectRequested);
}

}
