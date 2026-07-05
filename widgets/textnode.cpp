// #include "connectionhandle.h"
// #include "textnode.h"
// #include "data/abstractdata.h"

// #include <QPainter>

// namespace GraphView
// {
// TextNode::TextNode(QGraphicsItem *parent)
//     : AbstractWidget(parent)
// {
// }

// void TextNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
// {
//     QRectF rc{0, 0, _size.width() - 1, _size.height() - 1};

//     painter->setPen(_borderColor);
//     painter->setBrush(_backgroundColor);
//     painter->drawRect(rc);

//     painter->setPen(_textColor);
//     painter->drawText(rc, _title, QTextOption(Qt::AlignCenter));
// }

// QColor TextNode::borderColor() const
// {
//     return _borderColor;
// }

// void TextNode::setBorderColor(const QColor &newBorderColor)
// {
//     _borderColor = newBorderColor;
// }

// QColor TextNode::backgroundColor() const
// {
//     return _backgroundColor;
// }

// void TextNode::setBackgroundColor(const QColor &newBackgroundColor)
// {
//     _backgroundColor = newBackgroundColor;
// }

// QColor TextNode::textColor() const
// {
//     return _textColor;
// }

// void TextNode::setTextColor(const QColor &newTextColor)
// {
//     _textColor = newTextColor;
// }

// float TextNode::fontSize() const
// {
//     return _fontSize;
// }

// void TextNode::setFontSize(float newFontSize)
// {
//     _fontSize = newFontSize;
// }

// QString TextNode::title() const
// {
//     return _title;
// }

// void TextNode::setTitle(const QString &newTitle)
// {
//     _title = newTitle;
// }

// Data::AbstractData *TextNode::data() const
// {
//     return _data;
// }

// void TextNode::setData(Data::AbstractData *newData)
// {
//     _data = newData;
//     connect(_data, &Data::AbstractData::reloaded, this, &TextNode::slotLoadData);
// }

// void TextNode::addConnection(ConnectionHandleWidget *handle)
// {
//     handle->setParentItem(this);
//     _handles << handle;
// }

// ConnectionHandleWidget *TextNode::addConnection(Qt::Edge edge)
// {
//     auto handle = new ConnectionHandleWidget{this};
//     handle->setConnectionEdge(edge);
//     addConnection(handle);
//     return handle;
// }

// ConnectionHandleWidget *TextNode::removeConnection(Qt::Edge edge)
// {
//     for (auto &h:_handles) {
//         if (h->connectionEdge() == edge) {
//             h->setParentItem(nullptr);
//             replaceHandles();
//             return h;
//         }
//     }
//     return nullptr;
// }

// int TextNode::connectionHandleIndex(ConnectionHandleWidget *handle) const
// {
//     return _handles.indexOf(handle);
// }

// ConnectionHandleWidget *TextNode::connectionHandle(int index) const
// {
//     if (index == -1)
//         return nullptr;
//     return _handles.at(index);
// }

// QList<ConnectionHandleWidget *> TextNode::connectionHandles(ConnectionType connectionType) const
// {
//     QList<ConnectionHandleWidget *> list;
//     for (auto &h : _handles)
//         if (h->connectionType() == connectionType)
//             list << h;
//     return list;
// }

// void TextNode::slotLoadData()
// {
//     reloadData();
// }

// void TextNode::reloadData()
// {
// }
// }
