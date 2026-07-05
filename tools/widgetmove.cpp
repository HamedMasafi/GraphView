#include "widgetmove.h"
#include "commands/movewidgets.h"
#include "event.h"
#include "scene.h"
#include "widgets/abstractwidget.h"

#include <QGraphicsSceneEvent>
#include <QKeyEvent>

namespace GraphView::Tools
{
WidgetMove::WidgetMove(Scene *scene)
    : AbstractTool{scene}
{
}

bool WidgetMove::accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
    _selectedWidget = dynamic_cast<Widgets::AbstractWidget *>(item);
    return _selectedWidget;
}

void WidgetMove::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton) {
        _clickPos = _selectedWidget->mapFromScene(mouseEvent->scenePos());
        //        _scene->selectWidget(_selectedWidget);
        _wasMoved = false;
        resizeRect = _selectedWidget->childRect();
        _lastPos = _selectedWidget->pos();
    }
}

void WidgetMove::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton) {
        auto oldPos = _selectedWidget->mapFromScene(mouseEvent->buttonDownScenePos(Qt::LeftButton));
        _selectedWidget->setPos(_scene->snapPoint(mouseEvent->scenePos() - _clickPos));
        Core::MoveEvent e{_selectedWidget->pos(), oldPos};
        Q_EMIT _selectedWidget->moving(&e);
        //    setHandlesOnItem(_selectedWidget->childRect());
    }
}

void WidgetMove::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent)
    // auto cmd = new Commands::MoveWidgets{_selectedWidget, _lastPos};
    // _scene->pushCommand(cmd);

    // _selectedWidget->ungrabMouse();
    // Q_EMIT finished();
}

}
