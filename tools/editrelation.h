#pragma once

#include "abstracttool.h"

namespace GraphView
{

namespace Widgets{
class Relation;
}
namespace Tools
{
class EditRelation : public AbstractTool
{
    Q_OBJECT

public:
    EditRelation(Scene *scene);

    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    Widgets::Relation *_relation{nullptr};

};

}

}
