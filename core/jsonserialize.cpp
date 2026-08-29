#include "jsonserialize.h"

#include <QMetaProperty>

namespace GraphView::Core {

QString toBase64(const QVariant &v)
{
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds << v;
    return QString::fromLatin1(ba.toBase64());
}

QVariant fromBase64(const QString &s)
{
    QByteArray ba = QByteArray::fromBase64(s.toLatin1());
    QDataStream ds(&ba, QIODevice::ReadOnly);
    QVariant v;
    ds >> v;
    return v;
}

// انواع عددی ساده - خروجی مستقیماً QJsonValue از نوع مناسب

template<>
QJsonValue toJson<bool>(const bool &v)
{
    return QJsonValue{v};
}

template<>
bool fromJson<bool>(const QJsonValue &v)
{
    return v.toBool();
}

template<>
QJsonValue toJson<int>(const int &v)
{
    return QJsonValue{v};
}
template<>
int fromJson<int>(const QJsonValue &v)
{
    return v.toInt();
}

template<>
QJsonValue toJson<double>(const double &v)
{
    return QJsonValue{v};
}
template<>
double fromJson<double>(const QJsonValue &v)
{
    return v.toDouble();
}

template<>
QJsonValue toJson<uint>(const uint &v)
{
    return QJsonValue(static_cast<qint64>(v));
}
template<>
uint fromJson<uint>(const QJsonValue &v)
{
    return v.toVariant().toUInt();
}

template<>
QJsonValue toJson<qint64>(const qint64 &v)
{
    return QJsonValue(QString::number(v));
}
template<>
qint64 fromJson<qint64>(const QJsonValue &v)
{
    return v.toString().toLongLong();
}

template<>
QJsonValue toJson<quint64>(const quint64 &v)
{
    return QJsonValue(QString::number(v));
}
template<>
quint64 fromJson<quint64>(const QJsonValue &v)
{
    return v.toString().toULongLong();
}

template<>
QJsonValue toJson<long>(const long &v)
{
    return QJsonValue(QString::number(v));
}
template<>
long fromJson<long>(const QJsonValue &v)
{
    return v.toString().toLong();
}

template<>
QJsonValue toJson<short>(const short &v)
{
    return QJsonValue(v);
}
template<>
short fromJson<short>(const QJsonValue &v)
{
    return static_cast<short>(v.toInt());
}

template<>
QJsonValue toJson<char>(const char &v)
{
    return QJsonValue(v);
}
template<>
char fromJson<char>(const QJsonValue &v)
{
    return static_cast<char>(v.toInt());
}

template<>
QJsonValue toJson<ulong>(const ulong &v)
{
    return QJsonValue(QString::number(v));
}
template<>
ulong fromJson<ulong>(const QJsonValue &v)
{
    return v.toString().toULong();
}

template<>
QJsonValue toJson<ushort>(const ushort &v)
{
    return QJsonValue(v);
}
template<>
ushort fromJson<ushort>(const QJsonValue &v)
{
    return static_cast<ushort>(v.toInt());
}

template<>
QJsonValue toJson<uchar>(const uchar &v)
{
    return QJsonValue(v);
}
template<>
uchar fromJson<uchar>(const QJsonValue &v)
{
    return static_cast<uchar>(v.toInt());
}

template<>
QJsonValue toJson<float>(const float &v)
{
    return QJsonValue(static_cast<double>(v));
}
template<>
float fromJson<float>(const QJsonValue &v)
{
    return static_cast<float>(v.toDouble());
}

template<>
QJsonValue toJson<signed char>(const signed char &v)
{
    return QJsonValue(v);
}
template<>
signed char fromJson<signed char>(const QJsonValue &v)
{
    return static_cast<signed char>(v.toInt());
}

// QString - رشته ساده
template<>
QJsonValue toJson<QString>(const QString &v)
{
    return QJsonValue(v);
}
template<>
QString fromJson<QString>(const QJsonValue &v)
{
    return v.toString();
}

// QChar
template<>
QJsonValue toJson<QChar>(const QChar &v)
{
    return QJsonValue(QString(v));
}
template<>
QChar fromJson<QChar>(const QJsonValue &v)
{
    return v.toString().at(0);
}

// QStringList - آرایه JSON
template<>
QJsonValue toJson<QStringList>(const QStringList &v)
{
    return QJsonArray::fromStringList(v);
}
template<>
QStringList fromJson<QStringList>(const QJsonValue &v)
{
    QStringList l;
    for (auto x : v.toArray())
        l << x.toString();
    return l;
}

// QByteArray
template<>
QJsonValue toJson<QByteArray>(const QByteArray &v)
{
    return QJsonValue(QString::fromLatin1(v.toBase64()));
}
template<>
QByteArray fromJson<QByteArray>(const QJsonValue &v)
{
    return QByteArray::fromBase64(v.toString().toLatin1());
}

// QDate
template<>
QJsonValue toJson<QDate>(const QDate &v)
{
    return QJsonValue(v.toString(Qt::ISODate));
}
template<>
QDate fromJson<QDate>(const QJsonValue &v)
{
    return QDate::fromString(v.toString(), Qt::ISODate);
}

// QTime
template<>
QJsonValue toJson<QTime>(const QTime &v)
{
    return QJsonValue(v.toString(Qt::ISODateWithMs));
}
template<>
QTime fromJson<QTime>(const QJsonValue &v)
{
    return QTime::fromString(v.toString(), Qt::ISODateWithMs);
}

// QDateTime
template<>
QJsonValue toJson<QDateTime>(const QDateTime &v)
{
    return QJsonValue(v.toString(Qt::ISODateWithMs));
}
template<>
QDateTime fromJson<QDateTime>(const QJsonValue &v)
{
    return QDateTime::fromString(v.toString(), Qt::ISODateWithMs);
}

// QUrl
template<>
QJsonValue toJson<QUrl>(const QUrl &v)
{
    return QJsonValue(v.toString());
}
template<>
QUrl fromJson<QUrl>(const QJsonValue &v)
{
    return QUrl(v.toString());
}

// QPoint
template<>
QJsonValue toJson<QPoint>(const QPoint &v)
{
    QJsonObject valueObject;
    valueObject["x"] = v.x();
    valueObject["y"] = v.y();
    return valueObject;
}
template<>
QPoint fromJson<QPoint>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QPoint{valueObject["x"].toInt(), valueObject["y"].toInt()};
}

