#include "removerelation.h"
#include "scene.h"
#include "handles/connectionhandle.h"
#include "widgets/relation.h"

namespace GraphView
{
namespace Commands
{
RemoveRelation::RemoveRelation(Scene *scene, Widgets::Relation *relation)
    : QUndoCommand{}
    , _scene(scene)
    , _relation(relation)
{
    setText("Remove relation");
}

void RemoveRelation::undo()
{
    if (_relation->from().handle())
        _relation->from().handle()->setRelation(_relation);
    if (_relation->to().handle())
        _relation->to().handle()->setRelation(_relation);
    _scene->addRelation(_relation);
}

void RemoveRelation::redo()
{
    _scene->removeRelation(_relation);
    if (_relation->from().handle())
        _relation->from().handle()->setRelation(nullptr);
    if (_relation->to().handle())
        _relation->to().handle()->setRelation(nullptr);
}
}
}
