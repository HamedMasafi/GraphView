#include "addrelation.h"
#include <QApplication>
#include "widgets/relation.h"
#include "scene.h"
#include "handles/connectionhandle.h"

namespace GraphView::Commands
{
//AddRelation::AddRelation(Scene *scene, Widgets::Relation *relation)
//    : QUndoCommand{}
//    , _scene{scene}
//    , _relation{relation}
//{
//    setText(QCoreApplication::translate("Commands::AddRelation", "New relation"));
//}

AddRelation::AddRelation(Scene *scene, Widgets::ConnectionHandle *from, Widgets::ConnectionHandle *to)
    : QUndoCommand{}
    , _scene{scene}
    , _from{from}
    , _to{to}
{
    setText(QCoreApplication::translate("Commands::AddRelation", "New relation"));
}

//AddRelation::AddRelation(Scene *scene, Widgets::ConnectionHandle *from, Widgets::Relation *to, const QPointF &intersectPoint)
//    : QUndoCommand{}
//    , _scene{scene}
//    , _relation{new Widgets::Relation}
//{

//}

void AddRelation::redo()
{
    if (!_relation) {
        _relation = new Widgets::Relation{_from, _to};
//        _from->setRelation(_relation);
//        _to->setRelation(_relation);
    }
    _scene->addRelation(_relation);
}

Widgets::Relation *AddRelation::relation() const
{
    return _relation;
}

void AddRelation::undo()
{
    _scene->removeRelation(_relation);
    _from->setRelation(nullptr);
    _to->setRelation(nullptr);
    _relation->deleteLater();
    _relation = nullptr;
}


} // namespace Commands