// QPointF
template<>
QJsonValue toJson<QPointF>(const QPointF &v)
{
    QJsonObject valueObject;
    valueObject["x"] = v.x();
    valueObject["y"] = v.y();
    return valueObject;
}
template<>
QPointF fromJson<QPointF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QPointF{valueObject["x"].toDouble(), valueObject["y"].toDouble()};
}

// QSize
template<>
QJsonValue toJson<QSize>(const QSize &v)
{
    QJsonObject valueObject;
    valueObject["w"] = v.width();
    valueObject["h"] = v.height();
    return valueObject;
}
template<>
QSize fromJson<QSize>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QSize{valueObject["w"].toInt(), valueObject["h"].toInt()};
}

// QSizeF
template<>
QJsonValue toJson<QSizeF>(const QSizeF &v)
{
    QJsonObject valueObject;
    valueObject["w"] = v.width();
    valueObject["h"] = v.height();
    return valueObject;
}
template<>
QSizeF fromJson<QSizeF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QSizeF{valueObject["w"].toDouble(), valueObject["h"].toDouble()};
}

// QRect
template<>
QJsonValue toJson<QRect>(const QRect &v)
{
    QJsonObject valueObject;
    valueObject["x"] = v.x();
    valueObject["y"] = v.y();
    valueObject["w"] = v.width();
    valueObject["h"] = v.height();
    return valueObject;
}
template<>
QRect fromJson<QRect>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QRect{valueObject["x"].toInt(),
                 valueObject["y"].toInt(),
                 valueObject["w"].toInt(),
                 valueObject["h"].toInt()};
}

// QRectF
template<>
QJsonValue toJson<QRectF>(const QRectF &v)
{
    QJsonObject valueObject;
    valueObject["x"] = v.x();
    valueObject["y"] = v.y();
    valueObject["w"] = v.width();
    valueObject["h"] = v.height();
    return valueObject;
}
template<>
QRectF fromJson<QRectF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QRectF{valueObject["x"].toDouble(),
                  valueObject["y"].toDouble(),
                  valueObject["w"].toDouble(),
                  valueObject["h"].toDouble()};
}

// QLine
template<>
QJsonValue toJson<QLine>(const QLine &v)
{
    QJsonObject valueObject;
    valueObject["x1"] = v.x1();
    valueObject["y1"] = v.y1();
    valueObject["x2"] = v.x2();
    valueObject["y2"] = v.y2();
    return valueObject;
}
template<>
QLine fromJson<QLine>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QLine{valueObject["x1"].toInt(),
                 valueObject["y1"].toInt(),
                 valueObject["x2"].toInt(),
                 valueObject["y2"].toInt()};
}

// QLineF
template<>
QJsonValue toJson<QLineF>(const QLineF &v)
{
    QJsonObject valueObject;
    valueObject["x1"] = v.x1();
    valueObject["y1"] = v.y1();
    valueObject["x2"] = v.x2();
    valueObject["y2"] = v.y2();
    return valueObject;
}
template<>
QLineF fromJson<QLineF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QLineF{valueObject["x1"].toDouble(),
                  valueObject["y1"].toDouble(),
                  valueObject["x2"].toDouble(),
                  valueObject["y2"].toDouble()};
}

