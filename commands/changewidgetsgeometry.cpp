#include "changewidgetsgeometry.h"

#include <QCoreApplication>
#include "widgets/abstractrectwidget.h"
#include <scene.h>

namespace GraphView::Commands {

ChangeWidgetsGeometry::ChangeWidgetsGeometry(Scene *scene)
    : QUndoCommand{}
    , _scene{scene}
{
    setText(QCoreApplication::translate("Commands::MoveWidget", "Move widget"));
}

void ChangeWidgetsGeometry::undo()
{
    for (auto &di : _data) {
        auto w = _scene->widgetById(di.id);
        w->setRect(di.oldPos);
        Q_EMIT w->moved(di.oldPos.topLeft());
    }
}

void ChangeWidgetsGeometry::redo()
{
    for (auto &di : _data) {
        auto w = _scene->widgetById(di.id);
        w->setRect(di.newPos);
        Q_EMIT w->moved(di.newPos.topLeft());
    }
}

void ChangeWidgetsGeometry::addWidget(Widgets::AbstractWidget *widget,
                                      const QPointF &from,
                                      const QPointF &to)
{
    _data << WidgetData{widget->id(), QRectF{from, widget->size()}, QRectF{to, widget->size()}};
}

void ChangeWidgetsGeometry::addWidget(Widgets::AbstractWidget *widget,
                                      const QRectF &from,
                                      const QRectF &to)
{
    _data << WidgetData{widget->id(), from, to};
}
} // namespace GraphView::Commands
