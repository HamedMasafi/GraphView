#pragma once

#include <QJsonObject>

namespace GraphView
{
class AbstractSerializable
{
public:
    virtual QJsonObject saveJson() const = 0;
    virtual bool loadJson(const QJsonObject &object) = 0;
};

}
