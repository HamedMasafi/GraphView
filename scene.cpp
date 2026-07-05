#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDrag>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QKeyEvent>
#include <QMimeData>
#include <QPainter>
#include <QPen>
#include <math.h>

#include "batchcreator.h"
#include "commands/addremovewidgets.h"
#include "commands/addwidget.h"
#include "commands/clonewidgets.h"
#include "commands/movewidgets.h"
#include "commands/removewidget.h"
#include "core/jsonserialize.h"
#include "graphview.h"
#include "itemresizer.h"
#include "scene.h"
#include "scene_p.h"
#include "tools/abstracttool.h"
#include "tools/connectwidgets.h"
#include "tools/editrelation.h"
#include "tools/itemremover.h"
#include "tools/resizehandlesprovider.h"
#include "tools/resizewidget.h"
#include "tools/widgetmove.h"
#include "tools/zoom.h"
#include "widgets/abstractwidget.h"
#include "widgets/relation.h"

namespace GraphView {

namespace Z {
constexpr int Widgets{100};
constexpr int ControlItems{10000};
} // namespace Z

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
    , dd{new ScenePrivate{this}}
{
    this->clear();

    dd->dragRect = addRect(0, 0, 100, 60);
    dd->dragRect->setPen(QPen{Qt::blue, 1});
    dd->dragRect->hide();

    dd->relationRect = new QGraphicsRectItem;
    dd->relationRect->setPen(QPen{Qt::darkBlue, 3});
    dd->relationRect->setZValue(99);
    addItem(dd->relationRect);
    dd->relationRect->hide();

    dd->relationPreview = new RelationWidget;
    addItem(dd->relationPreview);
    dd->relationPreview->hide();

    //    dd->resizer = new WidgetResizer{this};
    //    connect(dd->resizer, &WidgetResizer::resized, this, &Scene::widgets_resized);

    setSceneRect(QRectF{0, 0, 1000, 800});

    createBackgroundBrush();
    initBasicCommands();
    createTooltipItem();
}

Scene::Scene(const QRectF &sceneRect, QObject *parent)
    : QGraphicsScene(sceneRect, parent)
{
    this->clear();
    initBasicCommands();
}

Scene::Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(x, y, width, height, parent)
{
    this->clear();
    initBasicCommands();
}

Scene::~Scene()
{
    for (auto &t : std::as_const(dd->readyTools))
        t->setScene(nullptr);
    for (auto &t : std::as_const(dd->shadowTools))
        t->setScene(nullptr);
}

QRect Scene::documentArea() const
{
    return QRect();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (dd->tempTool) {
        dd->tempTool->mousePressed(mouseEvent);
        QGraphicsScene::mousePressEvent(mouseEvent);
        mouseEvent->accept();
        return;
    }
    mouseEvent->accept();
    for (auto &t : dd->shadowTools) {
        t->mousePressed(mouseEvent);
        // if (!mouseEvent->isAccepted())
        // return;
    }

    if (Q_LIKELY(dd->tool)) {
        dd->tool->mousePressed(mouseEvent);
    } else {
        auto item = itemAt(mouseEvent->scenePos(), QTransform());
        for (auto &t : dd->readyTools)
            if (t->accept(item, mouseEvent)) {
                dd->tool = t;
                qDebug() << "Tool selected" << t << t->text();
                dd->tool->mousePressed(mouseEvent);

                // QGraphicsScene::mousePressEvent(mouseEvent);
                // mouseEvent->accept();

                break;
            }
    }

    // QGraphicsScene::mousePressEvent(mouseEvent);
    mouseEvent->accept();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (dd->tempTool) {
        dd->tempTool->mouseMoved(mouseEvent);
        QGraphicsScene::mouseMoveEvent(mouseEvent);
        mouseEvent->accept();
        return;
    }
    for (auto &t : dd->shadowTools)
        t->mouseMoved(mouseEvent);

    if (Q_LIKELY(dd->tool)) {
        dd->tool->mouseMoved(mouseEvent);
    }
    // else
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    // emit mouseMove(mouseEvent);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (dd->tempTool) {
        dd->tempTool->mouseReleased(mouseEvent);
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        mouseEvent->accept();
        return;
    }

    for (auto &t : dd->shadowTools)
        t->mouseReleased(mouseEvent);

    //    qDebug() << Q_FUNC_INFO << dd->tool << mouseGrabberItem();
    if (Q_LIKELY(dd->tool)) {
        // if (!mouseGrabberItem())
        dd->tool->mouseReleased(mouseEvent);
    }
    // else
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    for (auto &t : dd->shadowTools)
        t->mouseDoubleClicked(event);
    if (Q_LIKELY(dd->tool))
        dd->tool->mouseDoubleClicked(event);
}

