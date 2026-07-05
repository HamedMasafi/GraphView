#pragma once

#include <QPointF>
#include <QUndoCommand>
#include <QUuid>

namespace GraphView {
class Scene;
class AbstractWidget;

class MoveWidgetsCommand : public QUndoCommand
{
public:
    MoveWidgetsCommand(Scene *scene);

    void undo() override;
    void redo() override;

    void addWidget(const QUuid &id, const QPointF &from, const QPointF &to);

private:
    struct WidgetData
    {
        QPointF oldPos;
        QPointF newPos;
        QUuid id;
    };
    QList<WidgetData> _data;
    Scene *_scene;
};

} // namespace GraphView
