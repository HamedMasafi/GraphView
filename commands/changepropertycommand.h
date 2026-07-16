#pragma once

#include <QJsonObject>
#include <QUndoCommand>

namespace GraphView {

class Scene;

namespace Widgets{
class AbstractWidget;
}
namespace Commands {
class ChangeState : public QUndoCommand
{
public:
    ChangeState(Scene *scene, Widgets::AbstractWidget *widget, const QJsonObject&oldState,const QJsonObject&newState);
    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    QUuid _id;
    QJsonObject _oldState;
    QJsonObject _newState;

};

class ChangeProperty : public QUndoCommand
{
public:
    ChangeProperty(GraphView::Scene *scene,
                   const QString &propertyName,
                   Widgets::AbstractWidget *widget,
                   const QVariant &oldValue,
                   const QVariant &newValue);
    void undo() override;
    void redo() override;

private:
    GraphView::Scene *_scene;
    QUuid _id;
    QString _propertyName;
    QVariant _oldValue;
    QVariant _newValue;

};


} // namespace Commands
} // namespace GraphView