void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();

    dd->dragRect->setPos(event->scenePos());
    dd->dragRect->setZValue(555);
    dd->dragRect->show();
}

void Scene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    dd->dragRect->hide();
    Q_UNUSED(event);
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);

    event->accept();
    dd->dragRect->setPos(snapPoint(event->scenePos()));
    dd->dragRect->show();
    //    dd->dragRect->setRect(0, 0, 100, 60);
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    emit widgetDropped(dd->dragRect->rect(), event->mimeData()->text());
    emit drop(event);
    //    dd->dragRect->hide();

    // auto mime = event->mimeData()->text();
    // static QRegularExpression r{R"(widget/(\S+);w=(\d*)&h=(\d*))"};
    // auto m = r.match(mime);

    // auto className = m.captured(1);
    // auto w= m.captured(2);
    // auto h= m.captured(3);

    if (!dd->dragCreator.isEmpty()) {
        auto cmd = new AddWidgetCommand{dd.data(),
                                           QRectF{snapPoint(event->scenePos()),
                                                  dd->dragRect->rect().size()},
                                           dd->dragCreator};
        pushCommand(cmd);
        setIsModified(true);
    }

    dd->dragRect->hide();
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    for (auto &t : dd->shadowTools)
        t->keyPressEvent(event);

    if (dd->tool)
        dd->tool->keyPressEvent(event);
    if (event->key() == Qt::Key_Delete && dd->selectedWidget) {
        //        _resizer->clear(true);
        Q_EMIT removeRequested(dd->selectedWidget);
    }
}

void Scene::keyReleaseEvent(QKeyEvent *event)
{
    for (auto &t : dd->shadowTools)
        t->keyReleaseEvent(event);

    if (dd->tool)
        dd->tool->keyReleaseEvent(event);
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    for (auto &t : dd->shadowTools)
        t->wheelEvent(event);

    if (dd->tool)
        dd->tool->wheelEvent(event);
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->save();


    painter->setBrush(
        Qt::darkGray); // فرض می‌کنیم dd->backgroundColor همیشه سفید باشد یا رنگ دلخواه شما
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect); // کل ناحیه را می‌پوشاند
    painter->restore();


    // اگر نوع گرید None باشد، همینجا تمام می‌کنیم
    // if (dd->gridType == GraphView::Scene::GridType::None)
    //     return;

    // 2. از sceneRect برای تعیین محدوده اصلی گرید استفاده می‌کنیم
    QRectF gridRect
        = sceneRect(); // اگر sceneRect تعریف نشده باشد، از rect استفاده می‌کند

    // اعمال مارجین‌ها بر روی sceneRect
    QRectF pureRect = gridRect.adjusted(dd->margins.left(),
                                        dd->margins.top(),
                                        -dd->margins.right(),
                                        -dd->margins.bottom());

    // اگر pureRect کوچکتر یا مساوی صفر باشد، رسم گرید معنی ندارد
    if (pureRect.isNull() || pureRect.isEmpty())
        return;

    painter->save();

    painter->setBrush(dd->backgroundColor);
    painter->fillRect(sceneRect(), dd->backgroundColor);


    if (!dd->backgroundPixmap.isNull())
        dd->drawBackgroundPixmap(painter);

    painter->setOpacity(dd->gridOpacity);
    painter->setPen(dd->gridColor);
    painter->drawRect(sceneRect());

    const qreal stepX = dd->gridSize.width();
    const qreal stepY = dd->gridSize.height();

    if (stepX <= 0 || stepY <= 0) {
        painter->restore();
        return;
    }

    // محاسبه شروع از نزدیک‌ترین نقطه روی گرید، بر اساس pureRect
    qreal startX = std::floor(pureRect.left() / stepX) * stepX;
    qreal endX = std::ceil(pureRect.right() / stepX) * stepX;

    qreal startY = std::floor(pureRect.top() / stepY) * stepY;
    qreal endY = std::ceil(pureRect.bottom() / stepY) * stepY;

    // 3. برش (Clipping) به sceneRect برای اطمینان از اینکه گرید بیرون نمی‌زند
    painter->setClipRect(gridRect, Qt::IntersectClip);

    switch (dd->gridType) {
    case GraphView::Scene::GridType::Dot: {
        qreal x = startX;
        while (x <= endX) {
            qreal y = startY;
            while (y <= endY) {
                if (pureRect.contains(x, y)) {
                    painter->drawPoint(QPointF(x, y));
                }

                y += stepY;
            }
            x += stepX;
        }
        // for (qreal x = startX; x <= endX; x += stepX) {
        //     for (qreal y = startY; y <= endY; y += stepY) {
        //         // مطمئن می‌شویم که نقطه داخل pureRect است قبل از رسم
        //         if (pureRect.contains(x, y)) {
        //             painter->drawPoint(QPointF(x, y));
        //         }
        //     }
        // }
        break;
    }

    case GraphView::Scene::GridType::Grid: {
        // محاسبه طول خطوط با در نظر گرفتن pureRect
        qreal lineEndY = pureRect.bottom();
        qreal lineEndX = pureRect.right();

        qreal x = startX;
        while (x <= endX) {
            painter->drawLine(QPointF(x, pureRect.top()), QPointF(x, lineEndY));
            x += stepX;
        }

        qreal y = startY;
        while (y <= endY) {
            painter->drawLine(QPointF(pureRect.left(), y), QPointF(lineEndX, y));
            y += stepY;
        }
        break;
    }

    default:
        break;
    }

    painter->restore();
}

