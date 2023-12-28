#pragma once

#include "allrising_global.h"
#include "MetaType/JsonConverter.h"
#include "MetaType/MetaType.h"
#include "QtExtend.h"
#include <QDataStream>
#include <QFile>
#include <QSaveFile>
#include <QtContainerFwd>
#include <QtGlobal>

#define ARS_CHECK(pointer)                                      if ( !(pointer) ) {return;}
#define ARS_CHECK_CONTINUE(pointer)                             if ( !(pointer) ) {continue;}
#define ARS_CHECK_BREAK(pointer)                                if ( !(pointer) ) {break;}
#define ARS_CHECK_RETURN(pointer, returnValue)                  if ( !(pointer) ) {return returnValue;}
#define ARS_CHECK_WARNING_RETURN(pointer, errMsg, returnValue)  if ( !(pointer) ) {qWarning() << errMsg; return returnValue;}
#define ARS_CHECK_WARNING(pointer, errMsg)                      if ( !(pointer) ) {qWarning() << errMsg; return;}
#define ARS_CHECK_BOUND(min, value, max)                        if ( (value) < (min) || (value) > (max) ) {return;}
#define ARS_CHECK_BOUND_CONTINUE(min, value, max)               if ( (value) < (min) || (value) > (max) ) {continue;}
#define ARS_CHECK_BOUND_RETURN(min, value, max, returnValue)    if ( (value) < (min) || (value) > (max) ) {return returnValue;}
#define ARS_CHECK_BOUND_WARNING(min, value, max, errMsg)        if ( (value) < (min) || (value) > (max) ) {qWarning() << errMsg; return;}

ALLRISING_UTILS_EXPORT double DistanceFromTime(double time, double speed);
ALLRISING_UTILS_EXPORT double TimeFromDistance(double distance, double speed);

ALLRISING_UTILS_EXPORT double InchToMillimetre(double inch);
ALLRISING_UTILS_EXPORT double MillimetreToInch(double mm);

class QString;
class QByteArray;
class QJsonValue;
class QVector3D;
class QStringList;
template <typename T> class QList;

ALLRISING_UTILS_EXPORT QString Vector3DToString(const QVector3D &v, int precision = 4);
ALLRISING_UTILS_EXPORT QVector3D Vector3DFromString(const QString &v);

/*! 以二进制格式读取指定文件的全文内容，读取失败时返回空字符串 */
ALLRISING_UTILS_EXPORT QByteArray ReadFileAsBinary(const QString &path, QString *errorString = nullptr);
/*! 以二进制格式写入指定文件，现有文件内容将全部被覆盖。写入成功返回true */
ALLRISING_UTILS_EXPORT bool WriteFileAsBinary(const QString &path, const QByteArray &data, QString *errorString = nullptr);

/*! 以文本格式读取指定文件，读取失败时返回空字符串。文件开头的BOM将被自动解析 */
ALLRISING_UTILS_EXPORT QString ReadFileAsString(const QString &path, QString *errorString = nullptr);
/*! 以文本格式写入指定文件，现有文件内容将全部被覆盖。写入成功返回true */
ALLRISING_UTILS_EXPORT bool WriteFileAsString(const QString &path, const QString &data, QString *errorString = nullptr);

ALLRISING_UTILS_EXPORT QStringList ReadFileAsStringList(const QString &path);

/*! 以Json格式读取指定文件，读取失败，或格式转换失败时返回Null */
ALLRISING_UTILS_EXPORT QJsonValue ReadFileAsJson(const QString &path, QString *errorString = nullptr);
/*! 以Json格式写入指定文件，现有文件内容将全部被覆盖。写入成功返回true */
ALLRISING_UTILS_EXPORT bool WriteFileAsJson(const QString &path, const QJsonValue &value, bool indent = true, QString *errorString = nullptr);

ALLRISING_UTILS_EXPORT QVariant readClipboard(int typeId);
ALLRISING_UTILS_EXPORT void writeClipboard(const QVariant &value);

template <typename T>
bool readFileAsDataStream(const QString &fileName, T &value, QString *errorString = nullptr)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return false;
    }

    QDataStream stream(&file);
    stream >> value;
    if (stream.status() != QDataStream::Ok) {
        if (errorString) {
            *errorString = "Bad format";
        }
        return false;
    }
    return true;
}

template <typename T>
bool writeFileAsDataStream(const QString &fileName, const T &value, QString *errorString = nullptr)
{
    QSaveFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        if (errorString) {
            *errorString = file.errorString();
        }
        return false;
    }

    QDataStream stream(&file);
    stream << value;
    if (stream.status() != QDataStream::Ok) {
        if (errorString) {
            *errorString = "Can't write file";
        }
        return false;
    }
    file.commit();
    return true;
}

template <typename T>
T readClipboard()
{
    return readClipboard(qMetaTypeId<T>()).template value<T>();
}

template <typename T>
void writeClipboard(const T &value)
{
    writeClipboard(QVariant::fromValue<T>(value));
}

template <typename T>
QList<T> ReadFileAsJsonArray(const QString &path, QString *errorString = nullptr)
{
    const QJsonArray array = ReadFileAsJson(path, errorString).toArray();
    return convertFromJson<QList<T> >(array);
}

template <typename T>
bool WriteFileAsJsonArray(const QString &path, const QList<T> &items, QString *errorString = nullptr)
{
    const QJsonValue array = convertToJson(items);
    return WriteFileAsJson(path, array, errorString);
}

/*! 读取由数字组成的Csv(Comma Seperated Values)文件 */
ALLRISING_UTILS_EXPORT QList<QList<double> > ReadCsvAsDouble(const QString &path, double invalidValue = 0);

/*! 获取一个整型数字的符号，为0时返回0；大于0时返回1，小于0时返回-1 */
template<typename T>
int arsSign(T value)
{
    if (value == 0) {
        return 0;
    } else {
        return value > 0 ? 1 : -1;
    }
}

/*! 获取一个浮点数字的符号，为0时返回0；大于0时返回1，小于0时返回-1 */
template<typename T>
int arsSignF(T value)
{
    if (qFuzzyIsNull(value)) {
        return 0;
    } else {
        return value > 0 ? 1 : -1;
    }
}

template<typename _Container>
QString JoinNumber(const _Container &number)
{
    QStringList l;
    for (const auto v: number) {
        l << QString::number(v);
    }
    return l.join(',');
}

template<typename T>
QList<T> SplitNumber(const QString &string, QString::SplitBehavior split = QString::KeepEmptyParts)
{
    QList<T> list;
    for (const QString &part: string.split(',', split)) {
        list << QVariant(part).value<T>();
    }
    return list;
}

template<typename Container, typename CreateFunc, typename RemoveFunc>
void resizeContainer(int size, Container &target, CreateFunc create, RemoveFunc remove)
{
    while (target.count() > size) {
        remove(target.takeLast());
    }
    while (target.count() < size) {
        target << create();
    }
}
