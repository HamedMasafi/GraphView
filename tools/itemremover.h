#pragma once

#include "abstracttool.h"

namespace GraphView
{
class Scene;
class View;
class AbstractWidget;
class RelationWidget;

class ItemRemoverTool : public AbstractTool
{
public:
    explicit ItemRemoverTool(Scene *scene);

    // void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyPressEvent(QKeyEvent *event) override;
    ToolType toolType() const override;

private:
    // AbstractWidget *_widget{nullptr};
    // RelationWidget *_relation{nullptr};
};

}
