#pragma once

#include <QPoint>
#include <QUndoCommand>
#include "core/graphview_global.h"

namespace GraphView {

namespace Handles {
class AbstractHandle;
}

namespace Widgets {
class PolylineItem;
}

namespace Commands {

class NodesCommand : public QUndoCommand
{
public:
    explicit NodesCommand();
    ~NodesCommand();
    void addHandle(GraphView::Handles::AbstractHandle *handle);
    void createHandles();
    void removeHandles();

protected:
    struct HandleData
    {
        Widgets::PolylineItem *item;
        QPointF point;
        SizeType handleIndex;
    };
    QList<HandleData *> handleData;
};

class AddNodesCommand : public NodesCommand
{
public:
    explicit AddNodesCommand(Widgets::PolylineItem *item,
                             QPointF point,
                             GraphView::SizeType handleIndex);
    void undo() override;
    void redo() override;
};

class RemoveNodesCommand : public NodesCommand
{
public:
    explicit RemoveNodesCommand();
    void undo() override;
    void redo() override;
};

} // namespace Commands
} // namespace GraphView
