#include "movewidgets.h"
#include "qundostack.h"
#include "widgets/abstractwidget.h"

#include <QApplication>
#include <scene.h>

namespace GraphView::Commands {
MoveWidgets::MoveWidgets(Scene *scene)
    : QUndoCommand{}
    , _scene{scene}
{
    setText(QCoreApplication::translate("Commands::MoveWidget", "Move widget"));
}

void MoveWidgets::undo()
{
    for (auto &di : _data) {
        auto w = _scene->widgetById(di.id);
        w->setPos(di.oldPos);
        Q_EMIT w->moved(di.oldPos);
    }
}

void MoveWidgets::redo()
{
    for (auto &di : _data) {
        auto w = _scene->widgetById(di.id);
        w->setPos(di.newPos);
        Q_EMIT w->moved(di.newPos);
    }
}

void MoveWidgets::addWidget(const QUuid &id, const QPointF &from, const QPointF &to)
{
    _data << WidgetData{from, to, id};
}

} // namespace GraphView::Commands
