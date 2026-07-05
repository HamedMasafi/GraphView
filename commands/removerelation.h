#pragma once

#include <QUndoCommand>

namespace GraphView
{
class Scene;
class RelationWidget;

class RemoveRelationCommand : public QUndoCommand
{
public:
    RemoveRelationCommand();
    RemoveRelationCommand(Scene *scene, RelationWidget *relation);

    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    RelationWidget *_relation;

};

}