void Scene::slotToolFinished()
{
    dd->tool = nullptr;
}

void Scene::widgets_resized()
{
    //    auto cmd = new WidgetsResizeCommand{this, d->report};

    //    if (d->resizer->selectedWidgets().count() == 1
    //        && d->resizer->selectedWidgets().at(0)->widgetType() == BandWidget)
    //        {
    //       cmd->setText(
    //           QString(tr("%1
    //           resized")).arg(d->resizer->selectedWidgets().at(0)->objectName()));
    //       cmd->setOldState(d->lastBandXML);
    //       cmd->setNewState(getSelectedBandsXML());

    //    } else {
    //       cmd->setText(
    //           QString(tr("%1
    //           resized")).arg(d->resizer->selectedWidgets().at(0)->objectName()));
    //       cmd->setOldState(d->lastXML);
    //       cmd->setNewState(getSelectedWidgetXML());

    //       QList<WidgetBase *> widgets = selectedWidgets();
    //       foreach (WidgetBase *widget, widgets)
    //           refreshWidgetParents(widget);
    //    }
    //    d->undoStack->push(cmd);
    //    cmd->setReady();
    //    d->lastXML = getSelectedWidgetXML();
    //    d->lastBandXML = getSelectedBandsXML();
}

void Scene::widget_doubleClicked()
{
    auto s = qobject_cast<AbstractWidget *>(sender());
    if (s)
        Q_EMIT widgetDoubleClicked(s);
}

void Scene::selectWidget(AbstractWidget *s)
{
    //    _resizer->setActiveItem(s);
    dd->selectedWidget = s;
}

void Scene::registerTool(AbstractTool *tool)
{
    tool->setScene(this);
    tool->setParent(this);
    if (dd->view)
        tool->setView(dd->view);

    switch (tool->toolType()) {
    case AbstractTool::ToolType::RequireActivation:
        dd->readyTools << tool;
        break;
    case AbstractTool::ToolType::AlwaysListener:
        dd->shadowTools << tool;
        break;
        // case AbstractTool::ToolType::OnDemand:
        // break;
    }

    dd->toolNames.insert(tool->metaObject()->className(), tool);
    connect(tool, &AbstractTool::finished, this, &Scene::slotToolFinished);
}

void Scene::beginDragDrop(const QString &className, const QSizeF &size)
{
    if (!dd->creators.contains(className))
        return;

    if (size == QSizeF{})
        dd->dragRect->setRect(0, 0, 100, 60);
    else
        dd->dragRect->setRect(0, 0, size.width(), size.height());

    dd->dragCreator = className;
    auto d = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(QStringLiteral("widgets/%1").arg(className));
    d->setMimeData(mimeData);
    d->exec();
}

QDrag *Scene::createDrag(const QString &className, const QSizeF &size)
{
    auto drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(
        QStringLiteral("widget/%1;w=%2&h=%3").arg(className).arg(size.width()).arg(size.height()));
    drag->setMimeData(mimeData);
    return drag;
}

void Scene::widget_selectedChanged()
{
    //    auto s = qobject_cast<AbstractWidget *>(sender());
    //    if (s) {
    //        selectWidget(s);
    //    }
}

void Scene::widget_moved(QPointF lastPos)
{
    auto s = qobject_cast<AbstractWidget *>(sender());
    if (!s)
        return;

    Q_EMIT widgetMoved(s, lastPos, s->pos());
}

void Scene::setNameForWidget(AbstractWidget *widget, bool force)
{
    dd->setNameForWidgetShort(widget, force);
}

void Scene::initBasicCommands()
{
    // registerTool<ConnectWidgetsTool>(Ready);
    // registerTool<ResizeWidgetTool>(Ready);
    // registerTool<ResizeHandlesProviderTool>(Ready);
    // registerTool<WidgetMoveTool>(Ready);
    // registerTool<EditRelationTool>(Ready);
    // registerTool<ZoomTool>(Shadow);
    // registerTool<ItemRemoverTool>(Shadow);
}

