#pragma once

#include <commands/addremovewidgets.h>

namespace GraphView {

class BatchCreator : public Commands::AddWidgets
{
public:
    using Commands::AddWidgets::AddWidgets;

    void apply();
};

}
