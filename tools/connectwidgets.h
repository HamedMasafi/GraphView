#pragma once

#include "abstracttool.h"

#include <QGraphicsRectItem>

namespace GraphView
{
namespace Widgets
{
class Relation;
class ConnectionHandle;
class CrossConnection;

class HilightRect : public QGraphicsRectItem
{
public:
    bool isObscuredBy(const QGraphicsItem *item) const;
};
}
namespace Tools
{
class ConnectWidgets : public AbstractTool
{
public:
    explicit ConnectWidgets(Scene *scene);

    bool accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent) override;
    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;
    QString text() const override;
    void deactivate() override;
    ToolType toolType() const override;

Q_SIGNALS:
    void finished(GraphView::Widgets::AbstractWidget *from, GraphView::Widgets::AbstractWidget *to);

private:
    enum class ConnectMode { None, FromHandle, FromLine };
    Q_ENUM(ConnectMode)
    enum class MoveType { None, NewRalation, ExistingRelation };
    Q_ENUM(MoveType)

    void nullAllSelectedItems();
    bool hilightHandleUnderCursor(QGraphicsSceneMouseEvent *mouseEvent, Widgets::AbstractWidget *&w, Widgets::ConnectionHandle *&ch);

    ConnectMode _connectMode{ConnectMode::None};
    QList<Widgets::Relation *> _sceneRelations;
    MoveType _moveType{MoveType::None};
    Widgets::Relation *_relationPreview;
    Widgets::CrossConnection *_crossConnection{nullptr};

    // common
    Widgets::ConnectionHandle *_destination{nullptr};

    // from widget
    Widgets::Relation *_relation{nullptr};
    Widgets::AbstractWidget *_connectSourceWidget{nullptr};
    Widgets::ConnectionHandle *_source{nullptr};

    // from relation
    Widgets::Relation *_sourceRelation{nullptr};
    QPointF _sourcePoint;

};

}
}
