#pragma once

#include "abstracttool.h"

namespace GraphView
{

class ZoomTool : public AbstractTool
{
    Q_OBJECT
    Q_PROPERTY(int zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged FINAL)
public:
    explicit ZoomTool(Scene *scene);

    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    ToolType toolType() const override;
    void setZoomLevel(int newZoomLevel);

    int zoomLevel() const;

signals:
    void zoomLevelChanged(int n);

private:
    int _zoomLevel{100};
    bool _ctrlPressed{false};
};
}
