#pragma once

#include <QRectF>
#include <QUndoCommand>
#include <QUuid>

namespace GraphView {

class ScenePrivate;
class Scene;
class AbstractWidget;

struct WidgetChangeData;

class AbstractAddRemoveWidgetsCommand: public QUndoCommand
{
public:
    AbstractAddRemoveWidgetsCommand(ScenePrivate *sceneData);
protected:
    void createAll() ;
    void removeAll();

    ScenePrivate *_sceneData;
    QList<WidgetChangeData *> _data;
};

class AddWidgetsCommand : public AbstractAddRemoveWidgetsCommand
{
public:
    using AbstractAddRemoveWidgetsCommand::AbstractAddRemoveWidgetsCommand;

    void add(const QString &className,
             const QRectF &rect,
             const QJsonObject &state,
             const QUuid &id = {});

    void undo() override;
    void redo() override;
};

} // namespace GraphView
