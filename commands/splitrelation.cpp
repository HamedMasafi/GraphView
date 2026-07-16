#include "splitrelation.h"
#include "core/pointshelper.h"
#include "scene.h"
#include "handles/connectionhandle.h"
#include "widgets/crossconnection.h"
#include "widgets/relation.h"

namespace GraphView::Commands
{
SplitRelation::SplitRelation(Scene *scene, Widgets::Relation *from, Widgets::ConnectionHandle *to, const QPointF &intersectPoint)
    : QUndoCommand{"Create relation"}
    , _scene{scene}
    , _from{from}
    , _to{to}
    , _intersectPoint{intersectPoint}
{
}

void SplitRelation::undo()
{
    _scene->removeItem(_cross);
    _scene->removeRelation(_secondPartRelation);
    _scene->removeRelation(_newRelation);
    _from->setPoly(_originalPoly);
    _from->setTo(_originalTo);
}

void SplitRelation::redo()
{
    if (!_cross)
        createNeededItems();

    _scene->addItem(_cross);
    _scene->addRelation(_secondPartRelation);

    _from->setTo(_cross);
    _from->setPoly(_poly1);
    _scene->addRelation(_newRelation);
}

void SplitRelation::createNeededItems()
{
    _originalPoly = _from->poly();

    bool found{false};
    for (int i = 0; i < _originalPoly.size() - 1; ++i) {
        if (!found && GraphView::Core::isInOneLine(_originalPoly.at(i), _intersectPoint, _originalPoly.at(i + 1))) {
            _poly1.append(_originalPoly.at(i));
            _poly2.append(_originalPoly.at(i));
            found = true;
        }

        if (found)
            _poly2.append(_originalPoly.at(i));
        else
            _poly1.append(_originalPoly.at(i));
    }
    _poly2.append(_originalPoly.last());

    _cross = new Widgets::CrossConnection;
    _cross->setCenterPos(_intersectPoint);

    _secondPartRelation = new Widgets::Relation{_cross, _from->to().handle()};
    _secondPartRelation->setPoly(_poly2);

    _newRelation = new Widgets::Relation{_cross, _to};
    _originalTo = _from->to().handle();
}

}
