#pragma once

#include "abstracttool.h"

#include <QGraphicsRectItem>

namespace GraphView
{
class RelationWidget;
class ConnectionHandleWidget;
class CrossConnection;

class HilightRectWidget : public QGraphicsRectItem
{
public:
    bool isObscuredBy(const QGraphicsItem *item) const;
};

class ConnectWidgetsTool : public AbstractTool
{
public:
    explicit ConnectWidgetsTool(Scene *scene);

    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;
    QString text() const override;
    void deactivate() override;
    ToolType toolType() const override;

Q_SIGNALS:
    void finished(AbstractWidget *from, AbstractWidget *to);

private:
    enum class ConnectMode { None, FromHandle, FromLine };
    Q_ENUM(ConnectMode)
    enum class MoveType { None, NewRalation, ExistingRelation };
    Q_ENUM(MoveType)

    void nullAllSelectedItems();
    bool hilightHandleUnderCursor(QGraphicsSceneMouseEvent *mouseEvent, AbstractWidget *&w, ConnectionHandleWidget *&ch);

    ConnectMode _connectMode{ConnectMode::None};
    QList<RelationWidget *> _sceneRelations;
    MoveType _moveType{MoveType::None};
    RelationWidget *_relationPreview;
    CrossConnection *_crossConnection{nullptr};

    // common
    ConnectionHandleWidget *_destination{nullptr};

    // from widget
    RelationWidget *_relation{nullptr};
    AbstractWidget *_connectSourceWidget{nullptr};
    ConnectionHandleWidget *_source{nullptr};

    // from relation
    RelationWidget *_sourceRelation{nullptr};
    QPointF _sourcePoint;

};

}
