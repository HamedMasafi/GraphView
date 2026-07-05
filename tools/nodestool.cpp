#include "nodestool.h"
#include "commands/movehandlecommand.h"
#include "commands/nodescommands.h"
#include "scene.h"
#include "handles/polylinehandle.h"
#include "widgets/polylineitem.h"

#include <QDebug>
#include <QtMath>
#include <QEvent>
#include <QGraphicsRectItem>
#include <QGraphicsSceneEvent>
#include <QKeyEvent>

namespace GraphView
{

namespace
{

double distanceBetween(const QPointF &a, const QPointF &b)
{
    double dx = a.x() - b.x();
    double dy = a.y() - b.y();
    return qSqrt(dx * dx + dy * dy);
}

double pointToSegmentDistance(const QPointF &p, const QPointF &a, const QPointF &b)
{
    // اگر بازه یک نقطه باشد
    if (a == b)
        return QLineF(p, a).length();

    // پروجکشن p روی segment
    QPointF ab = b - a;
    QPointF ap = p - a;

    double t = QPointF::dotProduct(ap, ab) / QPointF::dotProduct(ab, ab);
    t = qBound(0.0, t, 1.0);

    QPointF projection = a + t * ab;
    return QLineF(p, projection).length();
}
int nearestPointToPolygon(const QPolygonF &poly, const QPointF &pt, qreal *distance)
{
    if (!poly.size())
        return -1;

    if (poly.size() == 1) {
        if (distance)
            *distance = distanceBetween(poly[0], pt);
        return 1;
    }

    int bestIndex = 0;
    double bestDist = std::numeric_limits<double>::max();

    for (auto i = 0; i < poly.size() - 1; ++i) {
        double d = pointToSegmentDistance(pt, poly[i], poly[i + 1]);
        if (d < bestDist) {
            bestDist = d;
            bestIndex = i + 1; // درج پس از poly[i]
        }
    }

    if (distance)
        *distance = bestDist;
    return bestIndex;
}

}

NodesTool::NodesTool(Scene *scene)
    : AbstractTool{scene}
{
    _selectArea = scene->addRect({});
    _selectArea->setPen(QPen{Qt::darkBlue});
    _selectArea->setBrush(QBrush{QColor{0, 0, 200, 100}});
    _selectArea->hide();
    // _cursor = Qt::CrossCursor;
}

void NodesTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton) {
        auto item = scene()->itemAt(mouseEvent->scenePos(), QTransform{});
        _selectedHandle = dynamic_cast<PolylineHandle *>(item);

        if (_selectedHandle) {
            _mouseMode = MouseMode::MoveHandle;
            _startPos = _selectedHandle->pos();
        } else {
            _mouseMode = MouseMode::DrawRect;

            _scene->clearSelection();
            _selectArea->setRect(mouseEvent->pos().x(), mouseEvent->pos().y(), 0, 0);
            _selectArea->show();
            // _handles = _scene->findChildren<AbstractHandle *>();
            // mouseEvent->setAccepted(false);
        }
        _clickPos = mouseEvent->scenePos();
    }
}

void NodesTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton) {
        switch(_mouseMode){
        case MouseMode::None:
        case MouseMode::MoveHandle:
            _selectedHandle->setPos(_startPos + mouseEvent->scenePos()- _clickPos);
            _selectedHandle->poly()->setNode(_selectedHandle->index(), _selectedHandle->pos());
            break;
        case MouseMode::DrawRect:
            QRectF rc{std::min(_clickPos.x(), mouseEvent->scenePos().x()),
                      std::min(_clickPos.y(), mouseEvent->scenePos().y()),
                      std::abs(_clickPos.x() - mouseEvent->scenePos().x()),
                      std::abs(_clickPos.y() - mouseEvent->scenePos().y())};
            _selectArea->setRect(rc);
            QPainterPath selectionArea;
            selectionArea.addRect(rc);
            _scene->setSelectionArea(selectionArea);
            break;
        }
    }
}

void NodesTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton) {
        switch (_mouseMode) {
        case MouseMode::None:
        case MouseMode::MoveHandle:{
            auto cmd = new MoveHandleCommand{_selectedHandle->poly(),
                                                                   _selectedHandle->index(),
                                                                   _startPos,
                                                                   mouseEvent->scenePos()};
            _scene->undoStack()->push(cmd);
            break;
        }
        case MouseMode::DrawRect:
            break;
        }
    }
    _selectArea->hide();
    if (mouseEvent->button() == Qt::RightButton) {
        if (_selectedHandle) {
            auto cmd = new RemoveNodesCommand;
            cmd->addHandle(_selectedHandle);
            _scene->undoStack()->push(cmd);
        }
        // auto item = _scene->itemAt(mouseEvent->pos(), {});

        // if (!item)
        //     return;

        // auto handle = qgraphicsitem_cast<AiDigitize::HandleItem *>(item);

        // if (!handle)
        //     return;

        // emit handle->removeRequested();
    }
}

void NodesTool::mouseDoubleClicked(QGraphicsSceneMouseEvent *mouseEvent)
{
    QList<PolylineItem *> polyItems;
    auto allItems = _scene->items();
    for (auto &item : allItems) {
        auto poly = dynamic_cast<PolylineItem *>(item);
        if (poly)
            polyItems << poly;
    }

    auto item = scene()->itemAt(mouseEvent->scenePos(), QTransform{});
    _selectedHandle = dynamic_cast<PolylineHandle *>(item);
    if (_selectedHandle)
        return;
    int bestIndex{-1};
    qreal bestDistance{std::numeric_limits<qreal>::max()};
    PolylineItem *selectedPoly{nullptr};

    qDebug() << "polyItems.size=" << polyItems.size();
    if (polyItems.size()) {
        for (auto &poly : polyItems) {
            qreal distance;
            auto b = nearestPointToPolygon(poly->poly(), mouseEvent->scenePos(), &distance);

            if (b == -1)
                continue;

            if (distance < bestDistance) {
                bestDistance = distance;
                bestIndex = b;
                selectedPoly = poly;
            }
        }
    }

    if (selectedPoly) {
        // if (selectedPoly->poly())
        auto cmd = new AddNodesCommand{selectedPoly, mouseEvent->scenePos(), bestIndex};
        _scene->undoStack()->push(cmd);
    } else {
        qDebug() << "selected poly is null";
    }
}

void NodesTool::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        // QMultiMap<PolylineItem *, PolylineHandle *> polyHandles;

        QList<PolylineHandle *> polyHandles;
        // qDebug() << "delete pressed on nodes tool";
        auto selectedItems = _scene->selectedItems();
        for (auto &item : selectedItems) {
            auto handle = dynamic_cast<AbstractHandle *>(item);
            if (!handle)
                continue;

            auto polyHandle = qobject_cast<PolylineHandle *>(handle);

            if (polyHandle) {
                polyHandles << polyHandle;
            }
        }

        if (polyHandles.size()) {
            auto cmd = new RemoveNodesCommand;
            for (auto &ph : polyHandles)
                cmd->addHandle(ph);
            _scene->undoStack()->push(cmd);
        }

        _scene->clearSelection();
    }
}

AbstractTool::ToolType NodesTool::toolType() const
{
    return ToolType::RequireActivation;
}

}
