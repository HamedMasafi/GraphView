#include "changepropertycommand.h"

#include <scene.h>
#include <widgets/abstractwidget.h>

#include <QDebug>

namespace GraphView {

ChangeStateCommand::ChangeStateCommand(Scene *scene,
                         AbstractWidget *widget,
                         const QJsonObject &oldState,
                         const QJsonObject &newState)
    : _scene{scene}
    , _id{widget->id()}
    , _oldState{oldState}
    , _newState{newState}
{
    setText(QObject::tr("Change properties"));
}

void ChangeStateCommand::undo()
{
    auto w = _scene->widgetById(_id);
    if (w)
        w->restoreState(_oldState);
}

void ChangeStateCommand::redo()
{
    auto w = _scene->widgetById(_id);
    if (w)
        w->restoreState(_newState);
}

ChangePropertyCommand::ChangePropertyCommand(Scene *scene,
                               const QString &propertyName,
                               AbstractWidget *widget,
                               const QVariant &oldValue,
                               const QVariant &newValue)
    : _scene{scene}
    , _propertyName{propertyName}
    , _oldValue{oldValue}
    , _newValue{newValue}
{
    _id = widget->id();
    setText(QObject::tr("Change propertiy"));
}

void ChangePropertyCommand::undo()
{
    auto w = _scene->widgetById(_id);
    if (w && !w->setProperty(_propertyName.toUtf8(), _oldValue))
        qWarning() << "Unable to set property " << _propertyName << " for " << w;
}

void ChangePropertyCommand::redo()
{
    auto w = _scene->widgetById(_id);
    if (w && !w->setProperty(_propertyName.toUtf8(), _newValue))
        qWarning() << "Unable to set property " << _propertyName << " for " << w;
}

} // namespace GraphView
