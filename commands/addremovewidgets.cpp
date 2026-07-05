#include "addremovewidgets.h"

#include "scene.h"
#include "scene_p.h"
#include "widgets/abstractwidget.h"

#include <QCoreApplication>
#include <QJsonObject>

namespace GraphView {

struct WidgetChangeData
{
    QUuid id;
    QString className;
    QRectF rect;
    QJsonObject state;
    AbstractWidget *widget{};
};

void AddWidgetsCommand::add(const QString &className,
                     const QRectF &rect,
                     const QJsonObject &state,
                     const QUuid &id)
{
    auto d = new WidgetChangeData;
    d->id = id;
    d->className = className;
    d->rect = rect;
    d->state = state;
    _data << d;

    if (_data.size() > 1)
        setText(QCoreApplication::translate("AddWidgetsCommand", "Add widgets"));
    else
        setText(QCoreApplication::translate("AddWidgetsCommand", "Add widget"));
}

void AddWidgetsCommand::undo()
{
    removeAll();
}

void AddWidgetsCommand::redo()
{
    createAll();
}

AbstractAddRemoveWidgetsCommand::AbstractAddRemoveWidgetsCommand(ScenePrivate *sceneData)
    : QUndoCommand{}
    , _sceneData{sceneData}
{}

void AbstractAddRemoveWidgetsCommand::createAll()
{
    for (auto &d : _data) {
        d->widget = _sceneData->createWidget(d->className, d->id);
        d->id = d->widget->id();
        d->widget->restoreState(d->state);
        d->widget->setRect(d->rect);
    }
}

void AbstractAddRemoveWidgetsCommand::removeAll()
{
    for (auto &d : _data) {
        _sceneData->removeWidget(d->widget);
        d->widget = nullptr;
    }
}

} // namespace GraphView
