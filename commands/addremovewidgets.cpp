#include "addremovewidgets.h"

#include "scene.h"
#include "scene_p.h"
#include "widgets/abstractwidget.h"

#include <QCoreApplication>
#include <QJsonObject>

namespace GraphView::Commands {

struct WidgetChangeData
{
    QUuid id;
    QString className;
    QRectF rect;
    QJsonObject state;
    Widgets::AbstractWidget *widget{};
};

void AddWidgets::add(const QString &className,
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
        setText(QCoreApplication::translate("GraphView::Commands::AddWidgets", "Add widgets"));
    else
        setText(QCoreApplication::translate("GraphView::Commands::AddWidgets", "Add widget"));
}

void AddWidgets::undo()
{
    removeAll();
}

void AddWidgets::redo()
{
    createAll();
}

AbstractAddRemoveWidgets::AbstractAddRemoveWidgets(ScenePrivate *sceneData)
    : QUndoCommand{}
    , _sceneData{sceneData}
{}

void AbstractAddRemoveWidgets::createAll()
{
    for (auto &d : _data) {
        d->widget = _sceneData->createWidget(d->className, d->id);
        d->id = d->widget->id();
        d->widget->restoreState(d->state);
        d->widget->setRect(d->rect);
    }
}

void AbstractAddRemoveWidgets::removeAll()
{
    for (auto &d : _data) {
        _sceneData->removeWidget(d->widget);
        d->widget = nullptr;
    }
}

} // namespace GraphView::Commands
