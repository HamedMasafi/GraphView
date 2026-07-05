// #pragma once

// #include "abstractwidget.h"
// #include "graphviewglobal.h"

// namespace Data
// {
// class AbstractData;
// }

// namespace GraphView
// {
// class ConnectionHandleWidget;

// class TextNode : public AbstractWidget
// {
//     Q_OBJECT
// public:
//     enum { Type = UserType + 8 };

//     int type() const override
//     {
//         return Type;
//     }
//     explicit TextNode(QGraphicsItem *parent = nullptr);

//     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

//     Q_REQUIRED_RESULT QColor borderColor() const;
//     void setBorderColor(const QColor &newBorderColor);
//     QColor backgroundColor() const;
//     void setBackgroundColor(const QColor &newBackgroundColor);
//     QColor textColor() const;
//     void setTextColor(const QColor &newTextColor);
//     float fontSize() const;
//     void setFontSize(float newFontSize);

//     QString title() const;
//     void setTitle(const QString &newTitle);

//     Data::AbstractData *data() const;
//     void setData(Data::AbstractData *newData);

//     void addConnection(ConnectionHandleWidget *handle);
//     ConnectionHandleWidget *addConnection(Qt::Edge edge);

//     ConnectionHandleWidget *removeConnection(Qt::Edge edge);

//     int connectionHandleIndex(ConnectionHandleWidget *handle) const;
//     ConnectionHandleWidget *connectionHandle(int index) const;
//     QList<ConnectionHandleWidget *> connectionHandles(ConnectionType connectionType) const;

// private Q_SLOTS:
//     void slotLoadData();

// protected:
//     virtual void reloadData();
//     QColor _borderColor;
//     QColor _backgroundColor;
//     QColor _textColor;
//     float _fontSize{10};
//     QString _title;
//     Data::AbstractData *_data{nullptr};

// private:
//     void initLayout();
// };

// }
