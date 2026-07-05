#include "colorpicker.h"
#include "scene.h"
#include "graphview.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QDebug>

namespace GraphView
{

ColorPickerTool::ColorPickerTool(Scene *scene) : AbstractTool{scene}
{
    _cursor = QCursor{QPixmap{":/icons/color_picker.png"}, 1, 32};
}

void ColorPickerTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    _color = _sceneImage.pixelColor(mouseEvent->scenePos().toPoint());
    qDebug() << "Color selected" << _color;
    emit colorSelected(mouseEvent->scenePos(), _color);
    // emit finished();
}

void ColorPickerTool::activate()
{
    _sceneImage = QImage{_scene->sceneRect().size().toSize(), QImage::Format_ARGB32};
    _sceneImage.fill(Qt::transparent);

    QPainter painter{&_sceneImage};
    _scene->render(&painter);
    _view->setCursor(_cursor);
}

void ColorPickerTool::deactivate()
{
    _view->setCursor(Qt::ArrowCursor);
}

}
