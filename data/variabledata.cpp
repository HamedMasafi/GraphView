#include "variabledata.h"

namespace Data {

VariableData::VariableData()
    : AbstractData{AbstractData::Type::Variable}
{}

VariableData::DataType VariableData::dataType() const
{
    return _dataType;
}

void VariableData::setDataType(DataType newDataType)
{
    _dataType = newDataType;
}

QString VariableData::name() const
{
    return _name;
}

void VariableData::setName(const QString &newName)
{
    _name = newName;
}

} // namespace Data
