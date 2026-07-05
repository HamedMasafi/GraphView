#include "functiondata.h"

namespace GraphView {
FunctionData::FunctionData()
    : AbstractData{AbstractData::Type::Function}
{}

QString FunctionData::code() const
{
    return _code;
}

void FunctionData::setCode(const QString &newCode)
{
    _code = newCode;
}

QString FunctionData::name() const
{
    return _name;
}

void FunctionData::setName(const QString &newName)
{
    _name = newName;
}

QString FunctionData::returnType() const
{
    return _returnType;
}

void FunctionData::setReturnType(const QString &newReturnType)
{
    _returnType = newReturnType;
}
} // namespace GraphView
