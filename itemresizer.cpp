#include "itemresizer.h"
#include "widgets/abstractwidget.h"
#include "event.h"

#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

#define CIRCLER 10

namespace GraphView::Widgets
{

ResizeHandle::ResizeHandle(qreal radius, QGraphicsItem *parent)
    : QGraphicsRectItem(-radius / 2, -radius / 2, radius, radius, parent)
    , _radius(radius)
{
    setPen(QPen(Qt::black, 0));
    // this->setFlags( QGraphicsItem::ItemIsMovable );
}

void ResizeHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mousePressEvent(event);
    qDebug() << "handle Pressed";

    _lastPoint = centerPoint();
    mousePressed = true;
//    grabMouse();
}

void ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "1" << mousePressed;
    if (mousePressed) {
        QPointF pt(this->pos().x() - _lastPoint.x(), this->pos().y() - _lastPoint.y());
        emit moving(this->mapToScene(event->pos()));
    } // if

    QGraphicsRectItem::mouseMoveEvent(event);
}

void ResizeHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Released";
    Q_UNUSED(event);
    emit moved();
    mousePressed = false;
    QGraphicsRectItem::mouseReleaseEvent(event);
//    ungrabMouse();
}

QPointF ResizeHandle::centerPoint() const
{
    return QPointF(pos().x() + _radius / 2, pos().y() + _radius / 2);
}

QPointF ResizeHandle::lastPoint() const
{
    return _lastPoint;
}

ResizeDirection ResizeHandle::resizeDirection() const
{
    return m_resizeDirection;
}

void ResizeHandle::setResizeDirection(ResizeDirection value)
{
    m_resizeDirection = value;
}

void ResizeHandle::setScale(qreal scale)
{
    _radius = scale;
    this->setRect(0, 0, scale, scale);
}

WidgetResizer::WidgetResizer(QGraphicsScene *parent)
    : QObject()
    , parentScene(parent)
    ,
    //_parent( 0 ),
      m_scale(1)
{
    resizerTL = new ResizeHandle{CIRCLER};
    resizerT = new ResizeHandle{CIRCLER};
    resizerTR = new ResizeHandle{CIRCLER};
    resizerL = new ResizeHandle{CIRCLER};
    resizerR = new ResizeHandle{CIRCLER};
    resizerBL = new ResizeHandle{CIRCLER};
    resizerB = new ResizeHandle{CIRCLER};
    resizerBR = new ResizeHandle{CIRCLER};

    /*parent->addItem( resizerTL );
    parent->addItem( resizerT  );
    parent->addItem( resizerTR );
    parent->addItem( resizerL  );
    parent->addItem( resizerR  );
    parent->addItem( resizerBL );
    parent->addItem( resizerB  );
    parent->addItem( resizerBR );*/

    handles.append(resizerTL);
    handles.append(resizerT);
    handles.append(resizerTR);
    handles.append(resizerL);
    handles.append(resizerR);
    handles.append(resizerBL);
    handles.append(resizerB);
    handles.append(resizerBR);

    resizerTL->setPen(QPen(Qt::black));

    setVisible(false);

    resizerTL->setCursor(Qt::SizeFDiagCursor);
    resizerT->setCursor(Qt::SizeVerCursor);
    resizerTR->setCursor(Qt::SizeBDiagCursor);
    resizerL->setCursor(Qt::SizeHorCursor);
    resizerR->setCursor(Qt::SizeHorCursor);
    resizerBL->setCursor(Qt::SizeBDiagCursor);
    resizerB->setCursor(Qt::SizeVerCursor);
    resizerBR->setCursor(Qt::SizeFDiagCursor);

    resizerT->setResizeDirection(Top);
    resizerL->setResizeDirection(Left);
    resizerR->setResizeDirection(Right);
    resizerB->setResizeDirection(Bottom);

    resizerTR->setResizeDirection(Top | Right);
    resizerTL->setResizeDirection(Top | Left);
    resizerBR->setResizeDirection(Bottom | Right);
    resizerBL->setResizeDirection(Bottom | Left);

    for (auto &handle : handles) {
        /*QRadialGradient gradient( CIRCLER, CIRCLER, 270);
        gradient.setColorAt(0, QColor::fromRgb(128, 128, 255) );
        gradient.setColorAt(1, QColor::fromRgb(255, 255, 255) );
        handles.at( i )->setBrush( QBrush(gradient) );*/

        handle->setBrush(QBrush(Qt::white));
        parent->addItem(handle);

        connect(handle, &ResizeHandle::moving, this, &WidgetResizer::handleMoving);

        connect(handle, &ResizeHandle::moved, this, &WidgetResizer::resized);

    } // for
}

