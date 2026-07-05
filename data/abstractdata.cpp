#include "abstractdata.h"

namespace Data {
AbstractData::AbstractData(Type type)
    : _type{type}
{}

AbstractData::Type AbstractData::type() const
{
    return _type;
}

void AbstractData::sync()
{
    Q_EMIT reloaded();
}
} // namespace Data
