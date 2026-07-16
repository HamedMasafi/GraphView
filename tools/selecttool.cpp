#include "selecttool.h"
#include "commands/movehandlecommand.h"
#include "commands/movewidgets.h"
#include "event.h"
#include "graphview.h"
#include "handles/abstracthandle.h"
#include "handles/polylinehandle.h"
#include "handles/resizehandle.h"
#include "itemresizer.h"
#include "scene.h"
#include "widgets/abstractrectwidget.h"
#include "widgets/polylineitem.h"
#include "widgets/relation.h"

#include "commands/changewidgetsgeometry.h"

#include <QGraphicsSceneEvent>
#include <QJsonArray>
#include <QKeyEvent>

#include <widgets/abstractrectwidget.h>

namespace GraphView::Tools {

// namespace SelectToolImpl {

// class ResizeHandle : public QGraphicsEllipseItem
// {
// public:
//     ResizeHandle(qreal size = 15)
//         : QGraphicsEllipseItem{-size / 2, -size / 2, size, size}
//     {
//         setFlag(QGraphicsItem::ItemIgnoresTransformations);
//         setPen(QPen{Qt::darkGray});
//         setBrush(Qt::green);
//     }

//     Widgets::ResizeDirection resizeDirection() const { return m_resizeDirection; }
//     void setResizeDirection(Widgets::ResizeDirection newResizeDirection)
//     {
//         m_resizeDirection = newResizeDirection;
//     }

//     int index() const;
//     void setIndex(int newIndex);

// private:
//     Widgets::ResizeDirection m_resizeDirection;
//     int _index;
// };

// int ResizeHandle::index() const
// {
//     return _index;
// }

// void ResizeHandle::setIndex(int newIndex)
// {
//     _index = newIndex;
// }
// } // namespace SelectToolImpl

SelectTool::SelectTool(Scene *scene)
    : AbstractTool{scene}
{
    initHandles();

    _dragRect = _scene->addRect(0, 0, 100, 60);
    _dragRect->setZValue(10000);
    _dragRect->setPen(QPen{Qt::blue, 1});
    _dragRect->hide();

    if (scene)
        connect(scene, &Scene::selectionChanged, this, &SelectTool::slotSceneSelectionChanged);
}

void SelectTool::activate()
{
    if (_view) {
        _view->setDragMode(QGraphicsView::NoDrag);
        _view->setCursor(Qt::ArrowCursor);
    }
}

QString SelectTool::text() const
{
    return "Select tool";
}

void SelectTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    // if (!(mouseEvent->buttons() & Qt::LeftButton)){
    //     mouseEvent->setAccepted(false);
    //     return;
    // }

    m_isClonning = false;
    _wasMoved = false;
    auto item = _scene->itemAt(mouseEvent->scenePos(), QTransform());
    if (!item) {
        _clickPos = mouseEvent->scenePos();
        _mode = Mode::SelectWidgets;
        setVisible(false);
        _scene->clearSelection();
        return;
    }

    auto resizeHandle = dynamic_cast<Handles::ResizeHandle *>(item);
    if (resizeHandle) {
        _selectedHandle = resizeHandle;
        _itemClickPos = _selectedHandle->pos();
        _mode = Mode::ResizeWidget;
        return;
    }

    if (!(item->flags() & QGraphicsItem::ItemIsSelectable)) {
        _clickPos = mouseEvent->scenePos();
        _mode = Mode::SelectWidgets;
        setVisible(false);

        return;
    }

    auto rect = dynamic_cast<Widgets::AbstractRectWidget *>(item);
    if (rect && rect->flags() & QGraphicsItem::ItemIsSelectable) {
        if (rect->isSelected()) {
            _clickPos = mouseEvent->scenePos();

            auto selectedWidgets = _scene->selectedWidgets();

            _initialPositions.clear();
            for (auto &w : selectedWidgets)
                _initialPositions.insert(w, w->pos());
        } else {
            _clickPos = mouseEvent->scenePos();
            _objectInitialPos = rect->pos();

            if (mouseEvent->modifiers() & Qt::ShiftModifier) {
                setSelectedWidget(nullptr);
            } else {
                setSelectedWidget(rect);
                _scene->clearSelection();
                _initialPositions.clear();
            }
            rect->setSelected(true);

            _initialPositions.insert(rect, rect->pos());
        }
        _mode = _initialPositions.size() == 1 ? Mode::MoveWidget : Mode::MoveWidgets;
    }
}

void SelectTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->buttons() & Qt::LeftButton) {
        switch (_mode) {
        case Mode::ResizeWidget:
            handleMoving(dynamic_cast<Handles::ResizeHandle *>(_selectedHandle),
                         mouseEvent->scenePos());

            setSelectedWidgetsGeometry(QStringLiteral("x:%1 y:%2 width:%3 height:%4")
                                           .arg(_selectedWidget->x())
                                           .arg(_selectedWidget->y())
                                           .arg(_selectedWidget->width())
                                           .arg(_selectedWidget->height()));
            break;

        case Mode::SelectWidgets: {
            QRectF selectionArea{std::min(mouseEvent->scenePos().x(), _clickPos.x()),
                                 std::min(mouseEvent->scenePos().y(), _clickPos.y()),
                                 std::abs(mouseEvent->scenePos().x() - _clickPos.x()),
                                 std::abs(mouseEvent->scenePos().y() - _clickPos.y())};
            _dragRect->setRect(selectionArea);
            _dragRect->setVisible(true);
            break;
        }

        case Mode::MoveWidget: {
            auto i = _initialPositions.begin();

            if (mouseEvent->modifiers() & Qt::ControlModifier && !m_isClonning) {
                m_isClonning = true;

                auto w = _scene->cloneWidget(i.key());
                w->setPos(i.value());

                auto tmp = w->objectName();
                w->setObjectName(i.key()->objectName());
                i.key()->setObjectName(tmp);
            }
            i.key()->setPos(_scene->snapPoint(mouseEvent->scenePos() - _clickPos + i.value()));
            setHandlesOnItem(i.key()->childRect());
            setSelectedWidgetsGeometry(QStringLiteral("x:%1 y:%2 width:%3 height:%4")
                                           .arg(_selectedWidget->x())
                                           .arg(_selectedWidget->y())
                                           .arg(_selectedWidget->width())
                                           .arg(_selectedWidget->height()));
            break;
        }
        case Mode::MoveWidgets: {
            if (mouseEvent->modifiers() & Qt::ControlModifier && !m_isClonning) {
                m_isClonning = true;

                for (auto i = _initialPositions.begin(); i != _initialPositions.end(); ++i) {
                    auto w = _scene->cloneWidget(i.key());
                    w->setPos(i.value());

                    auto tmp = w->objectName();
                    w->setObjectName(i.key()->objectName());
                    i.key()->setObjectName(tmp);
                }
            }

            for (auto i = _initialPositions.begin(); i != _initialPositions.end(); ++i)
                i.key()->setPos(_scene->snapPoint(mouseEvent->scenePos() - _clickPos + i.value()));
            // _selectedWidget->setPos(
            //     _scene->snapPoint(mouseEvent->scenePos() - _clickPos + _objectInitialPos));
            // Core::MoveEvent e{_selectedWidget->pos(), oldPos};
            // Q_EMIT _selectedWidget->moving(&e);
            // setHandlesOnItem(_selectedWidget->childRect());
            break;
        }
        case Mode::MoveRelation: {
            _relation->processMouseMoveEvent(_scene->snapPoint(mouseEvent->scenePos()));
            break;
        }
        case Mode::None:
            break;

        default:
            break;
        }
        Q_EMIT moving();

        _wasMoved = true;
    } // if
}

void SelectTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_EMIT moved();
    // auto item = _scene->itemAt(mouseEvent->scenePos(), QTransform());
    // if (item && !_selectedHandle) {
    //     _selectedWidget = dynamic_cast<Widgets::AbstractWidget *>(item);
    //     qDebug() << "_selectedWidget" << _selectedWidget;
    //     setSelectedWidget(_selectedWidget);
    // }
    // qDebug() << mouseEvent->buttons() << (int)_mode;
    // if (mouseEvent->buttons() & Qt::LeftButton) {
    switch (_mode) {
    case Mode::ResizeWidget: {
        auto cmd = new Commands::ChangeWidgetsGeometry{_scene};
        _scene->pushCommand(cmd);
        cmd->addWidget(_selectedWidget, _initialRect, _selectedWidget->childRect());
        break;
    }
    case Mode::SelectWidgets: {
        _scene->clearSelection();
        QRectF selectionArea{std::min(mouseEvent->scenePos().x(), _clickPos.x()),
                             std::min(mouseEvent->scenePos().y(), _clickPos.y()),
                             std::abs(mouseEvent->scenePos().x() - _clickPos.x()),
                             std::abs(mouseEvent->scenePos().y() - _clickPos.y())};
        if (selectionArea.width() > 0 && selectionArea.height() > 0) {
            QPainterPath p;
            p.addRect(selectionArea);
            _scene->setSelectionArea(p);
        }

        _dragRect->setVisible(false);
        break;
    }
    case Mode::MoveWidget: {
        // checkForPlaceWidgetInRelation();
        // _selectedWidget->ungrabMouse();

        if (_wasMoved) {
            auto cmd = new Commands::ChangeWidgetsGeometry{_scene};
            _scene->pushCommand(cmd);
            _scene->setIsModified(true);
            cmd->addWidget(_selectedWidget,
                           mouseEvent->buttonDownScenePos(Qt::LeftButton),
                           _selectedWidget->pos());
        }
        break;
    }

    case Mode::MoveWidgets: {
        if (_wasMoved) {
            auto cmd = new Commands::MoveWidgets{_scene};
            _scene->pushCommand(cmd);
            _scene->setIsModified(true);
            for (auto i = _initialPositions.begin(); i != _initialPositions.end(); ++i) {
                cmd->addWidget(i.key()->id(), i.value(), i.key()->pos());
            }
        }
        break;
    }
    case Mode::MoveRelation: {
        _relation->processMouseReleaseEvent(_scene->snapPoint(mouseEvent->scenePos()));
        break;
    }
    case Mode::None:
        break;

    default:
        break;
    }
    // } // if
}

void SelectTool::mouseDoubleClicked(QGraphicsSceneMouseEvent *mouseEvent)
{
    auto item = _scene->itemAt(mouseEvent->scenePos(), QTransform());
    if (!item) {
        return;
    }

    auto w = dynamic_cast<Widgets::AbstractWidget *>(item);
    if (w && w->flags() & QGraphicsItem::ItemIsSelectable) {
        Q_EMIT _scene->widgetDoubleClicked(w);
    }
}

void SelectTool::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
        moveSelectedWidgets(static_cast<Qt::Key>(event->key()), 50);
    if (event->modifiers() == Qt::ShiftModifier)
        moveSelectedWidgets(static_cast<Qt::Key>(event->key()), 10);
    else
        moveSelectedWidgets(static_cast<Qt::Key>(event->key()), 1);
}

void SelectTool::handleMoving(QPointF point)
{
    auto s = dynamic_cast<Handles::ResizeHandle *>(sender());
    QRectF rc = resizeRect;

    auto pt = _scene->snapPoint(point);

    QSizeF minimumSize;     //  = ...
    QSizeF maximumSize;     //  = ...
    QPair<int, int> ration; // = ...
    if (s->resizeDirection() & Widgets::Top)
        rc.setTop(pt.y());
    if (s->resizeDirection() & Widgets::Left)
        rc.setLeft(pt.x());
    if (s->resizeDirection() & Widgets::Right)
        rc.setRight(pt.x());
    if (s->resizeDirection() & Widgets::Bottom)
        rc.setBottom(pt.y());

    if (rc.isValid()) {
        _selectedWidget->setRect(rc);
        setResezeHandlePos(s, pt);

        // resizeRect = rc;
        setHandlesOnItem(rc);
    } // if
}

