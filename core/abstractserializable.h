#pragma once

#include <QJsonObject>

namespace GraphView::Core
{
class AbstractSerializable
{
public:
    virtual QJsonObject saveJson() const = 0;
    virtual bool loadJson(const QJsonObject &object) = 0;
};

}
