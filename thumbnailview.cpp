#include "thumbnailview.h"

#include <QGraphicsItem>
#include <QMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <scene.h>

#include "handles/abstracthandle.h"

namespace GraphView {

ThumbnailView::ThumbnailView(QWidget *parent)
    : QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setInteractive(false);
    setDragMode(QGraphicsView::NoDrag);

    setOptimizationFlag(IndirectPainting);

    setProperty("__noZoom", true);
    setProperty("hideHandles", true);
}

void ThumbnailView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    doFit();
}

void ThumbnailView::updateFit()
{
    doFit();
}

void ThumbnailView::doFit()
{
    if (!scene())
        return;

    QRectF rect = scene()->sceneRect();
    if (rect.isEmpty())
        return;

    fitInView(rect, Qt::KeepAspectRatio);
}

void ThumbnailView::drawItems(QPainter *painter,
                              int numItems,
                              QGraphicsItem *items[],
                              const QStyleOptionGraphicsItem options[])
{
    for (int i = 0; i < numItems; ++i) {
        if (!items[i]->isVisible())
            continue;

        auto handle = dynamic_cast<GraphView::Handles::AbstractHandle *>(items[i]);
        if (handle)
            continue;

        painter->save();
        painter->setTransform(items[i]->sceneTransform(), true);
        items[i]->paint(painter, &options[i], this);
        painter->restore();
    }
}

void ThumbnailView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();
}

void ThumbnailView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
    auto sc = qobject_cast<GraphView::Scene *>(scene());

    if (sc) {
        painter->setBrush(sc->backgroundColor());
        painter->fillRect(rect, sc->backgroundColor());
    } else {
        painter->setBrush(backgroundBrush());
        painter->fillRect(rect, backgroundBrush());
    }
}

void ThumbnailView::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit doubleClicked();
}

} // namespace GraphView
