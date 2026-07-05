#pragma once

#include "abstracttool.h"

namespace GraphView
{
class Scene;
class View;

namespace Widgets
{
class AbstractWidget;
class Relation;
}

namespace Tools
{
class ItemRemover : public AbstractTool
{
public:
    explicit ItemRemover(Scene *scene);

    // void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyPressEvent(QKeyEvent *event) override;
    ToolType toolType() const override;

private:
    // Widgets::AbstractWidget *_widget{nullptr};
    // Widgets::Relation *_relation{nullptr};
};

}
}
