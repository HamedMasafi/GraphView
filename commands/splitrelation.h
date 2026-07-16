#pragma once

#include <QPoint>
#include <QUndoCommand>
#include "widgets/multiline.h"

namespace GraphView
{
class Scene;

namespace Widgets
{
class CrossConnection;
class ConnectionHandle;
class Relation;
}

namespace Commands
{
class SplitRelation : public QUndoCommand
{
public:
    SplitRelation(Scene *scene, Widgets::Relation *from, Widgets::ConnectionHandle *to, const QPointF &intersectPoint);

public:
    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    Widgets::Relation *_from;
    Widgets::ConnectionHandle *_to;
    const QPointF _intersectPoint;

    void createNeededItems();

    Widgets::CrossConnection *_cross{nullptr};
    Widgets::Relation *_secondPartRelation{nullptr};
    Widgets::Relation *_newRelation{nullptr};
    Widgets::ConnectionHandle *_originalTo{nullptr};
    Widgets::MultiLineF _originalPoly;
    Widgets::MultiLineF _poly1;
    Widgets::MultiLineF _poly2;
};

}
}
