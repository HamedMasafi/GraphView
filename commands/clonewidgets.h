#pragma once

#include <QJsonArray>
#include <QUndoCommand>

namespace GraphView {
class Scene;
class AbstractWidget;

class CloneWidgetsCommand : public QUndoCommand
{
public:
    CloneWidgetsCommand(Scene *scene);

    void redo() override;
    void undo() override;

private:
    Scene *_scene{};
    QJsonArray _data;
    QList<AbstractWidget*>_createdWidgets;
};

} // namespace GraphView
