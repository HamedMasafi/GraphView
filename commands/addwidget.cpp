

#include "addwidget.h"
#include "scene.h"
#include "scene_p.h"
#include "widgets/abstractwidget.h"
#include "core/pointshelper.h"
#include "handles/connectionhandle.h"
#include "widgets/relation.h"
#include "widgets/textnode.h"

#include <QApplication>

namespace GraphView
{

AddWidgetCommand::AddWidgetCommand(Scene *scene, const QRectF &rect, AbstractWidget *item)
    : _scene{scene}
    , _mode{Mode::InRect}
    , _rect{rect}
    , _item{item}
{
    setText(QCoreApplication::translate("AddWidgetCommand", "Add new widget"));
}

AddWidgetCommand::AddWidgetCommand(ScenePrivate *scenePrivate, const QRectF &rect, const QString &className)
    : _scenePrivate{scenePrivate}
    , _mode{Mode::InRect}
    , _rect{rect}
    , _className{className}
{
    setText(QStringLiteral("Add new %1").arg(_className));
}

AddWidgetCommand::AddWidgetCommand(Scene *scene, const QPointF &pt, AbstractWidget *item)
    : _scene{scene}
    , _mode{Mode::InPoint}
    , _point{pt}
    , _item{item}
{
    setText(QCoreApplication::translate("AddWidgetCommand", "Add new widget"));
}

void AddWidgetCommand::undo()
{
    _scene->removeWidget(_item);
}

void AddWidgetCommand::redo()
{
    _item = _scenePrivate->createWidget(_className, _id);
    _item->setRect(_rect);
    qDebug() << "item created" << _id << _item->id();
    _id = _item->id();
}

void AddWidgetCommand::checkForRelation()
{
    auto relations = _scene->relations();

    for (auto &r : relations) {
        if (!r->to().handle()) {
        }
    }
}

} // namespace GraphView
