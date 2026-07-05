#pragma once

#include "abstracttool.h"

#include <QCursor>

namespace GraphView
{

class ColorPickerTool : public AbstractTool
{
    Q_OBJECT

public:
    explicit ColorPickerTool(Scene *scene);

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