void Scene::createTooltipItem()
{
    dd->tooltipRect = addRect(0, 0, 0, 0); // new QGraphicsRectItem{this};
    dd->tooltipText = addText("");         // new QGraphicsTextItem{this};

    dd->tooltipRect->setBrush(qApp->palette().toolTipBase());
    dd->tooltipText->setDefaultTextColor(qApp->palette().toolTipText().color());

    dd->tooltipRect->setZValue(99);
    dd->tooltipText->setZValue(100);

    dd->tooltipRect->hide();
    dd->tooltipText->hide();
}

QList<AbstractWidget *> Scene::selectedWidgets() const
{
    QList<AbstractWidget *> ret;
    auto items = selectedItems();
    for (auto &item : items) {
        auto w = dynamic_cast<AbstractWidget *>(item);
        if (w)
            ret << w;
    }
    return ret;
}

QUndoStack *Scene::undoStack() const
{
    return dd->undoStack;
}

void Scene::pushCommand(QUndoCommand *cmd)
{
    dd->undoStack->push(cmd);
    setIsModified(true);
}

QList<AbstractWidget *> Scene::allWidgets() const
{
    QList<AbstractWidget *> ret;
    auto items = this->items();
    for (auto &item : items) {
        auto w = dynamic_cast<AbstractWidget *>(item);
        if (w)
            ret << w;
    }
    return ret;
}

GraphView::View *Scene::view() const
{
    return dd->view;
}

void Scene::setView(GraphView::View *newView)
{
    if (dd->view == newView)
        return;

    if (dd->view)
        dd->view->setScene(this);
    dd->view = newView;

    for (auto &t : dd->readyTools) {
        t->setView(newView);
    }
    for (auto &t : dd->shadowTools) {
        t->setView(newView);
    }
}

AbstractTool *Scene::activeTool() const
{
    return dd->tool;
}

AbstractTool *Scene::tool(const QString &name) const
{
    return dd->toolNames[name];
}

void Scene::alignSelectedWidgets(AlignMode mode)
{
    auto selectedWidgets = this->selectedWidgets();
    if (selectedWidgets.size() < 2)
        return;

    // آیتم مرجع (اولین انتخاب‌شده)
    auto *ref = selectedWidgets.first();
    if (!ref)
        return;

    // محاسبه min/max/centers
    qreal minLeft = std::numeric_limits<qreal>::max();
    qreal maxRight = std::numeric_limits<qreal>::lowest();
    qreal minTop = std::numeric_limits<qreal>::max();
    qreal maxBottom = std::numeric_limits<qreal>::lowest();

    qreal sumCenterX = 0;
    qreal sumCenterY = 0;

    for (auto *item : std::as_const(selectedWidgets)) {
        QRectF br = item->childRect();

        minLeft = std::min(minLeft, br.left());
        maxRight = std::max(maxRight, br.right());
        minTop = std::min(minTop, br.top());
        maxBottom = std::max(maxBottom, br.bottom());

        sumCenterX += br.center().x();
        sumCenterY += br.center().y();
    }

    qreal avgCenterX = sumCenterX / static_cast<qreal>(selectedWidgets.size());
    qreal avgCenterY = sumCenterY / static_cast<qreal>(selectedWidgets.size());

    // اندازه‌ی آیتم مرجع برای حالت‌های same-size
    QRectF refBr = ref->childRect();
    qreal refW = refBr.width();
    qreal refH = refBr.height();

    // -------------------------------------------------------
    //  توزیع یکنواخت — افقی
    // -------------------------------------------------------
    if (mode == AlignMode::DistributeHorizontally) {
        std::sort(selectedWidgets.begin(), selectedWidgets.end(), [](auto *a, auto *b) {
            return a->childRect().left() < b->childRect().left();
        });

        qreal totalWidth = 0;
        for (auto *item : std::as_const(selectedWidgets))
            totalWidth += item->childRect().width();

        qreal space = (maxRight - minLeft) - totalWidth;
        if (space < 0)
            space = 0;

        qreal gap = space / (static_cast<qreal>(selectedWidgets.size()) - 1.);
        qreal currentX = minLeft;

        for (auto *item : std::as_const(selectedWidgets)) {
            QRectF br = item->childRect();
            qreal dx = currentX - br.left();
            item->setPos(item->pos() + QPointF(dx, 0));
            currentX += br.width() + gap;
        }

        return;
    }

    // -------------------------------------------------------
    //  توزیع یکنواخت — عمودی
    // -------------------------------------------------------
    if (mode == AlignMode::DistributeVertically) {
        std::sort(selectedWidgets.begin(), selectedWidgets.end(), [](auto *a, auto *b) {
            return a->childRect().top() < b->childRect().top();
        });

        qreal totalHeight = 0;
        for (auto *item : std::as_const(selectedWidgets))
            totalHeight += item->childRect().height();

        qreal space = (maxBottom - minTop) - totalHeight;
        if (space < 0)
            space = 0;

        qreal gap = space / (static_cast<qreal>(selectedWidgets.size()) - 1.);
        qreal currentY = minTop;

        for (auto *item : std::as_const(selectedWidgets)) {
            QRectF br = item->childRect();
            qreal dy = currentY - br.top();
            item->setPos(item->pos() + QPointF(0, dy));
            currentY += br.height() + gap;
        }

        return;
    }

    // -------------------------------------------------------
    //  تراز معمولی
    // -------------------------------------------------------
    for (auto *item : std::as_const(selectedWidgets)) {
        QRectF br = item->childRect();
        QPointF pos = item->pos();

        qreal dx = 0;
        qreal dy = 0;

        switch (mode) {
        case AlignMode::AlignLeft:
            dx = minLeft - br.left();
            break;

        case AlignMode::AlignRight:
            dx = maxRight - br.right();
            break;

        case AlignMode::AlignTop:
            dy = minTop - br.top();
            break;

        case AlignMode::AlignBottom:
            dy = maxBottom - br.bottom();
            break;

        case AlignMode::AlignCenterHorizontally:
            dx = avgCenterX - br.center().x();
            break;

        case AlignMode::AlignCenterVertically:
            dy = avgCenterY - br.center().y();
            break;

        case AlignMode::MakeSameWidth:
            item->setWidth(refW);
            break;

        case AlignMode::MakeSameHeight:
            item->setHeight(refH);
            break;

        case AlignMode::MakeSameSize:
            item->setWidth(refW);
            item->setHeight(refH);
            break;

        default:
            break;
        }

        if (dx != 0 || dy != 0)
            item->setPos(pos + QPointF(dx, dy));
    }

    setIsModified(true);
}

