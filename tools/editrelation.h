#pragma once

#include "abstracttool.h"

namespace GraphView
{

class RelationWidget;

class EditRelationTool : public AbstractTool
{
    Q_OBJECT

public:
    EditRelationTool(Scene *scene);

    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    RelationWidget *_relation{nullptr};

};

}
