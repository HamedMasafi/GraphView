#include "changepropertycommand.h"

#include <scene.h>
#include <widgets/abstractwidget.h>

#include <QDebug>

namespace GraphView::Commands {

ChangeState::ChangeState(Scene *scene,
                         Widgets::AbstractWidget *widget,
                         const QJsonObject &oldState,
                         const QJsonObject &newState)
    : _scene{scene}
    , _id{widget->id()}
    , _oldState{oldState}
    , _newState{newState}
{
    setText(QObject::tr("Change properties"));
}

void ChangeState::undo()
{
    auto w = _scene->widgetById(_id);
    if (w)
        w->restoreState(_oldState);
}

void ChangeState::redo()
{
    auto w = _scene->widgetById(_id);
    if (w)
        w->restoreState(_newState);
}

ChangeProperty::ChangeProperty(Scene *scene,
                               const QString &propertyName,
                               Widgets::AbstractWidget *widget,
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

void ChangeProperty::undo()
{
    auto w = _scene->widgetById(_id);
    if (w && !w->setProperty(_propertyName.toUtf8(), _oldValue))
        qWarning() << "Unable to set property " << _propertyName << " for " << w;
}

void ChangeProperty::redo()
{
    auto w = _scene->widgetById(_id);
    if (w && !w->setProperty(_propertyName.toUtf8(), _newValue))
        qWarning() << "Unable to set property " << _propertyName << " for " << w;
}

} // namespace GraphView::Commands
