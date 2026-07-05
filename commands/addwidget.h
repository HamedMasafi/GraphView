#pragma once

#include <QRectF>
#include <QUndoCommand>
#include <QUuid>

namespace GraphView
{

class ScenePrivate;
class Scene;
class AbstractWidget;
class RelationWidget;

class AddWidgetCommand : public QUndoCommand
{
public:
    AddWidgetCommand(Scene *scene, const QRectF &rect, AbstractWidget *item);
    AddWidgetCommand(ScenePrivate *scenePrivate, const QRectF &rect, const QString &className);
    AddWidgetCommand(Scene *scene, const QPointF &pt, AbstractWidget *item);

    void undo() override;
    void redo() override;

private:
    void checkForRelation();

    enum class Mode { InPoint, InRect };

    ScenePrivate *_scenePrivate;
    Scene *_scene;
    Mode _mode;
    QRectF _rect;
    QPointF _point;
    AbstractWidget *_item;
    RelationWidget *_connectedrelation{nullptr};
    QUuid _id;
    QString _className;
};
}
