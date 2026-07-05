#pragma once

#include <QList>

#include "scene.h"
class QGraphicsRectItem;

class QPointF;
namespace GraphView {

namespace Widgets {

class Relation;
class AbstractWidget;
} // namespace Widgets

class ScenePrivate : public QSharedData
{
public:
    ScenePrivate(Scene *scene);

    Scene *scene;
    Widgets::AbstractWidget *selectedWidget{nullptr};
    QList<Widgets::AbstractWidget *> widgets;
    QList<Widgets::Relation *> relations;
    Widgets::AbstractWidget *connectFromWidget{nullptr};
    QGraphicsRectItem *dragRect;
    QGraphicsRectItem *relationRect;
    Widgets::Relation *relationPreview;
    QPointF lastClickPos;
    Scene::Mode mode{Scene::Mode::Normal};
    Tools::AbstractTool *tool{nullptr};
    Tools::AbstractTool *tempTool{nullptr};
    QList<Tools::AbstractTool *> readyTools;
    QList<Tools::AbstractTool *> shadowTools;
    QMap<QString, Tools::AbstractTool *> toolNames;
    QMap<QString, Scene::CreatorFunction> creators;
    GraphView::View *view{nullptr};
    QString dragCreator{};

    QGraphicsRectItem *tooltipRect;
    QGraphicsTextItem *tooltipText;

    QUndoStack *undoStack;
    QList<Widgets::AbstractWidget *> selectedWidgets;

    Scene::GridType gridType{Scene::GridType::Grid};
    //    Widgets::WidgetResizer *resizer;
    QSizeF gridSize{10, 10};
    QMarginsF margins{10, 10, 10, 10};
    QColor gridColor{Qt::gray};
    QColor backgroundColor{Qt::white};
    QPixmap backgroundPixmap;
    Scene::BackgroundFillMode backgroundFillMode{Scene::BackgroundFillMode::NoFill};

    qreal gridOpacity{0.8};
    bool snapToGrid{true};
    bool isModified{false};
    QMap<QUuid, Widgets::AbstractWidget *> widgetsById;

    Widgets::AbstractWidget *createWidget(const QString &name, const QUuid &id = {});
    void removeWidget(Widgets::AbstractWidget *widget);

    void setNameForWidget(Widgets::AbstractWidget *widget, bool force);
    void setNameForWidgetShort(Widgets::AbstractWidget *widget, bool force);
    void setNameForWidget(Widgets::AbstractWidget *widget, const QString &perfix);
    void drawBackgroundPixmap(QPainter *painter);
};

} // namespace GraphView
