#include "addrelation.h"
#include <QApplication>
#include "widgets/relation.h"
#include "scene.h"
#include "handles/connectionhandle.h"

namespace GraphView
{

AddRelationCommand::AddRelationCommand(Scene *scene, ConnectionHandle *from, ConnectionHandle *to)
    : QUndoCommand{}
    , _scene{scene}
    , _from{from}
    , _to{to}
{
    setText(QCoreApplication::translate("AddRelationCommand", "New relation"));
}

void AddRelationCommand::redo()
{
    if (!_relation) {
        _relation = new RelationWidget{_from, _to};
    }
    _scene->addRelation(_relation);
}

RelationWidget *AddRelationCommand::relation() const
{
    return _relation;
}

void AddRelationCommand::undo()
{
    _scene->removeRelation(_relation);
    _from->setRelation(nullptr);
    _to->setRelation(nullptr);
    _relation->deleteLater();
    _relation = nullptr;
}

} // namespace GraphView
