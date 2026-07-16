#pragma once

#include "widgets/abstractrectwidget.h"
#include "graphviewglobal.h"

namespace GraphView::Widgets
{

class Relation;
class ConnectionHandle : public AbstractRectWidget
{
    // Q_OBJECT
public:
    enum { Type = UserType + 4 };

    int type() const override
    {
        return Type;
    }
    explicit ConnectionHandle(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QPointF handleScenePos() const;

    bool isSelected() const;
    void setIsSelected(bool newIsSelected);

    Qt::Edge connectionEdge() const;
    void setConnectionEdge(Qt::Edge newConnectionEdge);

    ConnectionType connectionType() const;
    void setConnectionType(ConnectionType newConnectionType);

    int index() const;
    void setIndex(int newIndex);

    static constexpr qreal lenght{30};
    static constexpr qreal thikness{20};

    Relation *relation() const;
    void setRelation(Relation *newRelation);

    QPointF connectionPoint() const;
    QPointF sceneConnectionPoint() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Relation *_relation{nullptr};

    bool _isSelected{false};
    Qt::Edge _connectionEdge;
    ConnectionType _connectionType{Unset};
    int _index{0};
};
}