// QUuid
template<>
QJsonValue toJson<QUuid>(const QUuid &v)
{
    return QJsonValue(v.toString(QUuid::WithoutBraces));
}
template<>
QUuid fromJson<QUuid>(const QJsonValue &v)
{
    return QUuid{v.toString()};
}

// QColor
template<>
QJsonValue toJson<QColor>(const QColor &v)
{
    QJsonObject valueObject;
    valueObject["red"] = v.red();
    valueObject["green"] = v.green();
    valueObject["blue"] = v.blue();
    valueObject["alpha"] = v.alpha();
    return valueObject;
}
template<>
QColor fromJson<QColor>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QColor(valueObject["red"].toInt(),
                  valueObject["green"].toInt(),
                  valueObject["blue"].toInt(),
                  valueObject["alpha"].toInt(255));
}

// QPolygon
template<>
QJsonValue toJson<QPolygon>(const QPolygon &v)
{
    QJsonArray pointsArray;
    for (const QPoint &p : v) {
        QJsonObject pointObject;
        pointObject["x"] = p.x();
        pointObject["y"] = p.y();
        pointsArray.append(pointObject);
    }
    QJsonObject valueObject;
    valueObject["points"] = pointsArray;
    return valueObject;
}
template<>
QPolygon fromJson<QPolygon>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto pointsArray = valueObject["points"].toArray();
    QPolygon poly;
    for (const auto &p : std::as_const(pointsArray)) {
        auto pointObject = p.toObject();
        poly << QPoint{pointObject["x"].toInt(), pointObject["y"].toInt()};
    }
    return poly;
}

// QPolygonF
template<>
QJsonValue toJson<QPolygonF>(const QPolygonF &v)
{
    QJsonArray pointsArray;
    for (const QPointF &p : v) {
        QJsonObject pointObject;
        pointObject["x"] = p.x();
        pointObject["y"] = p.y();
        pointsArray.append(pointObject);
    }
    QJsonObject valueObject;
    valueObject["points"] = pointsArray;
    return valueObject;
}
template<>
QPolygonF fromJson<QPolygonF>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto pointsArray = valueObject["points"].toArray();
    QPolygonF poly;
    for (const auto &p : std::as_const(pointsArray)) {
        auto pointObject = p.toObject();
        poly << QPointF{pointObject["x"].toDouble(), pointObject["y"].toDouble()};
    }
    return poly;
}

// QBitArray
template<>
QJsonValue toJson<QBitArray>(const QBitArray &v)
{
    QString bits;
    bits.reserve(v.size());
    for (int i = 0; i < v.size(); ++i)
        bits.append(v.testBit(i) ? '1' : '0');
    QJsonObject valueObject;
    valueObject["bits"] = bits;
    return valueObject;
}
template<>
QBitArray fromJson<QBitArray>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto bits = valueObject["bits"].toString();
    QBitArray bitArray(bits.size());
    for (int i = 0; i < bits.size(); ++i)
        bitArray.setBit(i, bits[i] == '1');
    return bitArray;
}

// QImage
template<>
QJsonValue toJson<QImage>(const QImage &v)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    v.save(&buffer, "PNG");
    return QJsonValue(QString::fromLatin1(bytes.toBase64()));
}
template<>
QImage fromJson<QImage>(const QJsonValue &v)
{
    auto bytes = QByteArray::fromBase64(v.toString().toLatin1());
    QImage image;
    image.loadFromData(bytes, "PNG");
    return image;
}

// QPixmap
template<>
QJsonValue toJson<QPixmap>(const QPixmap &v)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    v.save(&buffer, "PNG");
    return QJsonValue(QString::fromLatin1(bytes.toBase64()));
}
template<>
QPixmap fromJson<QPixmap>(const QJsonValue &v)
{
    auto bytes = QByteArray::fromBase64(v.toString().toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(bytes, "PNG");
    return pixmap;
}

// QIcon
template<>
QJsonValue toJson<QIcon>(const QIcon &v)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    auto pixmap = v.pixmap(256, 256);
    pixmap.save(&buffer, "PNG");
    return QJsonValue(QString::fromLatin1(bytes.toBase64()));
}
template<>
QIcon fromJson<QIcon>(const QJsonValue &v)
{
    auto bytes = QByteArray::fromBase64(v.toString().toLatin1());
    QPixmap pixmap;
    pixmap.loadFromData(bytes, "PNG");
    return QIcon{pixmap};
}

