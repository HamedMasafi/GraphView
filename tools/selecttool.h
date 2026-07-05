#pragma once

#include "abstracttool.h"

#include <QMap>

class QGraphicsRectItem;

namespace GraphView {
namespace Core {
class MoveEvent;
}

namespace Handles {
class ResizeHandle;
class AbstractHandle;
class PolylineHandle;
} // namespace Handles
namespace Widgets {
class AbstractWidget;
class Relation;
} // namespace Widgets
namespace Tools {

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
namespace SelectToolImpl {
class ResizeHandle;
}

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

    void setSelectedWidget(Widgets::AbstractWidget *newSelectedWidget);
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
    void handleMoving(Handles::ResizeHandle *handle, QPointF point);
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
    void setResezeHandlePos(Handles::ResizeHandle *handle, QPointF pos);
    void setResezeHandlePos(Handles::ResizeHandle *handle, QPointF pos1, QPointF pos2);

    Widgets::AbstractWidget *_selectedWidget{nullptr};
    QRectF _initialRect;
    QPointF _objectInitialPos;
    QPointF _clickPos;
    QMap<Widgets::AbstractWidget *, QPointF> _initialPositions;
    bool _wasMoved;
    bool m_isClonning;
    Widgets::Relation *_relation{nullptr};
    Handles::ResizeHandle *resizerTL, *resizerT, *resizerTR;
    Handles::ResizeHandle *resizerL, *resizerR;
    Handles::ResizeHandle *resizerBL, *resizerB, *resizerBR;
    QList<Handles::ResizeHandle *> rectResizeHandles;
    QList<Handles::PolylineHandle *> polyResizeHandles;
    Handles::AbstractHandle *_selectedHandle{nullptr};
    QPointF _itemClickPos;
    qreal m_scale{1};
    QRectF resizeRect;
    QGraphicsRectItem *_dragRect;

    SelectToolSubCommand *_currentSubCommand{nullptr};
    static constexpr int CIRCLER{10};

    QString m_selectedWidgetsGeometry;


};

} // namespace Tools
} // namespace GraphView
