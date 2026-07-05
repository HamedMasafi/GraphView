#pragma once

#include <QBitArray>
#include <QBitmap>
#include <QBrush>
#include <QBuffer>
#include <QByteArray>
#include <QByteArrayList>
#include <QColor>
#include <QColorSpace>
#include <QDataStream>
#include <QDate>
#include <QDateTime>
#include <QEasingCurve>
#include <QFont>
#include <QIcon>
#include <QImage>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QKeySequence>
#include <QLine>
#include <QLineF>
#include <QLocale>
#include <QMetaType>
#include <QPalette>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QPolygon>
#include <QPolygonF>
#include <QRect>
#include <QRectF>
#include <QRegion>
#include <QRegularExpression>
#include <QSize>
#include <QSizeF>
#include <QString>
#include <QStringList>
#include <QTextFormat>
#include <QTime>
#include <QTransform>
#include <QUrl>
#include <QUuid>
#include <QVariant>

static const char *TYPE_KEY = "T";
static const char *TYPE_VALUE = "V";

namespace GraphView::Core {

// --- HELPERS ---
QString toBase64(const QVariant &v);
QVariant fromBase64(const QString &s);

// --- TEMPLATE DECLARATIONS ---
template<typename T>
QJsonValue toJson(const T &val);

template<typename T>
T fromJson(const QJsonValue &obj);

// --- SPECIALIZATIONS ---

// Void
template<>
inline QJsonValue toJson<void *>(void *const &v)
{
    Q_UNUSED(v)
    return QJsonValue{"Void"};
}
template<>
inline void *fromJson<void *>(const QJsonValue &)
{
    return nullptr;
}

// clang-format off
// تابع‌های toJson - خروجی QJsonValue
// template<typename T>
// QJsonValue toJson(const T& value);

// // تابع‌های fromJson - ورودی QJsonValue
// template<typename T>
// T fromJson(const QJsonValue& value);

#define DECLARE_CONVERTER(Type) \
template<> QJsonValue toJson<Type>(const Type &v); \
template<> Type fromJson<Type>(const QJsonValue &v);


DECLARE_CONVERTER(bool)
DECLARE_CONVERTER(int)
DECLARE_CONVERTER(double )
DECLARE_CONVERTER(uint)
DECLARE_CONVERTER(qint64)
DECLARE_CONVERTER(quint64)
DECLARE_CONVERTER(long)
DECLARE_CONVERTER(short)
DECLARE_CONVERTER(char)
DECLARE_CONVERTER(ulong)
DECLARE_CONVERTER(ushort)
DECLARE_CONVERTER(uchar)
DECLARE_CONVERTER(float)
DECLARE_CONVERTER(signed char)
DECLARE_CONVERTER(QChar)
DECLARE_CONVERTER(QString)
DECLARE_CONVERTER(QStringList)
DECLARE_CONVERTER(QByteArray)
DECLARE_CONVERTER(QDate)
DECLARE_CONVERTER(QTime)
DECLARE_CONVERTER(QDateTime)
DECLARE_CONVERTER(QUrl)
DECLARE_CONVERTER(QPoint)
DECLARE_CONVERTER(QPointF)
DECLARE_CONVERTER(QSize)
DECLARE_CONVERTER(QSizeF)
DECLARE_CONVERTER(QRect)
DECLARE_CONVERTER(QRectF)
DECLARE_CONVERTER(QLine)
DECLARE_CONVERTER(QLineF)
DECLARE_CONVERTER(QUuid)
DECLARE_CONVERTER(QColor)
DECLARE_CONVERTER(QPolygon)
DECLARE_CONVERTER(QPolygonF)
DECLARE_CONVERTER(QBitArray)
DECLARE_CONVERTER(QImage)
DECLARE_CONVERTER(QPixmap)
DECLARE_CONVERTER(QIcon)
DECLARE_CONVERTER(QRegion)
DECLARE_CONVERTER(QFont)
DECLARE_CONVERTER(QRegularExpression)
DECLARE_CONVERTER(QByteArrayList)
DECLARE_CONVERTER(QBrush)
DECLARE_CONVERTER(QPalette)
DECLARE_CONVERTER(QPen)
DECLARE_CONVERTER(QPen)
DECLARE_CONVERTER(QBitmap)
DECLARE_CONVERTER(QKeySequence)
DECLARE_CONVERTER(QTextFormat)
DECLARE_CONVERTER(QTransform)
DECLARE_CONVERTER(QColorSpace)
DECLARE_CONVERTER(QEasingCurve)
using QStringMap = QMap<QString, QString>;
DECLARE_CONVERTER(QStringMap)

// clang-format on

template<typename T>
QJsonObject pack(const T &value)
{
    QJsonObject result;
    result[TYPE_KEY] = QString{QMetaType(qMetaTypeId<T>()).name()};
    // QString(typeid(T).name()); // یا می‌توانید از یک نگاشت نام استفاده کنید

    QJsonValue jsonValue = toJson<T>(value);
    result[TYPE_VALUE] = jsonValue;

    return result;
}

template<typename T>
T unpack(const QJsonObject &packed)
{
    QJsonValue jsonValue = packed.value(TYPE_VALUE);

    return fromJson<T>(jsonValue);
}

QJsonObject variantToJson(const QVariant &val);
QVariant jsonToVariant(const QJsonObject &obj);

QJsonObject objectToJson(QObject *obj);
bool jsonToObject(const QJsonObject &json, QObject *obj);

QJsonObject gadgetToJson(void *obj, const QMetaObject &metaObject);
bool jsonToGadget(const QJsonObject &json, void *obj, const QMetaObject &metaObject);

template<class T>
QJsonArray objectsToJson(QList<T *> list)
{
    QJsonArray arr;
    for (auto &item : list)
        arr << objectToJson(item);
    return arr;
}

template<class T>
QList<T *> jsonToObjects(const QJsonArray &arr)
{
    QList<T *> list;
    for (auto v : arr) {
        if (!v.isObject())
            continue;
        auto item = new T;
        jsonToObject(v.toObject(), item);
        list << item;
    }
    return list;
}

template<class T>
QJsonArray gadgetsToJson(QList<T *> list)
{
    QJsonArray arr;
    for (auto &item : list)
        arr << gadgetToJson(item, T::statieMetaObject);
    return arr;
}

template<class T>
QList<T *> jsonToGadgets(const QJsonArray &arr)
{
    QList<T *> list;
    for (auto v : arr) {
        if (!v.isObject())
            continue;
        auto item = new T;
        jsonToGadget(v.toObject(), item, T::statieMetaObject);
        list << item;
    }
    return list;
}

// void registerBasicConverters();

} // namespace GraphView::Core
