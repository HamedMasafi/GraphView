#include "movewidgetsbyarrowkeys.h"

#include <QKeyEvent>
#include <QDebug>

#include "scene.h"
#include "widgets/abstractwidget.h"

namespace GraphView {

MoveWidgetsByArrowKeysTool::MoveWidgetsByArrowKeysTool(Scene *scene)
    : AbstractTool{scene}
{}

void MoveWidgetsByArrowKeysTool::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
        moveSelectedWidgets(static_cast<Qt::Key>(event->key()), 50);
    if (event->modifiers() == Qt::ShiftModifier)
        moveSelectedWidgets(static_cast<Qt::Key>(event->key()), 10);
    else
        moveSelectedWidgets(static_cast<Qt::Key>(event->key()), 1);
}

void MoveWidgetsByArrowKeysTool::moveSelectedWidgets(Qt::Key key, qreal distance)
{
    auto widgets = _scene->selectedWidgets();

    for (AbstractWidget *w : std::as_const(widgets)) {
        switch (key) {
        case Qt::Key_Left:
            w->setPos(w->pos().x() - distance, w->pos().y());
            break;
        case Qt::Key_Right:
            w->setPos(w->pos().x() + distance, w->pos().y());
            break;
        case Qt::Key_Up:
            w->setPos(w->pos().x(), w->pos().y() - distance);
            break;
        case Qt::Key_Down:
            w->setPos(w->pos().x(), w->pos().y() + distance);
            break;

        default:
            break;
        }
    }
}

QString MoveWidgetsByArrowKeysTool::text() const
{
    return "Move widgets by arrow keys";
}

AbstractTool::ToolType MoveWidgetsByArrowKeysTool::toolType() const
{
    return ToolType::AlwaysListener;
}

} // namespace GraphView
