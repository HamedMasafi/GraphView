#include "removewidget.h"
#include <QCoreApplication>
#include "scene.h"
#include <widgets/abstractwidget.h>

namespace GraphView
{

RemoveWidgetCommand::RemoveWidgetCommand(Scene *scene, AbstractWidget *item)
    : RemoveWidgetCommand{scene, QList<AbstractWidget *>{item}}
{
}

RemoveWidgetCommand::RemoveWidgetCommand(Scene *scene, QList<AbstractWidget *> items)
    : QUndoCommand{}
    , _scene{scene}
    , _items{items}
{
    for (AbstractWidget* item: items)
        _ids << item->id();
    setText(QCoreApplication::translate("RemoveWidgetCommand", "Remove widget"));
}

void RemoveWidgetCommand::undo()
{
    for (auto &item : _items)
        _scene->addWidget(item);
}

void RemoveWidgetCommand::redo()
{
    for (auto &item : _items)
        _scene->removeWidget(item);
}

}