void SelectTool::handleMoved()
{
    // _selectedWidget->ungrabMouse();
    // _selectedWidget->replaceHandles();
    Q_EMIT resized();
}

void SelectTool::handleMoving(Handles::ResizeHandle *handle, QPointF point)
{
    QRectF rc = resizeRect;

    auto pt = _scene->snapPoint(point);

    QSizeF minimumSize = _selectedWidget->minimumSize();
    QSizeF maximumSize = _selectedWidget->maximumSize();
    auto ratio = _selectedWidget->aspectRatio();

    if (ratio.first && ratio.second) {
    }

    if (handle->resizeDirection() & Widgets::Top)
        rc.setTop(pt.y());
    if (handle->resizeDirection() & Widgets::Left)
        rc.setLeft(pt.x());
    if (handle->resizeDirection() & Widgets::Right)
        rc.setRight(pt.x());
    if (handle->resizeDirection() & Widgets::Bottom)
        rc.setBottom(pt.y());

    if (rc.width() < 0)
        rc.setWidth(0);
    if (rc.height() < 0)
        rc.setHeight(0);
    if (!minimumSize.isEmpty()) {
        // clang-format off
        if (rc.width() < minimumSize.width())    rc.setWidth(minimumSize.width());
        if (rc.height() < minimumSize.height())  rc.setHeight(minimumSize.height());
        // clang-format on
    }
    if (!maximumSize.isEmpty()) {
        // clang-format off
        if (rc.width() < maximumSize.width())    rc.setWidth(maximumSize.width());
        if (rc.height() < maximumSize.height())   rc.setHeight(maximumSize.height());
        // clang-format on
    }
    if (ratio.first && ratio.second) {
        double targetAspect = ratio.first / ratio.second;
        double currentAspect = rc.width() / rc.height();

        if (currentAspect > targetAspect) {
            // مستطیل الان "بیش از حد پهن" است → عرض را کم می‌کنیم
            double newW = rc.height() * targetAspect;
            rc.setWidth(newW);
        } else {
            // مستطیل الان "بیش از حد بلند" است → ارتفاع را کم می‌کنیم
            double newH = rc.width() / targetAspect;
            rc.setHeight(newH);
        }

        // بازمرکز کردن
    }

    if (handle->resizeDirection() & Widgets::Top)
        rc.moveBottom(resizeRect.bottom());
    if (handle->resizeDirection() & Widgets::Left)
        rc.moveRight(resizeRect.right());
    if (handle->resizeDirection() & Widgets::Right)
        rc.moveLeft(resizeRect.left());
    if (handle->resizeDirection() & Widgets::Bottom)
        rc.moveTop(resizeRect.top());

    if (rc.isValid()) {
        _selectedWidget->setRect(rc);
        setResezeHandlePos(handle, pt);

        resizeRect = rc;
        //        proccessNewRect(rc);

        setHandlesOnItem(rc);
    } // if
}

void SelectTool::checkForPlaceWidgetInRelation()
{
    if (!_selectedWidget)
        return;
    //    if (_selectedWidget.pins)
}

void SelectTool::moveSelectedWidgets(Qt::Key key, qreal distance)
{
    auto widgets = _scene->selectedWidgets();

    for (Widgets::AbstractWidget *w : std::as_const(widgets)) {
        switch (key) {
        case Qt::Key_Left:
            w->setPos(w->pos().x() - distance, w->pos().y());
            break;
        case Qt::Key_Right:
            w->setPos(w->pos().x() + distance, w->pos().y());
            break;
        case Qt::Key_Up:
            w->setPos(w->pos().x(), w->pos().y() - distance);
            break;
        case Qt::Key_Down:
            w->setPos(w->pos().x(), w->pos().y() + distance);
            break;

        default:
            break;
        }
    }

    if (_selectedWidget)
        setHandlesOnItem(_selectedWidget->childRect());
}