// QRegion
template<>
QJsonValue toJson<QRegion>(const QRegion &v)
{
    QJsonArray rectsArray;
    for (const QRect &r : v) {
        QJsonObject rectObject;
        rectObject["x"] = r.x();
        rectObject["y"] = r.y();
        rectObject["w"] = r.width();
        rectObject["h"] = r.height();
        rectsArray.append(rectObject);
    }
    QJsonObject valueObject;
    valueObject["rects"] = rectsArray;
    return valueObject;
}
template<>
QRegion fromJson<QRegion>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto rectsArray = valueObject["rects"].toArray();
    QRegion region;
    for (const auto &r : std::as_const(rectsArray)) {
        auto rectObject = r.toObject();
        region += QRect{rectObject["x"].toInt(),
                        rectObject["y"].toInt(),
                        rectObject["w"].toInt(),
                        rectObject["h"].toInt()};
    }
    return region;
}

// QFont
template<>
QJsonValue toJson<QFont>(const QFont &v)
{
    QJsonObject valueObject;
    valueObject["family"] = v.family();
    valueObject["pointSizeF"] = v.pointSizeF();
    valueObject["weight"] = v.weight();
    valueObject["italic"] = v.italic();
    valueObject["underline"] = v.underline();
    valueObject["strikeOut"] = v.strikeOut();
    valueObject["kerning"] = v.kerning();
    valueObject["letterSpacing"] = v.letterSpacing();
    valueObject["wordSpacing"] = v.wordSpacing();
    valueObject["capitalization"] = static_cast<int>(v.capitalization());
    return valueObject;
}
template<>
QFont fromJson<QFont>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    QFont font;
    font.setFamily(valueObject["family"].toString());
    font.setPointSizeF(valueObject["pointSizeF"].toDouble());
    font.setWeight(static_cast<QFont::Weight>(valueObject["weight"].toInt()));
    font.setItalic(valueObject["italic"].toBool());
    font.setUnderline(valueObject["underline"].toBool());
    font.setStrikeOut(valueObject["strikeOut"].toBool());
    font.setKerning(valueObject["kerning"].toBool());
    font.setLetterSpacing(QFont::PercentageSpacing, valueObject["letterSpacing"].toDouble());
    font.setWordSpacing(valueObject["wordSpacing"].toDouble());
    font.setCapitalization(
        static_cast<QFont::Capitalization>(valueObject["capitalization"].toInt()));
    return font;
}

// QRegularExpression
template<>
QJsonValue toJson<QRegularExpression>(const QRegularExpression &v)
{
    QJsonObject valueObject;
    valueObject["pattern"] = v.pattern();
    valueObject["options"] = static_cast<int>(v.patternOptions());
    return valueObject;
}
template<>
QRegularExpression fromJson<QRegularExpression>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    return QRegularExpression{valueObject["pattern"].toString(),
                              static_cast<QRegularExpression::PatternOptions>(
                                  valueObject["options"].toInt())};
}

// QByteArrayList
template<>
QJsonValue toJson<QByteArrayList>(const QByteArrayList &v)
{
    QJsonArray array;
    for (const auto &ba : v)
        array.append(QString::fromLatin1(ba.toBase64()));
    QJsonObject valueObject;
    valueObject["list"] = array;
    return valueObject;
}
template<>
QByteArrayList fromJson<QByteArrayList>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto array = valueObject["list"].toArray();
    QByteArrayList list;
    for (const auto &item : std::as_const(array))
        list.append(QByteArray::fromBase64(item.toString().toLatin1()));
    return list;
}

// QBrush
template<>
QJsonValue toJson<QBrush>(const QBrush &v)
{
    QJsonObject colorObject;
    colorObject["r"] = v.color().red();
    colorObject["g"] = v.color().green();
    colorObject["b"] = v.color().blue();
    colorObject["a"] = v.color().alpha();
    QJsonObject valueObject;
    valueObject["style"] = static_cast<int>(v.style());
    valueObject["color"] = colorObject;
    return valueObject;
}
template<>
QBrush fromJson<QBrush>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto colorObject = valueObject["color"].toObject();
    QColor color{colorObject["r"].toInt(),
                 colorObject["g"].toInt(),
                 colorObject["b"].toInt(),
                 colorObject["a"].toInt()};
    return QBrush{color, static_cast<Qt::BrushStyle>(valueObject["style"].toInt())};
}

