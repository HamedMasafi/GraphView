#pragma once

#include <QUndoCommand>

namespace GraphView
{
class WidgetsResizeCommand : public QUndoCommand
{
public:
    WidgetsResizeCommand();
};

}