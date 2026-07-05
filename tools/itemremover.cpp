#include "itemremover.h"
#include "commands/removerelation.h"
#include "commands/removewidget.h"
#include "scene.h"
#include "widgets/abstractwidget.h"
#include "widgets/relation.h"

#include <QGraphicsSceneEvent>
#include <QKeyEvent>

namespace GraphView
{
ItemRemoverTool::ItemRemoverTool(Scene *scene)
    : AbstractTool{scene}
{
}

// void ItemRemoverTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
// {
    // auto item = _scene->itemAt(mouseEvent->scenePos(), QTransform());

    // auto relations = _scene->relations();
    // for (auto &r : relations)
    //     if (r->intersect(mouseEvent->scenePos(), nullptr)) {
    //         _relation = r;
    //         return;
    //     }
    // _relation = nullptr;

    // _widget = dynamic_cast<AbstractWidget *>(item);
// }

void ItemRemoverTool::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        qDebug() << Q_FUNC_INFO;
        QList<AbstractWidget *> widgets = _scene->selectedWidgets();

        // for (auto &item : selectedItems) {
        //     auto widget = qgraphicsitem_cast<AbstractWidget*>(item);
        //     if (widget)
        //         widgets << widget;
        // }

        if (widgets.size()) {
            _scene->pushCommand(new RemoveWidgetCommand{_scene, widgets});
            // _widget = nullptr;
        }
        // if (_relation) {
        //     auto cmd = new RemoveRelationCommand{_scene, _relation};
        //     _scene->pushCommand(cmd);
        //     _relation = nullptr;
        // }
    }
}

AbstractTool::ToolType ItemRemoverTool::toolType() const
{
    return AbstractTool::ToolType::AlwaysListener;
}

}
