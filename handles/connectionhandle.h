#pragma once

#include "widgets/abstractrectwidget.h"
#include "graphviewglobal.h"

namespace GraphView
{

class RelationWidget;
class ConnectionHandleWidget : public AbstractRectWidget
{
    // Q_OBJECT
public:
    enum { Type = UserType + 4 };

    int type() const override
    {
        return Type;
    }
    explicit ConnectionHandleWidget(QGraphicsItem *parent = nullptr);

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

    RelationWidget *relation() const;
    void setRelation(RelationWidget *newRelation);

    QPointF connectionPoint() const;
    QPointF sceneConnectionPoint() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    RelationWidget *_relation{nullptr};

    bool _isSelected{false};
    Qt::Edge _connectionEdge;
    ConnectionType _connectionType{Unset};
    int _index{0};
};
}