// QPalette
template<>
QJsonValue toJson<QPalette>(const QPalette &v)
{
    auto colorToJson = [](const QColor &c) {
        QJsonObject o;
        o["r"] = c.red();
        o["g"] = c.green();
        o["b"] = c.blue();
        o["a"] = c.alpha();
        return o;
    };
    QJsonObject valueObject;
    valueObject["window"] = colorToJson(v.color(QPalette::Window));
    valueObject["windowText"] = colorToJson(v.color(QPalette::WindowText));
    valueObject["base"] = colorToJson(v.color(QPalette::Base));
    valueObject["text"] = colorToJson(v.color(QPalette::Text));
    valueObject["button"] = colorToJson(v.color(QPalette::Button));
    valueObject["buttonText"] = colorToJson(v.color(QPalette::ButtonText));
    valueObject["highlight"] = colorToJson(v.color(QPalette::Highlight));
    valueObject["highlightedText"] = colorToJson(v.color(QPalette::HighlightedText));
    return valueObject;
}
template<>
QPalette fromJson<QPalette>(const QJsonValue &v)
{
    auto valueObject = v.toObject();
    auto toColor = [](const QJsonObject &o) {
        return QColor{o["r"].toInt(), o["g"].toInt(), o["b"].toInt(), o["a"].toInt()};
    };
    QPalette p;
    p.setColor(QPalette::Window, toColor(valueObject["window"].toObject()));
    p.setColor(QPalette::WindowText, toColor(valueObject["windowText"].toObject()));
    p.setColor(QPalette::Base, toColor(valueObject["base"].toObject()));
    p.setColor(QPalette::Text, toColor(valueObject["text"].toObject()));
    p.setColor(QPalette::Button, toColor(valueObject["button"].toObject()));
    p.setColor(QPalette::ButtonText, toColor(valueObject["buttonText"].toObject()));
    p.setColor(QPalette::Highlight, toColor(valueObject["highlight"].toObject()));
    p.setColor(QPalette::HighlightedText, toColor(valueObject["highlightedText"].toObject()));
    return p;
}

// QPen
template<>
QJsonValue toJson<QPen>(const QPen &v)
{
    QJsonObject colorObject;
    colorObject["r"] = v.color().red();
    colorObject["g"] = v.color().green();
    colorObject["b"] = v.color().blue();
    colorObject["a"] = v.color().alpha();

    QJsonObject valueObject;

    valueObject["widthF"] = v.widthF();
    valueObject["style"] = static_cast<int>(v.style());
    valueObject["capStyle"] = static_cast<int>(v.capStyle());
    valueObject["joinStyle"] = static_cast<int>(v.joinStyle());
    valueObject["color"] = colorObject;

    return valueObject;
}

template<>
QPen fromJson<QPen>(const QJsonValue &j)
{
    auto obj = j.toObject();
    auto colorObject = obj["color"].toObject();

    QColor color{colorObject["r"].toInt(),
                 colorObject["g"].toInt(),
                 colorObject["b"].toInt(),
                 colorObject["a"].toInt()};

    QPen pen;

    pen.setWidthF(obj["widthF"].toDouble());
    pen.setStyle(static_cast<Qt::PenStyle>(obj["style"].toInt()));
    pen.setCapStyle(static_cast<Qt::PenCapStyle>(obj["capStyle"].toInt()));
    pen.setJoinStyle(static_cast<Qt::PenJoinStyle>(obj["joinStyle"].toInt()));
    pen.setColor(color);

    return pen;
}
// QBitmap
template<>
QJsonValue toJson<QBitmap>(const QBitmap &v)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);

    v.save(&buffer, "PNG");

    return QJsonValue{QString::fromLatin1(bytes.toBase64())};
}

template<>
QBitmap fromJson<QBitmap>(const QJsonValue &j)
{
    auto bytes = QByteArray::fromBase64(j.toString().toLatin1());

    QBitmap bitmap;
    bitmap.loadFromData(bytes, "PNG");

    return bitmap;
}

// QKeySequence
template<>
QJsonValue toJson<QKeySequence>(const QKeySequence &v)
{
    return QJsonValue{v.toString(QKeySequence::PortableText)};
}

template<>
QKeySequence fromJson<QKeySequence>(const QJsonValue &j)
{
    return QKeySequence{j.toString(), QKeySequence::PortableText};
}

// QTextFormat
template<>
QJsonValue toJson<QTextFormat>(const QTextFormat &v)
{
    QJsonObject propertiesObject;

    for (auto it = v.properties().cbegin(); it != v.properties().cend(); ++it) {
        propertiesObject[QString::number(it.key())] = QJsonValue::fromVariant(it.value());
    }

    QJsonObject valueObject;

    valueObject["objectIndex"] = v.objectIndex();
    valueObject["type"] = v.type();
    valueObject["properties"] = propertiesObject;

    return valueObject;
}

template<>
QTextFormat fromJson<QTextFormat>(const QJsonValue &j)
{
    auto valueObject = j.toObject();

    QTextFormat format;

    format.setObjectIndex(valueObject["objectIndex"].toInt());

    auto propertiesObject = valueObject["properties"].toObject();

    for (auto it = propertiesObject.begin(); it != propertiesObject.end(); ++it) {
        format.setProperty(it.key().toInt(), it.value().toVariant());
    }

    return format;
}

