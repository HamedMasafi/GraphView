#pragma once

#include "abstracttool.h"

namespace GraphView {

class MoveWidgetsByArrowKeysTool : public AbstractTool
{
    Q_OBJECT

public:
    MoveWidgetsByArrowKeysTool(Scene *scene);
    QString text() const override;
    ToolType toolType() const override;

public:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void moveSelectedWidgets(Qt::Key key, qreal distance);
};
}
