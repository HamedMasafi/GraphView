#pragma once

#include "multiline.h"
#include "core/abstractserializable.h"

#include <QGraphicsItem>
#include <QPen>
namespace GraphView
{

class MoveEvent;
class ConnectionHandleWidget;
class AbstractWidget;
class ArrowHeadWidget;

class RelationWidget : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 7 };

    int type() const override
    {
        return Type;
    }
    class PointHolder : public AbstractSerializable
    {
    public:
        PointHolder(const QPointF &pt);
        PointHolder(ConnectionHandleWidget *handle);
        PointHolder operator=(const QPointF &pt);
        PointHolder operator=(ConnectionHandleWidget *handle);
        bool operator==(const QPointF &pt);
        bool operator==(ConnectionHandleWidget *handle);
        bool operator==(const PointHolder &other);

        QPointF point() const;
        ConnectionHandleWidget *handle() const;

        operator QPointF();
        operator const QPointF() const;

        QJsonObject saveJson() const override;
        bool loadJson(const QJsonObject &object) override;

    private:
        QPointF _pt;
        ConnectionHandleWidget *_handle;
    };

    RelationWidget(QGraphicsItem *parent = nullptr);
    RelationWidget(ConnectionHandleWidget *from, ConnectionHandleWidget *to);

    PointHolder &from();
    void setFrom(const PointHolder &newFrom);

    PointHolder &to();
    void setTo(const PointHolder &newTo);
    //    void setTo(const QPointF &pt);

    void reset(const PointHolder &newFrom, const PointHolder &newTo);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    void processMousePressEvent(const QPointF &pt);
    void processMouseMoveEvent(const QPointF &pt);
    void processMouseReleaseEvent(const QPointF &pt);

    bool isActive() const;
    void setActive(bool newisActive);

    MultiLineF poly() const;
    qreal distance(const QPointF &pt) const;
    bool intersect(const QPointF &pt, QPointF *intersectPoint, qreal maxDist = 4) const;
    void setPoly(const MultiLineF &newPoly);
    void reset();

private Q_SLOTS:
    void widget_moving(MoveEvent *);

private:
    void calculateArrow();
    void relocateLines();
    void normalizePoints();


    PointHolder _from{nullptr};
    PointHolder _to{nullptr};
    QPolygonF _arrowHead;
    //    QPointF _toPoint;
    MultiLineF _poly;
    QLineF _line;
    QPen _pen{QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)};
    QPen _activePen{QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)};

    //    ArrowHeadWidget *_arrow;
    bool _isActive{false};
    QList<QGraphicsLineItem *> _lines;

    friend class ConnectWidgetsTool;
};

}

QDebug operator<<(QDebug d, const GraphView::RelationWidget::PointHolder &p);
