#pragma once

#include "widgets/multiline.h"
#include <QUndoCommand>

namespace GraphView
{
class Scene;
class RelationWidget;
class ConnectionHandle;

class ChangeRelationCommand : public QUndoCommand
{
public:
    ChangeRelationCommand(Scene *scene, RelationWidget *relation, ConnectionHandle *oldHandle, ConnectionHandle *newHandle);

    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    RelationWidget *_relation;
    ConnectionHandle *_oldHandle;
    ConnectionHandle *_newHandle;
    MultiLineF _oldPoly;
};

}
