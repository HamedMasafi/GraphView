#pragma once

#include <commands/addremovewidgets.h>

namespace GraphView {

class BatchCreator : public AddWidgetsCommand
{
public:
    using AddWidgetsCommand::AddWidgetsCommand;

    void apply();
};

}
