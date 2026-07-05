#pragma once

#include <QUndoCommand>

namespace GraphView
{
class Scene;

namespace Widgets
{
class ConnectionHandle;
class Relation;
}

namespace Commands
{
class AddRelation : public QUndoCommand
{
public:
//    AddRelation(Scene *scene, Widgets::Relation *relation);
    AddRelation(Scene *scene, Widgets::ConnectionHandle *from, Widgets::ConnectionHandle *to);
//    AddRelation(Scene *scene, Widgets::ConnectionHandle *from, Widgets::Relation *to, const QPointF &intersectPoint);

public:
    void undo() override;
    void redo() override;

    Widgets::Relation *relation() const;

private:
    Scene *_scene;
    Widgets::Relation *_relation{nullptr};
    Widgets::ConnectionHandle *_from;
    Widgets::ConnectionHandle *_to;
};

} // namespace Commands
}
