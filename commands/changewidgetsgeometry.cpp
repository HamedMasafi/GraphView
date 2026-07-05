#include "changewidgetsgeometry.h"

#include <QCoreApplication>
#include "widgets/abstractrectwidget.h"
#include <scene.h>

namespace GraphView {

ChangeWidgetsGeometryCommand::ChangeWidgetsGeometryCommand(Scene *scene)
    : QUndoCommand{}
    , _scene{scene}
{
    setText(QCoreApplication::translate("ChangeWidgetsGeometryCommand", "Move widget"));
}

void ChangeWidgetsGeometryCommand::undo()
{
    for (auto &di : _data) {
        auto w = _scene->widgetById(di.id);
        w->setRect(di.oldPos);
        Q_EMIT w->moved(di.oldPos.topLeft());
    }
}

void ChangeWidgetsGeometryCommand::redo()
{
    for (auto &di : _data) {
        auto w = _scene->widgetById(di.id);
        w->setRect(di.newPos);
        Q_EMIT w->moved(di.newPos.topLeft());
    }
}

void ChangeWidgetsGeometryCommand::addWidget(AbstractWidget *widget,
                                      const QPointF &from,
                                      const QPointF &to)
{
    _data << WidgetData{widget->id(), QRectF{from, widget->size()}, QRectF{to, widget->size()}};
}

void ChangeWidgetsGeometryCommand::addWidget(AbstractWidget *widget,
                                      const QRectF &from,
                                      const QRectF &to)
{
    _data << WidgetData{widget->id(), from, to};
}
} // namespace GraphView
