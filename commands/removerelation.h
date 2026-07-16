#pragma once

#include <QUndoCommand>

namespace GraphView
{
class Scene;

namespace Widgets
{
class Relation;
}
namespace Commands
{
class RemoveRelation : public QUndoCommand
{
public:
    RemoveRelation();
    RemoveRelation(Scene *scene, Widgets::Relation *relation);

    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    Widgets::Relation *_relation;

};

}
}