void Scene::setTool(AbstractTool *newTool)
{
    if (dd->tool)
        dd->tool->deactivate();
    dd->tool = newTool;
    if (dd->tool)
        dd->tool->activate();
}

void Scene::setTool(const QString &className)
{
    if (!dd->toolNames.contains(className))
        return;
    setTool(dd->toolNames[className]);
}

QList<RelationWidget *> Scene::relations() const
{
    return dd->relations;
}

Scene::GridType Scene::gridType() const
{
    return dd->gridType;
}

void Scene::setGridType(GridType newGridType)
{
    dd->gridType = newGridType;
    createBackgroundBrush();
    update();
}

void Scene::createBackgroundBrush()
{
    QImage img{dd->gridSize.toSize(), QImage::Format_ARGB32};
    QPainter p{&img};
    p.setPen(QPen(QColor(180, 190, 220)));

    p.fillRect(QRect{0,
                     0,
                     static_cast<int>(dd->gridSize.width()),
                     static_cast<int>(dd->gridSize.height())},
               Qt::red);

    switch (dd->gridType) {
    case GridType::Grid:
        p.drawLine(0, 0, static_cast<int>(dd->gridSize.width()), 0);
        p.drawLine(0, 0, 0, static_cast<int>(dd->gridSize.height()));
        break;
    case GridType::Dot:
        p.drawPoint(0, 0);
        break;
    case GridType::None:
        break;
    }
    p.end();
    QBrush brush{img};
    setBackgroundBrush(brush);
}

void Scene::setTooltipText(const QString &text)
{
    dd->tooltipText->setPlainText(text);
}

void Scene::setTooltipPos(const QPointF &pos)
{
    dd->tooltipText->setPos(pos);
    QRectF textRect = dd->tooltipText->boundingRect();

    dd->tooltipRect->setRect(textRect.adjusted(-4, -2, 4, 2));
    dd->tooltipRect->setPos(pos);
}

void Scene::showTooltip(const QString &text, const QPointF &pos)
{
    if (!text.isNull())
        setTooltipText(text);
    if (!pos.isNull())
        setTooltipPos(pos);
    dd->tooltipRect->show();
    dd->tooltipText->show();
}

void Scene::hideTooltip()
{
    dd->tooltipRect->hide();
    dd->tooltipText->hide();
}

// void Scene::drawBackground(QPainter *painter, const QRectF &rect)
//{
//     constexpr int dd->gridSize{15};
//     QPen penPage(Qt::black, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
//     QBrush brushPage(Qt::white);
//     painter->setBrush(brushPage);
//     painter->setPen(penPage);

//    QRectF rc = rect;