WidgetResizer::~WidgetResizer()
{
}

void WidgetResizer::setVisible()
{
    setVisible(_selectedWidgets.count());
}

void WidgetResizer::setVisible(bool visible)
{
    for (int i = 0; i < handles.count(); i++)
        handles[i]->setVisible(visible);
}

void WidgetResizer::setActiveItem(Widgets::AbstractWidget *item)
{
    if (_selectedWidgets.contains(item))
        return;
    connect(item, &Widgets::AbstractWidget::moving, this, &WidgetResizer::widgetMoving);

    clear(true);
    _selectedWidgets.clear();
    _selectedWidgets.append(item);
    item->setSelected(true);

    refreshWidgets();

    selectionRect = item->childRect();
    resizeRect = item->childRect();
    setHandlesOnItem(selectionRect);
    setVisible();
}

void WidgetResizer::setHandlesOnItem(QRectF rc)
{
    // if( !_parent ) return;
    if (!_selectedWidgets.count())
        return;

    setResezeHandlePos(resizerTL, rc.topLeft());
    setResezeHandlePos(resizerT, rc.topRight(), rc.topLeft());
    setResezeHandlePos(resizerTR, rc.topRight());

    setResezeHandlePos(resizerL, rc.topLeft(), rc.bottomLeft());
    setResezeHandlePos(resizerR, rc.topRight(), rc.bottomRight());

    setResezeHandlePos(resizerBL, rc.bottomLeft());
    setResezeHandlePos(resizerB, rc.bottomRight(), rc.bottomLeft());
    setResezeHandlePos(resizerBR, rc.bottomRight());

    for (int i = 0; i < handles.count(); i++)
        handles.at(i)->setZValue(9999);
}

void WidgetResizer::setResezeHandlePos(ResizeHandle *handle, QPointF pos)
{
    qreal scaleHalf = m_scale * CIRCLER / 2;
    handle->setPos( //_selectedWidgets.at(0)->parentItem()->mapToParent(
        ////_parent->mapToParent (
        pos.x() - scaleHalf,
        pos.y() - scaleHalf);
}

void WidgetResizer::setResezeHandlePos(ResizeHandle *handle, QPointF pos1, QPointF pos2)
{
    qreal scaleHalf = m_scale * CIRCLER / 2;
    handle->setPos( //_selectedWidgets.at(0)->parentItem()->mapToParent(
        ////_parent->mapToParent (
        (pos2.x() + pos1.x()) / 2 - scaleHalf,
        (pos2.y() + pos1.y()) / 2 - scaleHalf);
}

void WidgetResizer::handleMoving(QPointF point)
{
    ResizeHandle *s = qobject_cast<ResizeHandle *>(sender());
    QRectF rc = resizeRect;

    Core::MoveEvent e(point, point);
    emit pointGridNeeded(&e);

    if (!e.isAccepted())
        return;

    if (s->resizeDirection() & Top)
        rc.setTop(e.point().y());
    if (s->resizeDirection() & Left)
        rc.setLeft(e.point().x());
    if (s->resizeDirection() & Right)
        rc.setRight(e.point().x());
    if (s->resizeDirection() & Bottom)
        rc.setBottom(e.point().y());

    /*if(_selectedWidgets.count() == 1){
        if(rc.width() < _selectedWidgets.at(0)->minimumSize().width())
            rc.setWidth(_selectedWidgets.at(0)->minimumSize().width());
        if(rc.height() < _selectedWidgets.at(0)->minimumSize().height())
            rc.setHeight(_selectedWidgets.at(0)->minimumSize().height());
    }*/

    if (rc.isValid()) {
        setResezeHandlePos(s, e.point());

        resizeRect = rc;
        proccessNewRect(rc);

        setHandlesOnItem(rc);
    } // if
}

