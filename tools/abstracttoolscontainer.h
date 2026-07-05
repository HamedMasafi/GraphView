#pragma once

#include <QMap>
#include <QObject>

namespace GraphView
{
class Scene;
class View;
class AbstractTool;

namespace {
template <class _Parent>
struct AbstractToolChooset{
    using Type = void;
};
template <>
struct AbstractToolChooset<Scene>{
    using Type = AbstractTool;
};
template <>
struct AbstractToolChooset<View>{
    using Type = AbstractTool;
};
}

template <class _Parent>
class AbstractToolsContainer
{
public:
    using _Tool = typename AbstractToolChooset<_Parent>::Type;
    AbstractToolsContainer(_Parent *parent);

    template<class T>
    T *registerTool()
    {
        auto t = new T{_parent};
        _tools.insert(T::staticMetaObject.className(), t);
        return t;
    }

    virtual void toolActivated(AbstractTool *tool) = 0;

    void setTool(_Tool *tool);

    template<class T>
    void setTool(){
        setTool(_tools.value(T::staticMetaObject.className()));
    }

protected:
    _Tool *currentTool{nullptr};

private:
    QMap<QString, AbstractTool *> _tools;
    _Parent *_parent;
};

template<class _Parent>
Q_OUTOFLINE_TEMPLATE AbstractToolsContainer<_Parent>::AbstractToolsContainer(_Parent *parent)
    : _parent{parent}
{
}

template<class _Parent>
Q_OUTOFLINE_TEMPLATE void AbstractToolsContainer<_Parent>::setTool(_Tool *tool)
{
    if (tool){
        currentTool = tool;
        toolActivated(tool);
    }
}
}