void SelectTool::setHandlesOnItem(const QRectF &rc)
{
    // if( !_parent ) return;
    if (!_selectedWidget)
        return;

    setResezeHandlePos(resizerTL, rc.topLeft());
    setResezeHandlePos(resizerT, rc.topRight(), rc.topLeft());
    setResezeHandlePos(resizerTR, rc.topRight());

    setResezeHandlePos(resizerL, rc.topLeft(), rc.bottomLeft());
    setResezeHandlePos(resizerR, rc.topRight(), rc.bottomRight());

    setResezeHandlePos(resizerBL, rc.bottomLeft());
    setResezeHandlePos(resizerB, rc.bottomRight(), rc.bottomLeft());
    setResezeHandlePos(resizerBR, rc.bottomRight());

    for (int i = 0; i < rectResizeHandles.count(); i++)
        rectResizeHandles.at(i)->setZValue(9999);

    resizeRect = rc;
}

void SelectTool::setHandlesOnItem(const QPolygonF &poly)
{
    while (polyResizeHandles.size() > poly.size()) {
        auto h = polyResizeHandles.takeFirst();
        _scene->removeItem(h);
        delete h;
    }
    while (polyResizeHandles.size() < poly.size()) {
        auto h = new Handles::PolylineHandle;
        h->setBackgroundColor(Qt::green);
        _scene->addItem(h);
        h->setZValue(999999);
        h->setFlag(QGraphicsItem::ItemIsMovable);

        // connect(h, &Widgets::ResizeHandle::moving, this, &SelectTool::handleMoving);
        // connect(h, &Widgets::ResizeHandle::moved, this, &SelectTool::moved);
        polyResizeHandles << h;
    }

    for (auto i = 0; i < poly.size(); ++i) {
        polyResizeHandles[i]->setIndex(i);
        polyResizeHandles[i]->setPos(poly[i]);
    }
}

void SelectTool::setResezeHandlePos(Handles::ResizeHandle *handle, QPointF pos)
{
    handle->setPos(pos.x(), pos.y());
}

void SelectTool::setResezeHandlePos(Handles::ResizeHandle *handle, QPointF pos1, QPointF pos2)
{
    handle->setPos((pos2.x() + pos1.x()) / 2, (pos2.y() + pos1.y()) / 2);
}

void SelectTool::setSelectedWidget(Widgets::AbstractWidget *newSelectedWidget)
{
    _selectedWidget = newSelectedWidget;

    if (_selectedWidget) {
        _initialRect = _selectedWidget->childRect();
        auto poly = dynamic_cast<Widgets::PolylineItem *>(newSelectedWidget);

        if (poly) {
            _selectedItemType = SelectedItemType::Polygon;
            setHandlesOnItem(poly->poly());
            setVisible(true);
        } else {
            _selectedItemType = SelectedItemType::Rect;
            setHandlesOnItem(_selectedWidget->childRect());

            if (_selectedWidget->resizeMode() == GraphView::WidgetResizeMode::Horizontal) {
                resizerR->setVisible(true);
                resizerL->setVisible(true);
            } else if (_selectedWidget->resizeMode() == GraphView::WidgetResizeMode::Vertical) {
                resizerT->setVisible(true);
                resizerB->setVisible(true);
            } else if (_selectedWidget->resizeMode() == GraphView::WidgetResizeMode::None) {
                setVisible(false);
            } else {
                setVisible(true);
            }
        }

        if (_selectedWidget->aspectRatio().first && _selectedWidget->aspectRatio().second) {
            resizerT->setVisible(false);
            resizerR->setVisible(false);
            resizerB->setVisible(false);
            resizerL->setVisible(false);
        }

        resizeRect = _selectedWidget->childRect();

        setSelectedWidgetsGeometry(QStringLiteral("x:%1 y:%2 width:%3 height:%4")
                                       .arg(_selectedWidget->x())
                                       .arg(_selectedWidget->y())
                                       .arg(_selectedWidget->width())
                                       .arg(_selectedWidget->height()));
    } else {
        setVisible(false);
    }
}

