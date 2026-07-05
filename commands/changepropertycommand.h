#pragma once

#include <QJsonObject>
#include <QUndoCommand>

namespace GraphView {

class Scene;
class AbstractWidget;

class ChangeStateCommand : public QUndoCommand
{
public:
    ChangeStateCommand(Scene *scene, AbstractWidget *widget, const QJsonObject&oldState,const QJsonObject&newState);
    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    QUuid _id;
    QJsonObject _oldState;
    QJsonObject _newState;

};

class ChangePropertyCommand : public QUndoCommand
{
public:
    ChangePropertyCommand(Scene *scene,
                   const QString &propertyName,
                   AbstractWidget *widget,
                   const QVariant &oldValue,
                   const QVariant &newValue);
    void undo() override;
    void redo() override;

private:
    Scene *_scene;
    QUuid _id;
    QString _propertyName;
    QVariant _oldValue;
    QVariant _newValue;

};

} // namespace GraphView
