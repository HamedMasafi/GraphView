#pragma once

#include <QPoint>
#include <QUndoCommand>
#include "widgets/multiline.h"

namespace GraphView
{
class Scene;
class CrossConnection;
class ConnectionHandle;
class RelationWidget;

class SplitRelationCommand : public QUndoCommand
{
public:
    SplitRelationCommand(Scene *scene, RelationWidget *from, ConnectionHandle *to, const QPointF &intersectPoint);

public:
    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    RelationWidget *_from;
    ConnectionHandle *_to;
    const QPointF _intersectPoint;

    void createNeededItems();

    CrossConnection *_cross{nullptr};
    RelationWidget *_secondPartRelation{nullptr};
    RelationWidget *_newRelation{nullptr};
    ConnectionHandle *_originalTo{nullptr};
    MultiLineF _originalPoly;
    MultiLineF _poly1;
    MultiLineF _poly2;
};

}
