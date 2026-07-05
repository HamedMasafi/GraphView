#pragma once

#include <QList>

#include "scene.h"
class QGraphicsRectItem;

class QPointF;
namespace GraphView {

class RelationWidget;
class AbstractWidget;

class ScenePrivate : public QSharedData
{
public:
    ScenePrivate(Scene *scene);

    Scene *scene;
    AbstractWidget *selectedWidget{nullptr};
    QList<AbstractWidget *> widgets;
    QList<RelationWidget *> relations;
    AbstractWidget *connectFromWidget{nullptr};
    QGraphicsRectItem *dragRect;
    QGraphicsRectItem *relationRect;
    RelationWidget *relationPreview;
    QPointF lastClickPos;
    Scene::Mode mode{Scene::Mode::Normal};
    AbstractTool *tool{nullptr};
    AbstractTool *tempTool{nullptr};
    QList<AbstractTool *> readyTools;
    QList<AbstractTool *> shadowTools;
    QMap<QString, AbstractTool *> toolNames;
    QMap<QString, Scene::CreatorFunction> creators;
    GraphView::View *view{nullptr};
    QString dragCreator{};

    QGraphicsRectItem *tooltipRect;
    QGraphicsTextItem *tooltipText;

    QUndoStack *undoStack;
    QList<AbstractWidget *> selectedWidgets;

    Scene::GridType gridType{Scene::GridType::Grid};
    //    WidgetResizer *resizer;
    QSizeF gridSize{10, 10};
    QMarginsF margins{10, 10, 10, 10};
    QColor gridColor{Qt::gray};
    QColor backgroundColor{Qt::white};
    QPixmap backgroundPixmap;
    Scene::BackgroundFillMode backgroundFillMode{Scene::BackgroundFillMode::NoFill};

    qreal gridOpacity{0.8};
    bool snapToGrid{true};
    bool isModified{false};
    QMap<QUuid, AbstractWidget *> widgetsById;

    AbstractWidget *createWidget(const QString &name, const QUuid &id = {});
    void removeWidget(AbstractWidget *widget);

    void setNameForWidget(AbstractWidget *widget, bool force);
    void setNameForWidgetShort(AbstractWidget *widget, bool force);
    void setNameForWidget(AbstractWidget *widget, const QString &perfix);
    void drawBackgroundPixmap(QPainter *painter);
};

} // namespace GraphView