qreal WidgetResizer::scale()
{
    return m_scale;
}
void WidgetResizer::setScale(qreal scale)
{
    m_scale = scale;
    for (int i = 0; i < handles.count(); i++)
        handles.at(i)->setScale(CIRCLER * scale);
}

void WidgetResizer::proccessNewRect(QRectF rc)
{
    /*if( _selectedWidgets.count() == 1 )
    {
       //rc.setTopLeft(
      //       _selectedWidgets.at(0)->parentItem()->mapFromScene( rc.topLeft() )
    );

       _selectedWidgets.at(0)->setPos( rc.topLeft() );
       _selectedWidgets.at(0)->setSize( rc.size() );

       return;
    }*/

    for (auto &widget : _selectedWidgets) {
        QRectF widgetRect = _widgetRects[widget];
        QSizeF sizeby(rc.width() / selectionRect.width(), rc.height() / selectionRect.height());

        widgetRect.moveTop(widgetRect.top() - selectionRect.top());
        widgetRect.moveLeft(widgetRect.left() - selectionRect.left());

        widgetRect.setTop(widgetRect.top() * sizeby.height());
        widgetRect.setBottom(widgetRect.bottom() * sizeby.height());
        widgetRect.setLeft(widgetRect.left() * sizeby.width());
        widgetRect.setRight(widgetRect.right() * sizeby.width());

        widgetRect.moveTop(widgetRect.top() + rc.top());
        widgetRect.moveLeft(widgetRect.left() + rc.left());

        widgetRect.moveTopLeft(widgetRect.topLeft());

        widget->setRect(widgetRect);
    }

    // selectionRect = rc;
}

void WidgetResizer::refresh()
{
    refreshWidgets();
    setVisible();
    refreshHandles();
}

void WidgetResizer::refreshHandles()
{
    setVisible();
}

QList<AbstractWidget *> WidgetResizer::selectedWidgets()
{
    return _selectedWidgets;
}

void WidgetResizer::clear()
{
    clear(true);
}

void WidgetResizer::clear(bool unselectWidgets)
{
    for (auto &widget : _selectedWidgets) {
        if (unselectWidgets)
            widget->setSelected(false);

        disconnect(widget, nullptr, this, nullptr);
    } // foreach

    _selectedWidgets.clear();
    setVisible(false);
}

void WidgetResizer::addWidget(AbstractWidget *widget)
{
    widget->setSelected(true);

    _selectedWidgets = {widget};
    connect(widget, &AbstractWidget::moving, this, &WidgetResizer::widgetMoving);
    setVisible();
}

void WidgetResizer::refreshWidgets()
{
    QRectF rc;

    if (_selectedWidgets.count() == 0)
        return;

    rc = _selectedWidgets.at(0)->childRect();
    for (auto &widget : _selectedWidgets) {
        _widgetRects[widget] = widget->childRect();
        QRectF widgetRect = widget->childRect(); // (widget->mapToScene(0, 0), widget->size());
        rc = rc.united(widgetRect);
    }

    selectionRect = rc;
    resizeRect = rc;
    setHandlesOnItem(rc);
}

void WidgetResizer::widgetMoving(Core::MoveEvent *event)
{
    Q_UNUSED(event)
    //    AbstractWidget *senderWidget = dynamic_cast<AbstractWidget*>(sender());
    //    QPointF step = senderWidget->mapFromScene(event->point());

    //    if (step.x() == 0 && step.y() == 0) return;

    //    foreach(AbstractWidget  *widget, _selectedWidgets)
    //    if (widget != senderWidget)
    //        widget->moveBy(step.x(), step.y());

    qDebug() << "Moving";
    setHandlesOnItem(_selectedWidgets.first()->childRect());
}

bool WidgetResizer::isWidgetSelected(AbstractWidget *widget)
{
    if (!_selectedWidgets.count())
        return false;

    for (auto &w: _selectedWidgets)
        if (widget == w)
            return true;

    return false;
}

QList<AbstractWidget *> WidgetResizer::selectedWidgets() const
{
    return _selectedWidgets;
}
}
