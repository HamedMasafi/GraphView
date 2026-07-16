#pragma once

#include "abstractdata.h"

#include <QString>

namespace Data {

class VariableData : public AbstractData
{
    Q_OBJECT
public:
    enum DataType { Char, Short, Int, Long, Float, Double };
    Q_ENUM(DataType)

    VariableData();

    DataType dataType() const;
    void setDataType(DataType newDataType);

    QString name() const;
    void setName(const QString &newName);

private:
    DataType _dataType;
    QString _name;
};

} // namespace Data
