#include "createwidgettool.h"
#include "graphview.h"
#include "scene.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

namespace GraphView
{
CreateWidgetTool::CreateWidgetTool(Scene *scene)
    : AbstractTool{scene}
{
    _dragRect = scene->addRect(0, 0, 100, 60);
    _dragRect->hide();
    _cursor = Qt::CrossCursor;
}

void CreateWidgetTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    auto _lastClickPos = _scene->snapPoint(mouseEvent->scenePos());
    _dragRect->setRect(_lastClickPos.x(), _lastClickPos.y(), 0, 0);
    _dragRect->show();
}

void CreateWidgetTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    auto _lastClickPos = mouseEvent->buttonDownScenePos(Qt::LeftButton);
    auto pos = _scene->snapPoint(mouseEvent->scenePos());
    _dragRect->setRect(qMin(pos.x(), _lastClickPos.x()),
                       qMin(pos.y(), _lastClickPos.y()),
                       qAbs(pos.x() - _lastClickPos.x()),
                       qAbs(pos.y() - _lastClickPos.y()));
}

void CreateWidgetTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    _dragRect->hide();
    auto _lastClickPos = mouseEvent->buttonDownScenePos(Qt::LeftButton);

    auto pos = mouseEvent->scenePos();
    QRectF rc{qMin(pos.x(), _lastClickPos.x()), qMin(pos.y(), _lastClickPos.y()), qAbs(pos.x() - _lastClickPos.x()), qAbs(pos.y() - _lastClickPos.y())};
    if (rc.height() > 10 && rc.width() > 10)
        Q_EMIT createObjectRequested(rc);
}

QString CreateWidgetTool::text() const
{
    return "Create new widget";
}

AbstractTool::ToolType CreateWidgetTool::toolType() const
{
    return AbstractTool::ToolType::RequireActivation;
}

}
