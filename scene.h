#pragma once

#include <QGraphicsScene>
#include <QMap>
#include <QUuid>

class QUndoStack;
class QUndoCommand;

class QMimeData;

class QDrag;
namespace GraphView {

class BatchCreator;
class ScenePrivate;

namespace Commands {
class AddWidget;
}
class View;
namespace Widgets {
class AbstractWidget;
class Relation;
class ConnectionHandle;
} // namespace Widgets
namespace Tools {
class AbstractTool;
}
class Scene : public QGraphicsScene
{
    Q_OBJECT

    Q_PROPERTY(QMarginsF margins READ margins WRITE setMargins NOTIFY marginsChanged FINAL)
    Q_PROPERTY(GridType gridType READ gridType WRITE setGridType NOTIFY gridTypeChanged FINAL)
    Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor NOTIFY gridColorChanged FINAL)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY
                   backgroundColorChanged FINAL)
    Q_PROPERTY(QPixmap backgroundPixmap READ backgroundPixmap WRITE setBackgroundPixmap NOTIFY backgroundPixmapChanged FINAL)
    Q_PROPERTY(
        qreal gridOpacity READ gridOpacity WRITE setGridOpacity NOTIFY gridOpacityChanged FINAL)
    Q_PROPERTY(QSizeF gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged FINAL)
    Q_PROPERTY(bool snapToGrid READ snapToGrid WRITE setSnapToGrid NOTIFY snapToGridChanged FINAL)
    Q_PROPERTY(bool isModified READ isModified WRITE setIsModified NOTIFY isModifiedChanged FINAL)
    Q_PROPERTY(BackgroundFillMode backgroundFillMode READ backgroundFillMode WRITE setBackgroundFillMode NOTIFY backgroundFillModeChanged FINAL)

