#include "nodescommands.h"
#include "handles/polylinehandle.h"
#include "widgets/polylineitem.h"

namespace GraphView
{

NodesCommand::NodesCommand() : QUndoCommand{}
{
}

NodesCommand::~NodesCommand()
{
    qDeleteAll(handleData);
}

void NodesCommand::addHandle(AbstractHandle *handle)
{
    auto polyHandle = qobject_cast<PolylineHandle *>(handle);

    if (polyHandle) {
        handleData << new HandleData{polyHandle->poly(), polyHandle->pos(), polyHandle->index()};
    }
}

void NodesCommand::createHandles()
{
    std::sort(handleData.begin(), handleData.end(), [](HandleData *a, HandleData *b) {
        return a->handleIndex < b->handleIndex;
    });
    for (auto &hd: handleData){
        hd->item->insertNode(hd->handleIndex, hd->point);
    }
}

void NodesCommand::removeHandles()
{
    std::sort(handleData.begin(), handleData.end(), [](HandleData *a, HandleData *b) {
        return a->handleIndex > b->handleIndex;
    });
    for (auto &hd: handleData){
        hd->item->removeNode(hd->handleIndex);
    }
}

AddNodesCommand::AddNodesCommand(PolylineItem *item, QPointF point, SizeType handleIndex)
    : NodesCommand{}
{
    handleData << new HandleData{item, point, handleIndex};
}

void AddNodesCommand::undo()
{
    this->removeHandles();
}

void AddNodesCommand::redo()
{
    this->createHandles();
}

RemoveNodesCommand::RemoveNodesCommand()
    : NodesCommand{}
{
}

void RemoveNodesCommand::undo()
{
    this->createHandles();
}

void RemoveNodesCommand::redo()
{
    this->removeHandles();
}

}
