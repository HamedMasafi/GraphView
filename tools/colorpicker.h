#pragma once

#include "abstracttool.h"

#include <QCursor>

namespace GraphView::Tools
{

class ColorPicker : public AbstractTool
{
    Q_OBJECT

public:
    explicit ColorPicker(Scene *scene);

    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void activate() override;
    void deactivate() override;

Q_SIGNALS:
    void colorSelected(QPointF point, QColor color);

private:
    QImage _sceneImage;
    QColor _color;
    QCursor _cursor;

};
}

