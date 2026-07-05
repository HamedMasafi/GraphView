#pragma once

#include <QRectF>
#include <QUndoCommand>
#include <QUuid>

namespace GraphView {
class Scene;
class AbstractWidget;

class ChangeWidgetsGeometryCommand : public QUndoCommand
{
public:
    ChangeWidgetsGeometryCommand(Scene *scene);

    void undo() override;
    void redo() override;

    void addWidget(AbstractWidget *widget, const QPointF &from, const QPointF &to);
    void addWidget(AbstractWidget *widget, const QRectF &from, const QRectF &to);

private:
    struct WidgetData
    {
        QUuid id;
        QRectF oldPos;
        QRectF newPos;
    };
    QList<WidgetData> _data;
    Scene *_scene;
};

} // namespace GraphView