// QTransform
template<>
QJsonValue toJson<QTransform>(const QTransform &v)
{
    QJsonArray array;

    array.append(v.m11());
    array.append(v.m12());
    array.append(v.m13());

    array.append(v.m21());
    array.append(v.m22());
    array.append(v.m23());

    array.append(v.m31());
    array.append(v.m32());
    array.append(v.m33());

    return array;
}

template<>
QTransform fromJson<QTransform>(const QJsonValue &j)
{
    auto array = j.toArray();

    return QTransform{array[0].toDouble(),
                      array[1].toDouble(),
                      array[2].toDouble(),

                      array[3].toDouble(),
                      array[4].toDouble(),
                      array[5].toDouble(),

                      array[6].toDouble(),
                      array[7].toDouble(),
                      array[8].toDouble()};
}

// QColorSpace
template<>
inline QJsonValue toJson<QColorSpace>(const QColorSpace &v)
{
    return QJsonValue{QString::fromLatin1(v.iccProfile().toBase64())};
}

template<>
inline QColorSpace fromJson<QColorSpace>(const QJsonValue &j)
{
    return QColorSpace::fromIccProfile(QByteArray::fromBase64(j.toString().toLatin1()));
}

// QEasingCurve
template<>
QJsonValue toJson<QEasingCurve>(const QEasingCurve &v)
{
    return QJsonValue{static_cast<int>(v.type())};
}
template<>
QEasingCurve fromJson<QEasingCurve>(const QJsonValue &v)
{
    return QEasingCurve{static_cast<QEasingCurve::Type>(v.toInt())};
}

// QStringMap
template<>
QJsonValue toJson<QStringMap>(const QStringMap &v)
{
    QJsonObject o;
    for (auto i = v.cbegin(); i != v.cend(); ++i) {
        o[i.key()] = i.value();
    }
    return o;
}

template<>
QStringMap fromJson<QStringMap>(const QJsonValue &j)
{
    auto o = j.toObject();
    QStringMap ret;
    for (auto i = o.constBegin(); i != o.constEnd(); ++i) {
        ret[i.key()] = i.value().toString();
    }
    return ret;
}

