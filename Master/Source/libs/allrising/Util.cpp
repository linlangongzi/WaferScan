#include "Util.h"
#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>
#include <QtMath>
#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>

#define MM_PER_INCH 25.4

double DistanceFromTime(double time, double speed)
{
    return time * speed * 0.5e-3;
}

double TimeFromDistance(double distance, double speed)
{
    return distance / speed * 2e3;
}

double InchToMillimetre(double inch)
{
    return inch * MM_PER_INCH;
}

double MillimetreToInch(double mm)
{
    return mm / MM_PER_INCH;
}

QString Vector3DToString(const QVector3D &v, int precision)
{
    return QStringLiteral("(%1, %2, %3)")
            .arg(v.x(), -1, 'g', precision)
            .arg(v.y(), -1, 'g', precision)
            .arg(v.z(), -1, 'g', precision);
}

QVector3D Vector3DFromString(const QString &v)
{
    static const QRegularExpression pattern(R"(\(([\d\.]+),\s*([\d\.]+),\s*([\d\.]+)\))");
    const auto match = pattern.match(v);
    if (match.hasMatch()) {
        return QVector3D(match.captured(1).toFloat(), match.captured(2).toFloat(), match.captured(3).toFloat());
    }
    return QVector3D();
}

QByteArray ReadFileAsBinary(const QString &path, QString *errorString)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return QByteArray();
    }
    const QByteArray data = file.readAll();
    if (data.isEmpty() && errorString) {
        *errorString = file.errorString();
    }
    return data;
}

bool WriteFileAsBinary(const QString &path, const QByteArray &data, QString *errorString)
{
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return false;
    }
    if (file.write(data) != data.size()) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return false;
    }
    if (!file.commit()) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return false;
    }
    return true;
}

QString ReadFileAsString(const QString &path, QString *errorString)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return QString();
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    return stream.readAll();
}

bool WriteFileAsString(const QString &path, const QString &data, QString *errorString)
{
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return false;
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << data;
    if (stream.status() != QTextStream::Ok) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return false;
    }
    if (!file.commit()) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return false;
    }
    return true;
}

QStringList ReadFileAsStringList(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QStringList();
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QStringList list;
    while (!file.atEnd()) {
        list << file.readLine();
    }
    return list;
}

QJsonValue ReadFileAsJson(const QString &path, QString *errorString)
{
    const QByteArray &data = ReadFileAsString(path, errorString).toUtf8();
    if (data.isEmpty()) {
        return QJsonValue();
    }
    return ByteArrayToJson(data, false, errorString);
}

bool WriteFileAsJson(const QString &path, const QJsonValue &value, bool indent, QString *errorString)
{
    const QByteArray data = JsonToByteArray(value, indent, errorString);
    if (data.isEmpty()) {
        return false;
    }
    return WriteFileAsString(path, QString::fromUtf8(data), errorString);       /*!< 根据 QJsonDocument 文档，toJson 返回的数据以 UTF8 编码 */
}

QList<QList<double> > ReadCsvAsDouble(const QString &path, double invalidValue)
{
    using Line = QList<double>;
    using Csv = QList<Line>;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return Csv();
    }
    Csv data;
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString l = stream.readLine();
        const auto parts = l.splitRef(',');
        Line line;
        line.reserve(parts.size());
        for (const auto &part: parts) {
            bool ok;
            double v = part.trimmed().toDouble(&ok);
            line << (ok ? v : invalidValue);
        }
        data << line;
    }
    return data;
}

QVariant readClipboard(int typeId)
{
    QClipboard *clipboard = QGuiApplication::clipboard();

    QString text = clipboard->text();
    QVariant v = JsonToVariant(ByteArrayToJson(text.toUtf8()), typeId);
    return v;
}

void writeClipboard(const QVariant &value)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(QString::fromUtf8(JsonToByteArray(VariantToJson(value))));
}
