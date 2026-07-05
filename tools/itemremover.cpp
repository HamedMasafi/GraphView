#include "itemremover.h"
#include "commands/removerelation.h"
#include "commands/removewidget.h"
#include "scene.h"
#include "widgets/abstractwidget.h"
#include "widgets/relation.h"

#include <QGraphicsSceneEvent>
#include <QKeyEvent>

namespace GraphView::Tools
{
ItemRemover::ItemRemover(Scene *scene)
    : AbstractTool{scene}
{
}

// void ItemRemover::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
// {
    // auto item = _scene->itemAt(mouseEvent->scenePos(), QTransform());

    // auto relations = _scene->relations();
    // for (auto &r : relations)
    //     if (r->intersect(mouseEvent->scenePos(), nullptr)) {
    //         _relation = r;
    //         return;
    //     }
    // _relation = nullptr;

    // _widget = dynamic_cast<Widgets::AbstractWidget *>(item);
// }

void ItemRemover::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        qDebug() << Q_FUNC_INFO;
        QList<Widgets::AbstractWidget *> widgets = _scene->selectedWidgets();

        // for (auto &item : selectedItems) {
        //     auto widget = qgraphicsitem_cast<Widgets::AbstractWidget*>(item);
        //     if (widget)
        //         widgets << widget;
        // }

        if (widgets.size()) {
            _scene->pushCommand(new Commands::RemoveWidget{_scene, widgets});
            // _widget = nullptr;
        }
        // if (_relation) {
        //     auto cmd = new Commands::RemoveRelation{_scene, _relation};
        //     _scene->pushCommand(cmd);
        //     _relation = nullptr;
        // }
    }
}

AbstractTool::ToolType ItemRemover::toolType() const
{
    return AbstractTool::ToolType::AlwaysListener;
}

}
