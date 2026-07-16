#pragma once

#include "tools/abstracttoolscontainer.h"
#include <QGraphicsView>
#include <functional>

class QUndoStack;
namespace GraphView
{
class Scene;

namespace Widgets
{
class AbstractWidget;
class ConnectionHandle;
}
namespace Tools{
class AbstractTool;
class ConnectWidgets;
class CreateWidget;
class Magnifier;
}
class View : public QGraphicsView//, public Tools::AbstractToolsContainer<View>
{
Q_OBJECT

    using CreatorFunction = std::function<Widgets::AbstractWidget *()>;

public:
    enum class Mode { Pointer, Hand, Rect, Relation };

    View(QWidget *parent = nullptr);

    Scene *scene() const;
    void setScene(Scene *newScene);

    Tools::AbstractTool *tool() const;
    void setTool(Tools::AbstractTool *tool);

    void createDefaultCommands();

    QUndoStack *undoStack() const;

    Mode mode() const;
    void setMode(Mode newMode);

    template<class T>
    void createWidget()
    {
        auto c = []() {
            return new T;
        };
        createWidget(c);
    }
    template<class T, typename P>
    void createWidget(P *param)
    {
        auto c = [param]() {
            return new T{param};
        };
        createWidget(c);
    }
    void createWidget(CreatorFunction fn);

    int zoomLevel() const;
    void setZoomLevel(int newZoomLevel);

    template<class T>
    T *registerTool()
    {
        auto t = new T{_scene};
        registerTool(t);
        return t;
    }
    QAction *registerTool(Tools::AbstractTool *tool);

private Q_SLOTS:
    void scene_createObjectRequested(QRectF rect);
    void scene_widgetMoved(GraphView::Widgets::AbstractWidget *widget, QPointF lastPos, QPointF newPos);
    void scene_widgetDoubleClicked(GraphView::Widgets::AbstractWidget *widget);
    void scene_widgetsConnectionRequested(GraphView::Widgets::ConnectionHandle *from, GraphView::Widgets::ConnectionHandle *to);
    void scene_removeRequested(GraphView::Widgets::AbstractWidget *widget);

protected:
    struct ToolData {
        QString name;
        QAction *action;
        Tools::AbstractTool *tool;
    };
    Mode _mode{Mode::Pointer};

    void createTools();
    CreatorFunction _creator;
    Scene *_scene{nullptr};
    QList<QAction *> _actions;

    Tools::ConnectWidgets *_relationTool;
    Tools::CreateWidget *_createWidgetTool;
    Tools::Magnifier *_magnifierTool;
    void keyPressEvent(QKeyEvent *event) override;
    int _zoomLevel{100};
    QMap<QString, Tools::AbstractTool *> _tools;
};

}