QJsonObject variantToJson(const QVariant &val)
{
    if (!val.isValid())
        return QJsonObject();

    int typeId = val.userType();
    QJsonValue v;
    // clang-format off
    switch (typeId) {
    case QMetaType::Bool: v = toJson(val.toBool()); break;
    case QMetaType::Int: v = toJson(val.toInt()); break;
    case QMetaType::UInt: v = toJson(val.toUInt()); break;
    case QMetaType::LongLong: v = toJson(val.toLongLong()); break;
    case QMetaType::ULongLong: v = toJson(val.toULongLong()); break;
    case QMetaType::Double: v = toJson(val.toDouble()); break;
    case QMetaType::Long: v = toJson(val.value<long>()); break;
    case QMetaType::Short: v = toJson(val.value<short>()); break;
    case QMetaType::Char: v = toJson(val.value<char>()); break;
    case QMetaType::ULong: v = toJson(val.value<ulong>()); break;
    case QMetaType::UShort: v = toJson(val.value<ushort>()); break;
    case QMetaType::UChar: v = toJson(val.value<uchar>()); break;
    case QMetaType::Float: v = toJson(val.value<float>()); break;
    case QMetaType::SChar: v = toJson(val.value<signed char>()); break;
    case QMetaType::QString: v = toJson(val.toString()); break;
    case QMetaType::QStringList: v = toJson(val.toStringList()); break;
    case QMetaType::QByteArray: v = toJson(val.toByteArray()); break;
    case QMetaType::QDate: v = toJson(val.toDate()); break;
    case QMetaType::QTime: v = toJson(val.toTime()); break;
    case QMetaType::QDateTime: v = toJson(val.toDateTime()); break;
    case QMetaType::QUrl: v = toJson(val.toUrl()); break;
    case QMetaType::QRect: v = toJson(val.toRect()); break;
    case QMetaType::QRectF: v = toJson(val.toRectF()); break;
    case QMetaType::QSize: v = toJson(val.toSize()); break;
    case QMetaType::QSizeF: v = toJson(val.toSizeF()); break;
    case QMetaType::QLine: v = toJson(val.toLine()); break;
    case QMetaType::QLineF: v = toJson(val.toLineF()); break;
    case QMetaType::QPoint: v = toJson(val.toPoint()); break;
    case QMetaType::QPointF: v = toJson(val.toPointF()); break;
    case QMetaType::QUuid: v = toJson(val.value<QUuid>()); break;
    case QMetaType::QColor: v = toJson(val.value<QColor>()); break;
    case QMetaType::QFont: v = toJson(val.value<QFont>()); break;
    case QMetaType::QPixmap: v = toJson(val.value<QPixmap>()); break;
    case QMetaType::QImage: v = toJson(val.value<QImage>()); break;
    case QMetaType::QIcon: v = toJson(val.value<QIcon>()); break;
    case QMetaType::QBrush: v = toJson(val.value<QBrush>()); break;
    case QMetaType::QPen: v = toJson(val.value<QPen>()); break;
    case QMetaType::QPalette: v = toJson(val.value<QPalette>()); break;
    case QMetaType::QBitArray: v = toJson(val.value<QBitArray>()); break;
    case QMetaType::QPolygon: v = toJson(val.value<QPolygon>()); break;
    case QMetaType::QPolygonF: v = toJson(val.value<QPolygonF>()); break;
    case QMetaType::QRegion: v = toJson(val.value<QRegion>()); break;
    case QMetaType::QBitmap: v = toJson(val.value<QBitmap>()); break;
    case QMetaType::QKeySequence: v = toJson(val.value<QKeySequence>()); break;
    case QMetaType::QTextFormat: v = toJson(val.value<QTextFormat>()); break;
    case QMetaType::QTransform: v = toJson(val.value<QTransform>()); break;
    case QMetaType::QRegularExpression: v = toJson(val.value<QRegularExpression>()); break;
    case QMetaType::QEasingCurve: v = toJson(val.value<QEasingCurve>()); break;
    case QMetaType::QByteArrayList: v = toJson(val.value<QByteArrayList>()); break;
    case QMetaType::QColorSpace: v = toJson(val.value<QColorSpace>()); break;
    default:{
        auto mt = QMetaType(typeId);
        if (mt.flags() & QMetaType::IsEnumeration) {
            v = toJson(val.toInt());
        } else {
            // Fallback for types not explicitly handled
            qWarning()<<Q_FUNC_INFO << "Unable to save" << typeId << QMetaType(typeId).name();
            return {{TYPE_KEY, "Unknown"}, {TYPE_VALUE, toBase64(val)}};
        }
    }
    }
    // clang-format on

    return {{TYPE_KEY, QString{QMetaType(typeId).name()}}, {TYPE_VALUE, v}};
}

