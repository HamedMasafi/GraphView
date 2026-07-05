#pragma once

#include <QRectF>
#include <QUndoCommand>
#include <QUuid>

namespace GraphView
{

class ScenePrivate;
class Scene;

namespace Widgets{
class AbstractWidget;
class Relation;
}
namespace Commands
{
class AddWidget : public QUndoCommand
{
public:
    AddWidget(Scene *scene, const QRectF &rect, Widgets::AbstractWidget *item);
    AddWidget(ScenePrivate *scenePrivate, const QRectF &rect, const QString &className);
    AddWidget(Scene *scene, const QPointF &pt, Widgets::AbstractWidget *item);

    void undo() override;
    void redo() override;

private:
    void checkForRelation();

    // Widgets::AbstractWidget *createWidget(const QString &name, const QUuid &id);

    enum class Mode { InPoint, InRect };

    ScenePrivate *_scenePrivate;
    Scene *_scene;
    Mode _mode;
    QRectF _rect;
    QPointF _point;
    Widgets::AbstractWidget *_item;
    Widgets::Relation *_connectedrelation{nullptr};
    QUuid _id;
    QString _className;
};

} // namespace Commands
}