//         // draw page frame
//    painter->fillRect(rc, Qt::white);
//    painter->drawRect(rc);

//         // set margins

//    if (dd->gridType == GridType::Dot)
//        for (int i = rc.left(); i <= rc.right(); i = i + dd->gridSize)
//        for (int j = rc.top(); j <= rc.bottom(); j = j + dd->gridSize)
//            painter->drawPoint(i, j);

//    if (dd->gridType == GridType::Grid) {
//        painter->setPen(QPen(Qt::lightGray));
//        for (int i = rc.left(); i <= rc.right(); i = i + dd->gridSize)
//        painter->drawLine(i, rc.top(), i, rc.bottom());
//        for (int j = rc.top(); j <= rc.bottom(); j = j + dd->gridSize)
//        painter->drawLine(rc.left(), j, rc.right(), j);
//    }//if

//         // draw inside margins
//    painter->setBrush(Qt::NoBrush);
//    painter->setPen(QPen(QColor(180, 190, 220)));
//    painter->drawRect(rc);
//}

QList<AbstractWidget *> Scene::widgets() const
{
    return dd->widgets;
}

QPointF Scene::snapPoint(const QPointF &pt) const
{
    if (!dd->snapToGrid)
        return pt;

    if (dd->gridSize.width() <= 0 || dd->gridSize.height() <= 0)
        return pt;
    // auto p = QPointF{static_cast<qreal>((pt.x() / dd->gridSize.width()) * dd->gridSize.width()),
    //                  static_cast<qreal>((pt.y() / dd->gridSize.height()) * dd->gridSize.height())};
    // qDebug() << pt << p << dd->gridSize;
    // return pt;

    qreal x = std::round(pt.x() / dd->gridSize.width()) * dd->gridSize.width();
    qreal y = std::round(pt.y() / dd->gridSize.height()) * dd->gridSize.height();

    return QPointF(x, y);
}

void Scene::addWidget(AbstractWidget *widget)
{
    setNameForWidget(widget);
    widget->setZValue(Z::Widgets);
    // widget->_id = QUuid::createUuid();
    dd->widgets.append(widget);
    dd->widgetsById.insert(widget->id(), widget);
    addItem(widget);
    connect(widget, &AbstractWidget::doubleClicked, this, &Scene::widget_doubleClicked);
    connect(widget, &AbstractWidget::moved, this, &Scene::widget_moved);
    connect(widget, &AbstractWidget::selectedChanged, this, &Scene::widget_selectedChanged);
    setIsModified(true);

    emit widgetAdded(widget);
}

void Scene::addWidgetOnce(AbstractWidget *widget)
{
    auto i = std::find_if(dd->widgets.begin(),
                          dd->widgets.end(),
                          [&widget](AbstractWidget *w) { return w == widget; });
    if (i == dd->widgets.end())
        addWidget(widget);
}

void Scene::removeWidget(AbstractWidget *widget)
{
    removeItem(widget);
    dd->widgets.removeOne(widget);
    dd->widgetsById.remove(widget->id());
    emit widgetRemoved(widget);
    // widget->deleteLater();
}

void Scene::addRelation(RelationWidget *relation)
{
    addItem(relation);
    dd->relations << relation;
}

void Scene::removeRelation(RelationWidget *relation)
{
    removeItem(relation);
    dd->relations.removeOne(relation);
}

QMarginsF Scene::margins() const
{
    return dd->margins;
}

void Scene::setMargins(const QMarginsF &newMargins)
{
    if (dd->margins == newMargins)
        return;
    dd->margins = newMargins;
    emit marginsChanged();
}

QColor Scene::gridColor() const
{
    return dd->gridColor;
}

void Scene::setGridColor(const QColor &newGridColor)
{
    if (dd->gridColor == newGridColor)
        return;
    dd->gridColor = newGridColor;
    emit gridColorChanged();
}

QColor Scene::backgroundColor() const
{
    return dd->backgroundColor;
}

void Scene::setBackgroundColor(const QColor &newBackgroundColor)
{
    if (dd->backgroundColor == newBackgroundColor)
        return;
    dd->backgroundColor = newBackgroundColor;
    update();
    emit backgroundColorChanged();
}

qreal Scene::gridOpacity() const
{
    return dd->gridOpacity;
}

void Scene::setGridOpacity(qreal newGridOpacity)
{
    if (qFuzzyCompare(dd->gridOpacity, newGridOpacity))
        return;
    dd->gridOpacity = newGridOpacity;
    emit gridOpacityChanged();
}

QSizeF Scene::gridSize() const
{
    return dd->gridSize;
}

void Scene::setGridSize(const QSizeF &newGridSize)
{
    if (dd->gridSize == newGridSize)
        return;
    dd->gridSize = newGridSize;
    emit gridSizeChanged();
}