public:
    enum class AlignMode {
        AlignLeft,
        AlignRight,
        AlignTop,
        AlignBottom,

        AlignCenterHorizontally,
        AlignCenterVertically,

        MakeSameWidth,
        MakeSameHeight,
        MakeSameSize,

        DistributeHorizontally,
        DistributeVertically
    };

    enum class BackgroundFillMode {
        NoFill,
        Center,
        Stretch,
        Repeat
    };

    using CreatorFunction = std::function<Widgets::AbstractWidget *()>;

    enum class Mode { Normal, DragZone };
    enum class GridType { None, Dot, Grid };

    Scene(QObject *parent = nullptr);

    Scene(const QRectF &sceneRect, QObject *parent);

    Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

    QRect documentArea() const;

    ~Scene();

    // void addItem(QGraphicsItem *item) override
    // {
    //     QGraphicsScene::addItem(item);
    //     auto w = dynamic_cast<Widgets::AbstractWidget *>(item);
    //     if (w)
    //         emit widgetAdded(w);
    // }

    // void removeItem(QGraphicsItem *item) override
    // {
    //     QGraphicsScene::removeItem(item);
    //     auto w = dynamic_cast<Widgets::AbstractWidget *>(item);
    //     if (w)
    //         emit widgetRemoved(w);
    // }

    void addWidget(Widgets::AbstractWidget *widget);
    void addWidgetOnce(Widgets::AbstractWidget *widget);
    void removeWidget(Widgets::AbstractWidget *widget);

    void addRelation(Widgets::Relation *relation);
    void removeRelation(Widgets::Relation *relation);

    QList<Widgets::AbstractWidget *> widgets() const;
    QPointF snapPoint(const QPointF &pt) const;

    GridType gridType() const;
    void setGridType(GridType newGridType);

    void createBackgroundBrush();

    void setTooltipText(const QString &text);
    void setTooltipPos(const QPointF &pos);
    void showTooltip(const QString &text = {}, const QPointF &pos = {});
    void hideTooltip();

    QList<Widgets::Relation *> relations() const;

    Tools::AbstractTool *activeTool() const;
    Tools::AbstractTool *tool(const QString &name) const;

    void alignSelectedWidgets(AlignMode mode);

    template<class T>
    T *tool() const
    {
        return qobject_cast<T *>(tool(T::staticMetaObject.className()));
    }

    void setTool(Tools::AbstractTool *newTool);
    void setTool(const QString &className);

    template<class T>
    void setTool()
    {
        setTool(QString{T::staticMetaObject.className()});
    }

    void selectWidget(Widgets::AbstractWidget *s);

    void registerTool(Tools::AbstractTool *tool);

    template<class T>
    T *registerTool()
    {
        auto tool = new T{this};
        registerTool(tool);
        return tool;
    }

    void registerWidget(const QString &name, const CreatorFunction &fun);

    template<class T>
    void registerWidget(const QString &name)
    {
        CreatorFunction cb = []() { return new T; };
        registerWidget(name, cb);
    }

    template<class T>
    void registerWidget()
    {
        registerWidget<T>(T::staticMetaObject.className());
    }

    void beginDragDrop(const QString &className, const QSizeF &size = QSizeF{});
    QDrag *createDrag(const QString &className, const QSizeF &size = QSizeF{});

    GraphView::View *view() const;
    void setView(GraphView::View *newView);

    QUndoStack *undoStack() const;
    void pushCommand(QUndoCommand *cmd);

    QList<Widgets::AbstractWidget *> allWidgets() const;
    QList<Widgets::AbstractWidget *> selectedWidgets() const;

    QMarginsF margins() const;
    void setMargins(const QMarginsF &newMargins);

    QColor gridColor() const;
    void setGridColor(const QColor &newGridColor);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    qreal gridOpacity() const;
    void setGridOpacity(qreal newGridOpacity);

    QSizeF gridSize() const;
    void setGridSize(const QSizeF &newGridSize);

    bool snapToGrid() const;
    void setSnapToGrid(bool newSnapToGrid);

    GraphView::Widgets::AbstractWidget *createWidget(const QString &name, const QUuid &id = {});

    void createWidget(const QString &name, const QRectF &rect, const QUuid &id = {});

    BatchCreator *createBatchCreator();

    bool isModified() const;
    void setIsModified(bool newIsModified);
    GraphView::Widgets::AbstractWidget *widgetById(const QUuid &id) const;

    QJsonArray dumpSelectedWidgets() const;
    QList<GraphView::Widgets::AbstractWidget *> pasteWidgets(const QJsonArray &array);
    Widgets::AbstractWidget *cloneWidget(Widgets::AbstractWidget *widget);
    void cloneSelectedWidgets();

    void cutSelectedToClipboard();
    void copySelectedToClipboard();
    void pasteFromClipboard();

    void sendSelectedToBack();
    void bringSelectedToFront();
    void setNameForWidget(Widgets::AbstractWidget *widget, bool force = false);

    QPixmap backgroundPixmap() const;
    void setBackgroundPixmap(const QPixmap &newBackgroundPixmap);

    BackgroundFillMode backgroundFillMode() const;
    void setBackgroundFillMode(BackgroundFillMode newBackgroundFillMode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

    void drawBackground(QPainter *painter, const QRectF &rect) override;

Q_SIGNALS:
    void widgetAdded(GraphView::Widgets::AbstractWidget *widget);
    void widgetRemoved(GraphView::Widgets::AbstractWidget *widget);
    void widgetDropped(QRectF rect, QString name);
    void widgetDoubleClicked(GraphView::Widgets::AbstractWidget *widget);

    void createObjectRequested(QRectF rect);
    void mouseRelease(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMove(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePress(QGraphicsSceneMouseEvent *mouseEvent);
    void drop(QGraphicsSceneDragDropEvent *event);
    void widgetMoved(GraphView::Widgets::AbstractWidget *widget, QPointF lastPos, QPointF newPos);
    void widgetsConnectionRequested(GraphView::Widgets::ConnectionHandle *from,
                                    GraphView::Widgets::ConnectionHandle *to);
    void removeRequested(GraphView::Widgets::AbstractWidget *widget);

    void marginsChanged();
    void gridTypeChanged();
    void gridColorChanged();
    void backgroundColorChanged();
    void gridOpacityChanged();
    void gridSizeChanged();
    void snapToGridChanged();

    void isModifiedChanged();

    void backgroundPixmapChanged();

    void backgroundFillModeChanged();

private Q_SLOTS:
    void slotToolFinished();
    void widgets_resized();
    void widget_doubleClicked();
    void widget_selectedChanged();
    void widget_moved(QPointF lastPos);

private:
    void initBasicCommands();
    void createTooltipItem();

    // Widgets::AbstractWidget *_selectedWidget{nullptr};
    // QList<Widgets::AbstractWidget *> _widgets;
    // QList<Widgets::Relation *> _relations;
    // Widgets::AbstractWidget *_connectFromWidget{nullptr};
    // QGraphicsRectItem *_dragRect;
    // QGraphicsRectItem *_relationRect;
    // Widgets::Relation *_relationPreview;
    // QPointF _lastClickPos;
    // Mode _mode{Mode::Normal};
    // Tools::AbstractTool *_tool{nullptr};
    // Tools::AbstractTool *_tempTool{nullptr};
    // QList<Tools::AbstractTool *> _readyTools;
    // QList<Tools::AbstractTool *> _shadowTools;
    // QMap<QString, Tools::AbstractTool *> _toolNames;
    // QMap<QString, CreatorFunction> _creators;
    // GraphView::View *_view{nullptr};
    // QString _dragCreator{};

    // QGraphicsRectItem *_tooltipRect;
    // QGraphicsTextItem *_tooltipText;

    // QUndoStack *_undoStack;
    // QList<Widgets::AbstractWidget *> _selectedWidgets;

    // GridType _gridType{GridType::Grid};
    // //    Widgets::WidgetResizer *_resizer;
    // QSizeF _gridSize{10, 10};
    // QMarginsF m_margins{10, 10, 10, 10};
    // QColor m_gridColor{Qt::gray};
    // QColor m_backgroundColor{Qt::white};
    // qreal m_gridOpacity{0.8};
    // bool m_snapToGrid{true};
    // bool m_isModified{false};
    // QMap<QUuid, Widgets::AbstractWidget *> _widgetsById;

    QExplicitlySharedDataPointer<ScenePrivate> dd;
    friend class Tools::AbstractTool; // temp
    friend class Commands::AddWidget;
};
} // namespace GraphView
