#pragma once

#include "abstracttool.h"

#include <QMap>

class QGraphicsRectItem;

namespace GraphView {
class MoveEvent;

class ResizeHandle;
class AbstractHandle;
class PolylineHandle;

class AbstractWidget;
class RelationWidget;

class SelectTool;
class SelectToolSubCommand
{
public:
    SelectToolSubCommand(SelectTool *parent);
    virtual void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) = 0;
    virtual void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) = 0;
    virtual void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) = 0;

protected:
    SelectTool *_parent;
};

class SelectTool : public AbstractTool
{
    Q_OBJECT

    Q_PROPERTY(QString selectedWidgetsGeometry READ selectedWidgetsGeometry WRITE setSelectedWidgetsGeometry NOTIFY selectedWidgetsGeometryChanged FINAL)

public:
    explicit SelectTool(Scene *scene);

    void activate() override;
    QString text() const override;

    void mousePressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClicked(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyReleaseEvent(QKeyEvent *event)override;

    void setSelectedWidget(AbstractWidget *newSelectedWidget);
    void setVisible(bool visible);

    QString selectedWidgetsGeometry() const;
    void sceneChanged(Scene *newScene) override;

Q_SIGNALS:
    void moving();
    void moved();
    void resized();

    void selectedWidgetsGeometryChanged(const QString &text);

private Q_SLOTS:
    void slotSceneSelectionChanged();
    void handleMoving(QPointF point);
    void handleMoved();

private:
    void setSelectedWidgetsGeometry(const QString &newSelectedWidgetsGeometry);
    void handleMoving(ResizeHandle *handle, QPointF point);
    void checkForPlaceWidgetInRelation();

    void moveSelectedWidgets(Qt::Key key, qreal distance);
    enum class Mode {
        None,
        MoveWidgets,
        MoveWidget,
        ResizeWidget,
        SelectWidgets,
        MoveRelation,
        MoveHandle
    };
    enum class SelectedItemType { None, Rect, Polygon };

    SelectedItemType _selectedItemType{SelectedItemType::None};

    Mode _mode{Mode::None};
    void initHandles();
    void setHandlesOnItem(const QRectF &rc);
    void setHandlesOnItem(const QPolygonF &poly);
    void setResezeHandlePos(ResizeHandle *handle, QPointF pos);
    void setResezeHandlePos(ResizeHandle *handle, QPointF pos1, QPointF pos2);

    AbstractWidget *_selectedWidget{nullptr};
    QRectF _initialRect;
    QPointF _objectInitialPos;
    QPointF _clickPos;
    QMap<AbstractWidget *, QPointF> _initialPositions;
    bool _wasMoved;
    bool m_isClonning;
    RelationWidget *_relation{nullptr};
    ResizeHandle *resizerTL, *resizerT, *resizerTR;
    ResizeHandle *resizerL, *resizerR;
    ResizeHandle *resizerBL, *resizerB, *resizerBR;
    QList<ResizeHandle *> rectResizeHandles;
    QList<PolylineHandle *> polyResizeHandles;
    AbstractHandle *_selectedHandle{nullptr};
    QPointF _itemClickPos;
    qreal m_scale{1};
    QRectF resizeRect;
    QGraphicsRectItem *_dragRect;

    SelectToolSubCommand *_currentSubCommand{nullptr};
    static constexpr int CIRCLER{10};

    QString m_selectedWidgetsGeometry;


};

} // namespace GraphView
