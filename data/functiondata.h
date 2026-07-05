#pragma once

#include "abstractdata.h"

#include <QString>

namespace GraphView {

class FunctionData : public AbstractData
{
public:
    FunctionData();

    QString code() const;
    void setCode(const QString &newCode);

    QString name() const;
    void setName(const QString &newName);

    QString returnType() const;
    void setReturnType(const QString &newReturnType);

private:
    QString _code;
    QString _returnType;
    QString _name;
};

} // namespace GraphView
