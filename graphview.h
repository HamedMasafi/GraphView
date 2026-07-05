#pragma once

#include "tools/abstracttoolscontainer.h"
#include <QGraphicsView>
#include <functional>

class QUndoStack;
namespace GraphView
{
class Scene;

class AbstractWidget;
class ConnectionHandleWidget;
class AbstractTool;
class ConnectWidgetsTool;
class CreateWidgetTool;
class MagnifierTool;
class View : public QGraphicsView//, public AbstractToolsContainer<View>
{
Q_OBJECT

    using CreatorFunction = std::function<AbstractWidget *()>;

public:
    enum class Mode { Pointer, Hand, Rect, Relation };

    View(QWidget *parent = nullptr);

    Scene *scene() const;
    void setScene(Scene *newScene);

    AbstractTool *tool() const;
    void setTool(AbstractTool *tool);

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
    QAction *registerTool(AbstractTool *tool);

private Q_SLOTS:
    void scene_createObjectRequested(QRectF rect);
    void scene_widgetMoved(AbstractWidget *widget, QPointF lastPos, QPointF newPos);
    void scene_widgetDoubleClicked(AbstractWidget *widget);
    void scene_widgetsConnectionRequested(ConnectionHandleWidget *from, ConnectionHandleWidget *to);
    void scene_removeRequested(AbstractWidget *widget);

protected:
    struct ToolData {
        QString name;
        QAction *action;
        AbstractTool *tool;
    };
    Mode _mode{Mode::Pointer};

    void createTools();
    CreatorFunction _creator;
    Scene *_scene{nullptr};
    QList<QAction *> _actions;

    ConnectWidgetsTool *_relationTool;
    CreateWidgetTool *_createWidgetTool;
    MagnifierTool *_magnifierTool;
    void keyPressEvent(QKeyEvent *event) override;
    int _zoomLevel{100};
    QMap<QString, AbstractTool *> _tools;
};

}
