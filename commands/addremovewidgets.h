#pragma once

#include <QRectF>
#include <QUndoCommand>
#include <QUuid>

namespace GraphView {

class ScenePrivate;

class Scene;
namespace Widgets {
class AbstractWidget;
}
namespace Commands {

struct WidgetChangeData;

class AbstractAddRemoveWidgets: public QUndoCommand
{
public:
    AbstractAddRemoveWidgets(ScenePrivate *sceneData);
protected:
    void createAll() ;
    void removeAll();

    ScenePrivate *_sceneData;
    QList<WidgetChangeData *> _data;
};

class AddWidgets : public AbstractAddRemoveWidgets
{
public:
    using AbstractAddRemoveWidgets::AbstractAddRemoveWidgets;

    void add(const QString &className,
             const QRectF &rect,
             const QJsonObject &state,
             const QUuid &id = {});

    void undo() override;
    void redo() override;
};

} // namespace Commands
} // namespace GraphView
