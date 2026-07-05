#pragma once

#include <QUndoCommand>

namespace GraphView
{
class Scene;
class AbstractWidget;

class RemoveWidgetCommand : public QUndoCommand
{
public:
    RemoveWidgetCommand(Scene *scene, AbstractWidget *item);
    RemoveWidgetCommand(Scene *scene, QList<AbstractWidget *> items);

    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    QList<AbstractWidget *> _items;
    QList<QUuid> _ids;
};

}
