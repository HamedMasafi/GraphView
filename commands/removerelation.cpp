#include "removerelation.h"
#include "scene.h"
#include "handles/connectionhandle.h"
#include "widgets/relation.h"

namespace GraphView
{

RemoveRelationCommand::RemoveRelationCommand(Scene *scene, RelationWidget *relation)
    : QUndoCommand{}
    , _scene(scene)
    , _relation(relation)
{
    setText("Remove relation");
}

void RemoveRelationCommand::undo()
{
    if (_relation->from().handle())
        _relation->from().handle()->setRelation(_relation);
    if (_relation->to().handle())
        _relation->to().handle()->setRelation(_relation);
    _scene->addRelation(_relation);
}

void RemoveRelationCommand::redo()
{
    _scene->removeRelation(_relation);
    if (_relation->from().handle())
        _relation->from().handle()->setRelation(nullptr);
    if (_relation->to().handle())
        _relation->to().handle()->setRelation(nullptr);
}

}
