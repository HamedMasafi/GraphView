#pragma once

#include <QObject>

namespace Data {
class AbstractData : public QObject
{
    Q_OBJECT

public:
    enum class Type { Unknown, Function, Variable, Firmware };

    Type type() const;
    void sync();

protected:
    explicit AbstractData(Type type);

Q_SIGNALS:
    void reloaded();

private:
    Type _type;
};

} // namespace Data
