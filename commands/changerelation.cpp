#include "changerelation.h"
#include "handles/connectionhandle.h"
#include "widgets/relation.h"

namespace GraphView
{

ChangeRelationCommand::ChangeRelationCommand(Scene *scene,
                               RelationWidget *relation,
                               ConnectionHandle *oldHandle,
                               ConnectionHandle *newHandle)
    : _scene{scene}
    , _relation{relation}
    , _oldHandle{oldHandle}
    , _newHandle{newHandle}
    , _oldPoly{relation->poly()}
{
}

void ChangeRelationCommand::redo()
{
    _relation->setTo(_newHandle);
    _relation->update();
}

void ChangeRelationCommand::undo()
{
    _relation->setTo(_oldHandle);
    _relation->setPoly(_oldPoly);
}

}
