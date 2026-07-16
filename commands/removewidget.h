#pragma once

#include <QUndoCommand>

namespace GraphView
{
class Scene;

namespace Widgets
{
class AbstractWidget;
}

namespace Commands
{
class RemoveWidget : public QUndoCommand
{
public:
    RemoveWidget(Scene *scene, Widgets::AbstractWidget *item);
    RemoveWidget(Scene *scene, QList<Widgets::AbstractWidget *> items);

    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    QList<Widgets::AbstractWidget *> _items;
    QList<QUuid> _ids;
};

}
}
