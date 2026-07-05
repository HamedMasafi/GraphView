#pragma once

#include <QGraphicsView>

namespace GraphView {

class ThumbnailView : public QGraphicsView
{
    Q_OBJECT
public:
    ThumbnailView(QWidget *parent = nullptr);
    void doFit();

public slots:
    void updateFit();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void drawItems(QPainter *painter,
                   int numItems,
                   QGraphicsItem *items[],
                   const QStyleOptionGraphicsItem options[]) override;

    void mousePressEvent(QMouseEvent *) override {}
    void mouseMoveEvent(QMouseEvent *) override {}
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *) override {}
    void drawBackground(QPainter *painter, const QRectF &rect) override;

Q_SIGNALS:
    void clicked();
    void doubleClicked();
};

} // namespace GraphView
