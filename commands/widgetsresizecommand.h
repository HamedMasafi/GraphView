#pragma once

#include <QUndoCommand>

namespace GraphView::Commands
{
class WidgetsResizeCommand : public QUndoCommand
{
public:
    WidgetsResizeCommand();
};

}