QVariant jsonToVariant(const QJsonObject &obj)
{
    QString typeName = obj.value(TYPE_KEY).toString();
    QJsonValue v = obj.value(TYPE_VALUE);

    // clang-format off
    if (typeName == "Bool") return QVariant::fromValue(fromJson<bool>(v));
    if (typeName == "Int") return QVariant::fromValue(fromJson<int>(v));
    if (typeName == "UInt") return QVariant::fromValue(fromJson<uint>(v));
    if (typeName == "LongLong") return QVariant::fromValue(fromJson<qint64>(v));
    if (typeName == "ULongLong") return QVariant::fromValue(fromJson<quint64>(v));
    if (typeName == "Double") return QVariant::fromValue(fromJson<double>(v));
    if (typeName == "Long") return QVariant::fromValue(fromJson<long>(v));
    if (typeName == "Short") return QVariant::fromValue(fromJson<short>(v));
    if (typeName == "Char") return QVariant::fromValue(fromJson<char>(v));
    if (typeName == "ULong") return QVariant::fromValue(fromJson<ulong>(v));
    if (typeName == "UShort") return QVariant::fromValue(fromJson<ushort>(v));
    if (typeName == "UChar") return QVariant::fromValue(fromJson<uchar>(v));
    if (typeName == "Float") return QVariant::fromValue(fromJson<float>(v));
    if (typeName == "SChar") return QVariant::fromValue(fromJson<signed char>(v));
    if (typeName == "QString") return QVariant::fromValue(fromJson<QString>(v));
    if (typeName == "QStringList") return QVariant::fromValue(fromJson<QStringList>(v));
    if (typeName == "QByteArray") return QVariant::fromValue(fromJson<QByteArray>(v));
    if (typeName == "QDate") return QVariant::fromValue(fromJson<QDate>(v));
    if (typeName == "QTime") return QVariant::fromValue(fromJson<QTime>(v));
    if (typeName == "QDateTime") return QVariant::fromValue(fromJson<QDateTime>(v));
    if (typeName == "QUrl") return QVariant::fromValue(fromJson<QUrl>(v));
    if (typeName == "QRect") return QVariant::fromValue(fromJson<QRect>(v));
    if (typeName == "QRectF") return QVariant::fromValue(fromJson<QRectF>(v));
    if (typeName == "QSize") return QVariant::fromValue(fromJson<QSize>(v));
    if (typeName == "QSizeF") return QVariant::fromValue(fromJson<QSizeF>(v));
    if (typeName == "QLine") return QVariant::fromValue(fromJson<QLine>(v));
    if (typeName == "QLineF") return QVariant::fromValue(fromJson<QLineF>(v));
    if (typeName == "QPoint") return QVariant::fromValue(fromJson<QPoint>(v));
    if (typeName == "QPointF") return QVariant::fromValue(fromJson<QPointF>(v));
    if (typeName == "QUuid") return QVariant::fromValue(fromJson<QUuid>(v));
    if (typeName == "QColor") return QVariant::fromValue(fromJson<QColor>(v));
    if (typeName == "QFont") return QVariant::fromValue(fromJson<QFont>(v));
    if (typeName == "QPixmap") return QVariant::fromValue(fromJson<QPixmap>(v));
    if (typeName == "QImage") return QVariant::fromValue(fromJson<QImage>(v));
    if (typeName == "QIcon") return QVariant::fromValue(fromJson<QIcon>(v));
    if (typeName == "QBrush") return QVariant::fromValue(fromJson<QBrush>(v));
    if (typeName == "QPen") return QVariant::fromValue(fromJson<QPen>(v));
    if (typeName == "QPalette") return QVariant::fromValue(fromJson<QPalette>(v));
    if (typeName == "QBitArray") return QVariant::fromValue(fromJson<QBitArray>(v));
    if (typeName == "QPolygon") return QVariant::fromValue(fromJson<QPolygon>(v));
    if (typeName == "QPolygonF") return QVariant::fromValue(fromJson<QPolygonF>(v));
    if (typeName == "QRegion") return QVariant::fromValue(fromJson<QRegion>(v));
    if (typeName == "QBitmap") return QVariant::fromValue(fromJson<QBitmap>(v));
    if (typeName == "QKeySequence") return QVariant::fromValue(fromJson<QKeySequence>(v));
    if (typeName == "QTextFormat") return QVariant::fromValue(fromJson<QTextFormat>(v));
    if (typeName == "QTransform") return QVariant::fromValue(fromJson<QTransform>(v));
    if (typeName == "QRegularExpression") return QVariant::fromValue(fromJson<QRegularExpression>(v));
    if (typeName == "QEasingCurve") return QVariant::fromValue(fromJson<QEasingCurve>(v));
    if (typeName == "QByteArrayList") return QVariant::fromValue(fromJson<QByteArrayList>(v));
    if (typeName == "QColorSpace") return QVariant::fromValue(fromJson<QColorSpace>(v));

    if (typeName == "bool") return QVariant::fromValue(fromJson<bool>(v));
    if (typeName == "int") return QVariant::fromValue(fromJson<int>(v));
    if (typeName == "double") return QVariant::fromValue(fromJson<double>(v));
    if (typeName == "uchar") return QVariant::fromValue(fromJson<uchar>(v));

    // clang-format on
    qDebug() << "Unable to deserialize" << typeName << obj;
    return {};
}

QJsonObject objectToJson(QObject *obj)
{
    QJsonObject jsonObject;
    auto me = obj->metaObject();

    for (int i = 0; i < me->propertyCount(); ++i) {
        auto p = me->property(i);
        if (p.isEnumType())
            jsonObject[QString{p.name()}] = variantToJson(p.read(obj).toInt());
        else
            jsonObject[QString{p.name()}] = variantToJson(p.read(obj));
    }

    return jsonObject;
}

bool jsonToObject(const QJsonObject &json, QObject *obj)
{
    auto me = obj->metaObject();

    for (int i = 0; i < me->propertyCount(); ++i) {
        auto p = me->property(i);
        p.write(obj, jsonToVariant(json[p.name()].toObject()));
    }
    return true;
}

QJsonObject gadgetToJson(void *obj, const QMetaObject &metaObject)
{
    QJsonObject jsonObject;

    for (int i = 0; i < metaObject.propertyCount(); ++i) {
        auto p = metaObject.property(i);
        if (p.isEnumType())
            jsonObject[QString{p.name()}] = variantToJson(p.readOnGadget(obj).toInt());
        else
            jsonObject[QString{p.name()}] = variantToJson(p.readOnGadget(obj));
    }

    return jsonObject;
}

bool jsonToGadget(const QJsonObject &json, void *obj, const QMetaObject &metaObject)
{
    for (int i = 0; i < metaObject.propertyCount(); ++i) {
        auto p = metaObject.property(i);
        if (json.contains(p.name()))
            p.writeOnGadget(obj, jsonToVariant(json[p.name()].toObject()));
    }
    return true;
}

} // namespace GraphView::Core