void SelectTool::setVisible(bool visible)
{
    for (auto &h : polyResizeHandles)
        h->setVisible(visible && _selectedItemType == SelectedItemType::Polygon);
    for (auto &h : rectResizeHandles)
        h->setVisible(visible && _selectedItemType == SelectedItemType::Rect);
}

void SelectTool::initHandles()
{
    resizerTL = new Handles::ResizeHandle;
    resizerT = new Handles::ResizeHandle;
    resizerTR = new Handles::ResizeHandle;
    resizerL = new Handles::ResizeHandle;
    resizerR = new Handles::ResizeHandle;
    resizerBL = new Handles::ResizeHandle;
    resizerB = new Handles::ResizeHandle;
    resizerBR = new Handles::ResizeHandle;

    rectResizeHandles.append(resizerTL);
    rectResizeHandles.append(resizerT);
    rectResizeHandles.append(resizerTR);
    rectResizeHandles.append(resizerL);
    rectResizeHandles.append(resizerR);
    rectResizeHandles.append(resizerBL);
    rectResizeHandles.append(resizerB);
    rectResizeHandles.append(resizerBR);

    resizerTL->setCursor(Qt::SizeFDiagCursor);
    resizerT->setCursor(Qt::SizeVerCursor);
    resizerTR->setCursor(Qt::SizeBDiagCursor);
    resizerL->setCursor(Qt::SizeHorCursor);
    resizerR->setCursor(Qt::SizeHorCursor);
    resizerBL->setCursor(Qt::SizeBDiagCursor);
    resizerB->setCursor(Qt::SizeVerCursor);
    resizerBR->setCursor(Qt::SizeFDiagCursor);

    resizerT->setResizeDirection(Widgets::Top);
    resizerL->setResizeDirection(Widgets::Left);
    resizerR->setResizeDirection(Widgets::Right);
    resizerB->setResizeDirection(Widgets::Bottom);

    resizerTR->setResizeDirection(Widgets::Top | Widgets::Right);
    resizerTL->setResizeDirection(Widgets::Top | Widgets::Left);
    resizerBR->setResizeDirection(Widgets::Bottom | Widgets::Right);
    resizerBL->setResizeDirection(Widgets::Bottom | Widgets::Left);

    for (auto &handle : rectResizeHandles) {
        handle->setBackgroundColor(Qt::green);
        _scene->addItem(handle);
        handle->setZValue(999999);
        // handle->setFlag(QGraphicsItem::ItemIsMovable);
        // connect(handle, &Widgets::ResizeHandle::moving, this, &SelectTool::handleMoving);
        // connect(handle, &Widgets::ResizeHandle::moved, this, &SelectTool::moved);
        // handle->installEventFilter(this);
        handle->hide();
    } // for
}

SelectToolSubCommand::SelectToolSubCommand(SelectTool *parent)
    : _parent{parent}
{}

QString SelectTool::selectedWidgetsGeometry() const
{
    return m_selectedWidgetsGeometry;
}

void SelectTool::sceneChanged(Scene *newScene)
{
    if (_scene)
        disconnect(_scene, nullptr, this, nullptr);

    if (newScene)
        connect(newScene, &Scene::selectionChanged, this, &SelectTool::slotSceneSelectionChanged);
}

void SelectTool::slotSceneSelectionChanged()
{
    auto selectedWidgets = _scene->selectedWidgets();

    switch (selectedWidgets.size()) {
    case 0:
        setVisible(false);
        break;
    case 1:
        setSelectedWidget(selectedWidgets[0]);
        break;

    default:
        _initialPositions.clear();
        for (auto &w : selectedWidgets)
            _initialPositions.insert(w, w->pos());
        break;
    }
}

void SelectTool::setSelectedWidgetsGeometry(const QString &newSelectedWidgetsGeometry)
{
    if (m_selectedWidgetsGeometry == newSelectedWidgetsGeometry)
        return;
    m_selectedWidgetsGeometry = newSelectedWidgetsGeometry;
    emit selectedWidgetsGeometryChanged(m_selectedWidgetsGeometry);
}

} // namespace GraphView::Tools