bool Scene::snapToGrid() const
{
    return dd->snapToGrid;
}

void Scene::setSnapToGrid(bool newSnapToGrid)
{
    if (dd->snapToGrid == newSnapToGrid)
        return;
    dd->snapToGrid = newSnapToGrid;
    emit snapToGridChanged();
}

AbstractWidget *Scene::createWidget(const QString &name, const QUuid &id)
{
    return dd->createWidget(name, id);
}

void Scene::createWidget(const QString &name, const QRectF &rect, const QUuid &id)
{
    auto cmd = new AddWidgetsCommand(dd.data());
    cmd->add(name, rect, {}, id);
    dd->undoStack->push(cmd);
    ;
}

BatchCreator *Scene::createBatchCreator()
{
    return new BatchCreator{dd.data()};
}

void GraphView::Scene::registerWidget(const QString &name, const CreatorFunction &fun)
{
    dd->creators.insert(name, fun);
}

bool Scene::isModified() const
{
    return dd->isModified;
}

void Scene::setIsModified(bool newIsModified)
{
    // if (dd->isModified == newIsModified)
    // return;
    dd->isModified = newIsModified;
    emit isModifiedChanged();
}

QJsonArray Scene::dumpSelectedWidgets() const
{
    QJsonArray arr;
    auto list = selectedWidgets();
    for (auto w : std::as_const(list)) {
        QJsonObject map;
        w->saveState(map);
        arr << map;
    }

    return arr;
}

QList<AbstractWidget *> Scene::pasteWidgets(const QJsonArray &array)
{
    QList<AbstractWidget *> ret;
    clearSelection();

    for (const auto &item : array) {
        auto o = item.toObject();
        auto w = dd->createWidget(o["__className"].toString());
        if (w) {
            w->restoreState(o);
            w->setSelected(true);
            setNameForWidget(w);
            ret << w;
        } else {
            qWarning() << "Unable to create widget" << o["__className"].toString();
        }
    }
    return ret;
}

AbstractWidget *Scene::cloneWidget(AbstractWidget *widget)
{
    QJsonObject map;
    widget->saveState(map);
    auto w = dd->createWidget(widget->className());
    if (!w) {
        qWarning() << "Couldn't duplicate the widget" << widget->className();
        return nullptr;
    }
    w->restoreState(map);
    setNameForWidget(w, true);

    clearSelection();
    w->setSelected(true);
    return w;
}

void Scene::cloneSelectedWidgets()
{
    auto cmd = new CloneWidgetsCommand{this};
    dd->undoStack->push(cmd);
    // auto data = dumpSelectedWidgets();
    // auto widgets = pasteWidgets(data);

    // clearSelection();
    // for (auto &w : widgets) {
    //     w->setPos(w->pos().x() + 20, w->pos().y() + 20);
    //     setNameForWidget(w, true);
    //     w->setSelected(true);
    // }
    // return widgets;
}

void Scene::cutSelectedToClipboard()
{
    copySelectedToClipboard();
    auto cmd = new RemoveWidgetCommand{this, selectedWidgets()};
    clearSelection();
    pushCommand(cmd);
}

void Scene::copySelectedToClipboard()
{
    auto data = dumpSelectedWidgets();

    if (!data.size())
        return;

    auto json = QJsonDocument{data}.toJson();
    qApp->clipboard()->setText(json);
}

void Scene::pasteFromClipboard()
{
    auto doc = QJsonDocument::fromJson(qApp->clipboard()->text().toUtf8());
    if (!doc.isArray())
        return;
    pasteWidgets(doc.array());
}

void Scene::sendSelectedToBack()
{
    auto widgets = allWidgets();
    auto selectedWidgets = this->selectedWidgets();
    std::sort(widgets.begin(),
              widgets.end(),
              [](AbstractWidget *w1, AbstractWidget *w2) {
                  if (w1->zValue() == w2->zValue())
                      return true;
                  return w1->zValue() < w2->zValue();
              });

    auto z{Z::Widgets};

    for (auto &w : selectedWidgets)
        w->setZValue(z++);

    for (auto &w : widgets)
        if (selectedWidgets.contains(w))
            continue;
        else
            w->setZValue(z++);
}

void Scene::bringSelectedToFront()
{
    auto widgets = allWidgets();
    auto selectedWidgets = this->selectedWidgets();
    std::sort(widgets.begin(),
              widgets.end(),
              [](AbstractWidget *w1, AbstractWidget *w2) {
                  if (w1->zValue() == w2->zValue())
                      return true;
                  return w1->zValue() < w2->zValue();
              });

    auto z{Z::Widgets};

    for (auto &w : widgets)
        if (selectedWidgets.contains(w))
            continue;
        else
            w->setZValue(z++);

    for (auto &w : selectedWidgets)
        w->setZValue(z++);
}

