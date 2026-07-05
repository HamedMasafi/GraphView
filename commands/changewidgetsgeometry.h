#pragma once

#include <QRectF>
#include <QUndoCommand>
#include <QUuid>

namespace GraphView {
class Scene;
namespace Widgets {
class AbstractWidget;
}

namespace Commands {
class ChangeWidgetsGeometry : public QUndoCommand
{
public:
    ChangeWidgetsGeometry(GraphView::Scene *scene);

    void undo() override;
    void redo() override;

    void addWidget(Widgets::AbstractWidget *widget, const QPointF &from, const QPointF &to);
    void addWidget(Widgets::AbstractWidget *widget, const QRectF &from, const QRectF &to);

private:
    struct WidgetData
    {
        QUuid id;
        QRectF oldPos;
        QRectF newPos;
    };
    QList<WidgetData> _data;
    GraphView::Scene *_scene;
};

} // namespace Commands
} // namespace GraphView
