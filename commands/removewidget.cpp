#include "removewidget.h"
#include <QCoreApplication>
#include "scene.h"
#include <widgets/abstractwidget.h>

namespace GraphView::Commands
{

RemoveWidget::RemoveWidget(Scene *scene, Widgets::AbstractWidget *item)
    : RemoveWidget{scene, QList<Widgets::AbstractWidget *>{item}}
{
}

RemoveWidget::RemoveWidget(Scene *scene, QList<Widgets::AbstractWidget *> items)
    : QUndoCommand{}
    , _scene{scene}
    , _items{items}
{
    for (Widgets::AbstractWidget* item: items)
        _ids << item->id();
    setText(QCoreApplication::translate("Commands::RemoveWidget", "Remove widget"));
}

void RemoveWidget::undo()
{
    for (auto &item : _items)
        _scene->addWidget(item);
}

void RemoveWidget::redo()
{
    for (auto &item : _items)
        _scene->removeWidget(item);
}

}
