#include "clonewidgets.h"

#include <scene.h>
#include <widgets/abstractwidget.h>

#include <QJsonArray>

namespace GraphView::Commands {

CloneWidgets::CloneWidgets(Scene *scene)
    : QUndoCommand{}
    , _scene{scene}
{
    _data = _scene->dumpSelectedWidgets();
}

void CloneWidgets::undo() {
    for (auto &w : _createdWidgets) {
        _scene->removeWidget(w);
    }
    _createdWidgets.clear();
}

void CloneWidgets::redo()
{
    _createdWidgets = _scene->pasteWidgets(_data);

    _scene->clearSelection();
    for (auto &w : _createdWidgets) {
        w->setPos(w->pos().x() + 20, w->pos().y() + 20);
        w->setSelected(true);
    }
}

} // namespace GraphView::Commands
