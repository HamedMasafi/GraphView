#pragma once

#include <QPointF>
#include <QUndoCommand>
#include <QUuid>

namespace GraphView {
class Scene;
namespace Widgets {
class AbstractWidget;
}

namespace Commands {
class MoveWidgets : public QUndoCommand
{
public:
    MoveWidgets(GraphView::Scene *scene);

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
    GraphView::Scene *_scene;
};

} // namespace Commands
} // namespace GraphView
