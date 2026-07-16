#pragma once

#include <QJsonArray>
#include <QUndoCommand>

namespace GraphView {
class Scene;
namespace Widgets {
class AbstractWidget;
}

namespace Commands {

class CloneWidgets : public QUndoCommand
{
public:
    CloneWidgets(Scene *scene);

    void redo() override;
    void undo() override;

private:
    Scene *_scene{};
    QJsonArray _data;
    QList<Widgets::AbstractWidget*>_createdWidgets;
};

} // namespace Commands
} // namespace GraphView
