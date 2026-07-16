#pragma once

#include "widgets/multiline.h"
#include <QUndoCommand>

namespace GraphView
{
class Scene;

namespace Widgets
{
class Relation;
class ConnectionHandle;
}
namespace Commands
{

class ChangeRelation : public QUndoCommand
{
public:
    ChangeRelation(Scene *scene, Widgets::Relation *relation, Widgets::ConnectionHandle *oldHandle, Widgets::ConnectionHandle *newHandle);

    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    Widgets::Relation *_relation;
    Widgets::ConnectionHandle *_oldHandle;
    Widgets::ConnectionHandle *_newHandle;
    Widgets::MultiLineF _oldPoly;
};

}
}