AbstractWidget *Scene::widgetById(const QUuid &id) const
{
    return dd->widgetsById[id];
}

ScenePrivate::ScenePrivate(Scene *scene)
    : scene{scene}
    , undoStack{new QUndoStack{scene}}
{}

AbstractWidget *ScenePrivate::createWidget(const QString &name, const QUuid &id)
{
    if (!creators.contains(name))
        return nullptr;
    auto w = creators[name]();
    w->_className = name;

    if (id.isNull())
        w->_uuid = QUuid::createUuid();
    else
        w->_uuid = id;
    scene->addWidget(w);
    return w;
}

void ScenePrivate::removeWidget(AbstractWidget *widget)
{
    scene->removeItem(widget);
    widgets.removeOne(widget);
    widgetsById.remove(widget->id());
    emit scene->widgetRemoved(widget);
    widget->deleteLater();
}

void ScenePrivate::setNameForWidget(AbstractWidget *widget, bool force)
{
    if (!widget->objectName().isEmpty() && !force)
        return;

    auto className = widget->_className;
    className[0] = className[0].toLower();
    setNameForWidget(widget, className);
}

void ScenePrivate::setNameForWidgetShort(AbstractWidget *widget, bool force)
{
    if (!widget->objectName().isEmpty() && !force)
        return;

    auto className = widget->_className;
    QString shortName;

    for (auto ch : std::as_const(className))
        if (ch.isUpper())
            shortName.append(ch);

    setNameForWidget(widget, shortName);
}

void ScenePrivate::setNameForWidget(AbstractWidget *widget, const QString &perfix)
{
    QStringList names;
    auto items = scene->items();
    for (auto &item : items) {
        auto w = dynamic_cast<AbstractWidget *>(item);
        if (w)
            names << w->objectName();
    }

    int index{1};

    while (names.contains(perfix + QString::number(index)))
        index++;

    widget->setObjectName(perfix + QString::number(index));
}

void ScenePrivate::drawBackgroundPixmap(QPainter *painter)
{
    switch (backgroundFillMode) {
    case GraphView::Scene::BackgroundFillMode::NoFill:
        painter->drawPixmap(0, 0, backgroundPixmap);
        break;
    case GraphView::Scene::BackgroundFillMode::Center: {
        QRectF sceneRect = scene->sceneRect();
        QPointF center = sceneRect.center();
        QPointF topLeft = center
                          - QPointF(backgroundPixmap.width() / 2.0, backgroundPixmap.height() / 2.0);
        painter->drawPixmap(topLeft, backgroundPixmap);
        break;
        break;
    }
    case GraphView::Scene::BackgroundFillMode::Stretch: {
        QRectF sceneRect = scene->sceneRect();
        QPixmap scaledPixmap = backgroundPixmap.scaled(sceneRect.size().toSize(),
                                                       Qt::IgnoreAspectRatio,
                                                       Qt::SmoothTransformation);
        painter->drawPixmap(sceneRect.topLeft(), scaledPixmap);
        break;
    }
    case GraphView::Scene::BackgroundFillMode::Repeat: {
        // تصویر به صورت کاشی‌وار تکرار می‌شود
        QRectF sceneRect = scene->sceneRect();
        int pixWidth = backgroundPixmap.width();
        int pixHeight = backgroundPixmap.height();

        // محاسبه تعداد کاشی‌های مورد نیاز برای پوشش کل صحنه
        int cols = static_cast<int>(std::ceil(sceneRect.width() / pixWidth));
        int rows = static_cast<int>(std::ceil(sceneRect.height() / pixHeight));

        // رسم کاشی‌ها از گوشه سمت چپ بالای صحنه
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                QPointF pos = sceneRect.topLeft() + QPointF(col * pixWidth, row * pixHeight);
                painter->drawPixmap(pos, backgroundPixmap);
            }
        }
        break;
    }
    }
}

QPixmap Scene::backgroundPixmap() const
{
    return dd->backgroundPixmap;
}

void Scene::setBackgroundPixmap(const QPixmap &newBackgroundPixmap)
{
    dd->backgroundPixmap = newBackgroundPixmap;
    update();
    emit backgroundPixmapChanged();
}

Scene::BackgroundFillMode Scene::backgroundFillMode() const
{
    return dd->backgroundFillMode;
}

void Scene::setBackgroundFillMode(BackgroundFillMode newBackgroundFillMode)
{
    if (dd->backgroundFillMode == newBackgroundFillMode)
        return;
    dd->backgroundFillMode = newBackgroundFillMode;
    emit backgroundFillModeChanged();
}

} // namespace GraphView
