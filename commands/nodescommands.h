#pragma once

#include <QPoint>
#include <QUndoCommand>
#include "core/graphview_global.h"

namespace GraphView {

class AbstractHandle;
class PolylineItem;

class NodesCommand : public QUndoCommand
{
public:
    explicit NodesCommand();
    ~NodesCommand();
    void addHandle(AbstractHandle *handle);
    void createHandles();
    void removeHandles();

protected:
    struct HandleData
    {
        PolylineItem *item;
        QPointF point;
        SizeType handleIndex;
    };
    QList<HandleData *> handleData;
};

class AddNodesCommand : public NodesCommand
{
public:
    explicit AddNodesCommand(PolylineItem *item,
                             QPointF point,
                             SizeType handleIndex);
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

} // namespace GraphView
