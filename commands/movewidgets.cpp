#include "movewidgets.h"
#include "qundostack.h"
#include "widgets/abstractwidget.h"

#include <QApplication>
#include <scene.h>

namespace GraphView {

MoveWidgetsCommand::MoveWidgetsCommand(Scene *scene)
    : QUndoCommand{}
    , _scene{scene}
{
    setText(QCoreApplication::translate("MoveWidgetsCommand", "Move widget"));
}

void MoveWidgetsCommand::undo()
{
    for (auto &di : _data) {
        auto w = _scene->widgetById(di.id);
        w->setPos(di.oldPos);
        Q_EMIT w->moved(di.oldPos);
    }
}

void MoveWidgetsCommand::redo()
{
    for (auto &di : _data) {
        auto w = _scene->widgetById(di.id);
        w->setPos(di.newPos);
        Q_EMIT w->moved(di.newPos);
    }
}

void MoveWidgetsCommand::addWidget(const QUuid &id, const QPointF &from, const QPointF &to)
{
    _data << WidgetData{from, to, id};
}

} // namespace GraphView
