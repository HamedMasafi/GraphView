

#include "addwidget.h"
#include "scene.h"
#include "scene_p.h"
#include "widgets/abstractwidget.h"
#include "core/pointshelper.h"
#include "handles/connectionhandle.h"
#include "widgets/relation.h"
#include "widgets/textnode.h"

#include <QApplication>

namespace GraphView::Commands
{

AddWidget::AddWidget(Scene *scene, const QRectF &rect, Widgets::AbstractWidget *item)
    : _scene{scene}
    , _mode{Mode::InRect}
    , _rect{rect}
    , _item{item}
{
    setText(QCoreApplication::translate("Commands::AddWidget", "Add new widget"));
}

AddWidget::AddWidget(ScenePrivate *scenePrivate, const QRectF &rect, const QString &className)
    : _scenePrivate{scenePrivate}
    , _mode{Mode::InRect}
    , _rect{rect}
    , _className{className}
{
    setText(QStringLiteral("Add new %1").arg(_className));
}

AddWidget::AddWidget(Scene *scene, const QPointF &pt, Widgets::AbstractWidget *item)
    : _scene{scene}
    , _mode{Mode::InPoint}
    , _point{pt}
    , _item{item}
{
    setText(QCoreApplication::translate("Commands::AddWidget", "Add new widget"));
}

void AddWidget::undo()
{
    _scene->removeWidget(_item);
}

void AddWidget::redo()
{
    // switch (_mode) {
    // case Mode::InPoint:
    //     _item->setPos(_point);
    //     break;
    // case Mode::InRect:
    //     _item->setPos(_rect.topLeft());
    //     _item->setSize(_rect.size());
    //     break;
    // }
    _item = _scenePrivate->createWidget(_className, _id);
    _item->setRect(_rect);
    qDebug() << "item created" << _id << _item->id();
    _id = _item->id();
}

void AddWidget::checkForRelation()
{
    // auto tn = dynamic_cast<Widgets::TextNode *>(_item);
    // if (!tn)
    //     return;

    // auto connections = tn->connectionHandles(GraphView::Input);
    // if (connections.size() != 1)
    //     return;

    auto relations = _scene->relations();

    for (auto &r : relations) {
        if (!r->to().handle()) {
            // if (GraphView::Core::distance(r->to().point(), connections.first()->sceneConnectionPoint()) < 10) {
            //     _connectedrelation = r;
            //     // r->setTo(connections.first());
            //     return;
            // }
        }
    }
}

// Widgets::AbstractWidget *AddWidget::createWidget(const QString &name, const QUuid &id)
// {
//     if (!_scenePrivae->creators.contains(name))
//         return nullptr;
//     auto w = _scenePrivate->creators[name]();
//     w->_className = name;

//     if (id.isNull())
//         w->_uuid = QUuid::createUuid();
//     else
//         w->_uuid = id;
//     _scenePrivate->scene->addWidget(w);
//     return w;
// }

} // namespace Commands
