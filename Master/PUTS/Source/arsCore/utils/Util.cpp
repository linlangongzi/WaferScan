#include "Util.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QTextStream>
#include <QtMath>

#define MM_PER_INCH 25.4

double DBFromPercent(double percent, double reference)
{
    return 20 * qLn(percent / reference) / qLn(10);
}

double DBToPercent(double db, double reference)
{
    return qPow(10, db / 20) * reference;
}

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

QDebug &DebugWithQObjectHeader(QDebug &debug, const char *name)
{
    bool oldSpace = debug.autoInsertSpaces();
    debug.nospace() << name << ": ";
    debug.setAutoInsertSpaces(oldSpace);
    return debug;
}

QByteArray ReadFileAsBinary(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }
    return file.readAll();
}

bool WriteFileAsBinary(const QString &path, const QByteArray &data)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    return file.write(data) == data.size();
}

QString ReadFileAsString(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    return stream.readAll();
}

bool WriteFileAsString(const QString &path, const QString &data)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << data;
    return true;
}

QJsonValue ReadFileAsJson(const QString &path)
{
    const QByteArray &data = ReadFileAsString(path).toUtf8();
    if (data.isEmpty()) {
        return QJsonValue();
    }
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        qWarning() << "Error reading json" << path << "at" << err.offset << ":" << err.errorString();
        return QJsonValue();
    }
    if (doc.isArray()) {
        return doc.array();
    } else if (doc.isObject()) {
        return doc.object();
    }
    return QJsonValue();
}

bool WriteFileAsJson(const QString &path, const QJsonValue &value, bool indent)
{
    QJsonDocument doc;
    if (value.isArray()) {
        doc = QJsonDocument(value.toArray());
    } else if (value.isObject()) {
        doc = QJsonDocument(value.toObject());
    } else {
        return false;
    }
    const QByteArray data = doc.toJson(indent ? QJsonDocument::Indented : QJsonDocument::Compact);
    if (data.isEmpty()) {
        return false;
    }
    return WriteFileAsString(path, QString::fromUtf8(data));                    /*!< 根据 QJsonDocument 文档，toJson 返回的数据以 UTF8 编码 */
}

QList<QList<double> > ReadCsvAsDouble(const QString &path, double invalidValue)
{
    typedef QList<double> Line;
    typedef QList<Line> Csv;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return Csv();
    }
    Csv data;
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        const QVector<QStringRef> parts = stream.readLine().splitRef(',');
        Line line;
        line.reserve(parts.size());
        for (auto it = parts.cbegin(); it != parts.cend(); ++it) {
            bool ok;
            double v = it->toDouble(&ok);
            line << (ok ? v : invalidValue);
        }
        data << line;
    }
    return data;
}
