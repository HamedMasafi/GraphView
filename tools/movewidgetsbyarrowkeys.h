#pragma once

#include "abstracttool.h"

namespace GraphView::Tools {


class MoveWidgetsByArrowKeys : public AbstractTool
{
    Q_OBJECT

public:
    MoveWidgetsByArrowKeys(Scene *scene);
    QString text() const override;
    ToolType toolType() const override;

public:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void moveSelectedWidgets(Qt::Key key, qreal distance);
};
}
