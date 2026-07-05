#pragma once

#include <QUndoCommand>

namespace GraphView
{
class Scene;
class ConnectionHandle;
class RelationWidget;

class AddRelationCommand : public QUndoCommand
{
public:
    AddRelationCommand(Scene *scene, ConnectionHandle *from, ConnectionHandle *to);

public:
    void undo() override;
    void redo() override;

    RelationWidget *relation() const;

private:
    Scene *_scene;
    RelationWidget *_relation{nullptr};
    ConnectionHandle *_from;
    ConnectionHandle *_to;
};

}
