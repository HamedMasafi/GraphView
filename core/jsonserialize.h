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

#include <type_traits>

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
DECLARE_CONVERTER(QBitmap)
DECLARE_CONVERTER(QKeySequence)
DECLARE_CONVERTER(QTextFormat)
DECLARE_CONVERTER(QTransform)
DECLARE_CONVERTER(QColorSpace)
DECLARE_CONVERTER(QEasingCurve)
using QStringMap = QMap<QString, QString>;
DECLARE_CONVERTER(QStringMap)

// clang-format on

template<typename T, typename V>
QJsonValue toJson(const QPair<T, V> &v)
{
    return QJsonObject{{"first", toJson(v.first)}, {"second", toJson(v.second)}};
}

template<typename T, typename V>
QPair<T, V> fromJson(const QJsonValue &v)
{
    auto o = v.toObject();
    return qMakePair(fromJson<T>(o["first"]), fromJson<V>(o["second"]));
}

// template<typename T>
// QJsonValue toJson(const QMap<QString, T> &v)
// {
    // QJsonObject a;
    // for (auto i = v.begin(); i != v.end(); ++i) {
    //     a[i.key()] = toJson<T>(i.value());
    // }
    // return a;
// }

// template<typename T>
// QMap<QString, T> fromJson2(const QJsonValue &v)
// {
//     QMap<QString, T> map;
//     auto o = v.toObject();

//     for (auto n = o.begin(); n != o.end(); ++n) {
//         map[n.key()] = fromJson<T>(n.value());
//     }
//     return map;
// }

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
    for (const auto &v : arr) {
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
    for (const auto &v : arr) {
        if (!v.isObject())
            continue;
        auto item = new T;
        jsonToGadget(v.toObject(), item, T::statieMetaObject);
        list << item;
    }
    return list;
}



template<typename T>
struct IsQList : std::false_type {};

template<typename T>
struct IsQList<QList<T>> : std::true_type
{
    using ValueType = T;
};


// ------------------------------------------------------------
// QPair<QString, T>
// ------------------------------------------------------------

template<typename T>
struct IsQStringPair : std::false_type {};

template<typename T>
struct IsQStringPair<QPair<QString, T>> : std::true_type
{
    using ValueType = T;
};


// ------------------------------------------------------------
// QMap<QString, T>
// ------------------------------------------------------------

template<typename T>
struct IsQStringMap : std::false_type {};

template<typename T>
struct IsQStringMap<QMap<QString, T>> : std::true_type
{
    using ValueType = T;
};


// ------------------------------------------------------------
// Has staticMetaObject
// ------------------------------------------------------------

template<typename T, typename = void>
struct HasStaticMetaObject : std::false_type {};

template<typename T>
struct HasStaticMetaObject<T, std::void_t<decltype(T::staticMetaObject)>>
    : std::true_type
{};

template<typename T>
struct IsSupportedType : std::false_type {};

// cland-format off
template<> struct IsSupportedType<bool>:std::true_type{};
template<> struct IsSupportedType<int>:std::true_type{};
template<> struct IsSupportedType<double >:std::true_type{};
template<> struct IsSupportedType<uint>:std::true_type{};
template<> struct IsSupportedType<qint64>:std::true_type{};
template<> struct IsSupportedType<quint64>:std::true_type{};
template<> struct IsSupportedType<long>:std::true_type{};
template<> struct IsSupportedType<short>:std::true_type{};
template<> struct IsSupportedType<char>:std::true_type{};
template<> struct IsSupportedType<ulong>:std::true_type{};
template<> struct IsSupportedType<ushort>:std::true_type{};
template<> struct IsSupportedType<uchar>:std::true_type{};
template<> struct IsSupportedType<float>:std::true_type{};
template<> struct IsSupportedType<signed char>:std::true_type{};
template<> struct IsSupportedType<QChar>:std::true_type{};
template<> struct IsSupportedType<QString>:std::true_type{};
template<> struct IsSupportedType<QStringList>:std::true_type{};
template<> struct IsSupportedType<QByteArray>:std::true_type{};
template<> struct IsSupportedType<QDate>:std::true_type{};
template<> struct IsSupportedType<QTime>:std::true_type{};
template<> struct IsSupportedType<QDateTime>:std::true_type{};
template<> struct IsSupportedType<QUrl>:std::true_type{};
template<> struct IsSupportedType<QPoint>:std::true_type{};
template<> struct IsSupportedType<QPointF>:std::true_type{};
template<> struct IsSupportedType<QSize>:std::true_type{};
template<> struct IsSupportedType<QSizeF>:std::true_type{};
template<> struct IsSupportedType<QRect>:std::true_type{};
template<> struct IsSupportedType<QRectF>:std::true_type{};
template<> struct IsSupportedType<QLine>:std::true_type{};
template<> struct IsSupportedType<QLineF>:std::true_type{};
template<> struct IsSupportedType<QUuid>:std::true_type{};
template<> struct IsSupportedType<QColor>:std::true_type{};
template<> struct IsSupportedType<QPolygon>:std::true_type{};
template<> struct IsSupportedType<QPolygonF>:std::true_type{};
template<> struct IsSupportedType<QBitArray>:std::true_type{};
template<> struct IsSupportedType<QImage>:std::true_type{};
template<> struct IsSupportedType<QPixmap>:std::true_type{};
template<> struct IsSupportedType<QIcon>:std::true_type{};
template<> struct IsSupportedType<QRegion>:std::true_type{};
template<> struct IsSupportedType<QFont>:std::true_type{};
template<> struct IsSupportedType<QRegularExpression>:std::true_type{};
template<> struct IsSupportedType<QByteArrayList>:std::true_type{};
template<> struct IsSupportedType<QBrush>:std::true_type{};
template<> struct IsSupportedType<QPalette>:std::true_type{};
template<> struct IsSupportedType<QPen>:std::true_type{};
template<> struct IsSupportedType<QBitmap>:std::true_type{};
template<> struct IsSupportedType<QKeySequence>:std::true_type{};
template<> struct IsSupportedType<QTextFormat>:std::true_type{};
template<> struct IsSupportedType<QTransform>:std::true_type{};
template<> struct IsSupportedType<QColorSpace>:std::true_type{};
template<> struct IsSupportedType<QEasingCurve>:std::true_type{};
// cland-format off

// ------------------------------------------------------------
// toJson
// ------------------------------------------------------------

template<typename T>
QJsonValue convertToJson(const T &v)
{
    using U = std::remove_cv_t<std::remove_reference_t<T>>;

    if constexpr (IsSupportedType<U>::value) {
        return toJson(v);
    } else if constexpr (std::is_base_of_v<QObject, U>) {
        return objectToJson(v);
    } else if constexpr (HasStaticMetaObject<U>::value) {
        const QMetaObject *metaObject = &U::staticMetaObject;
        return gadgetToJson(v, metaObject);
    } else if constexpr (IsQList<U>::value) {
        QJsonArray result;

        for (const auto &item : v)
            result.append(convertToJson(item));

        return result;

    } else if constexpr (IsQStringPair<U>::value) {
        QJsonObject result;
        result.insert(v.first, convertToJson(v.second));

        return result;

    } else if constexpr (IsQStringMap<U>::value) {
        QJsonObject result;

        for (auto it = v.cbegin(); it != v.cend(); ++it)
            result.insert(it.key(), convertToJson(it.value()));

        return result;

    } else {
        return QJsonValue::fromVariant(QVariant::fromValue(v));
    }
}

} // namespace GraphView::Core


