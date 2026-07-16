#include "changerelation.h"
#include "handles/connectionhandle.h"
#include "widgets/relation.h"

namespace GraphView::Commands
{

ChangeRelation::ChangeRelation(Scene *scene,
                               Widgets::Relation *relation,
                               Widgets::ConnectionHandle *oldHandle,
                               Widgets::ConnectionHandle *newHandle)
    : _scene{scene}
    , _relation{relation}
    , _oldHandle{oldHandle}
    , _newHandle{newHandle}
    , _oldPoly{relation->poly()}
{
}

void ChangeRelation::redo()
{
    _relation->setTo(_newHandle);
    _relation->update();
//    _oldHandle->setRelation(nullptr);
//    _newHandle->setRelation(_relation);
}

void ChangeRelation::undo()
{
    _relation->setTo(_oldHandle);
    _relation->setPoly(_oldPoly);
//    _newHandle->setRelation(nullptr);
//    _oldHandle->setRelation(_relation);
}

}
