#ifndef ARSCORE_UTIL_H
#define ARSCORE_UTIL_H

#define ARS_ASSERT_MAIN_THREAD      Q_ASSERT_X(qApp->thread() == QThread::currentThread(), __FILE__, "Attempt to call outside main thread");
#define ARS_ASSERT_THREAD           Q_ASSERT_X(thread() == QThread::currentThread(), __FILE__, "Attempt to call slot function on different thread");

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

#ifdef Q_CC_MSVC
class QDebug;
QDebug &DebugWithQObjectHeader(QDebug &debug, const char *name);
#define arsDebug       DebugWithQObjectHeader(qDebug(), staticMetaObject.className())
#define arsWarning     DebugWithQObjectHeader(qWarning(), staticMetaObject.className())
#define arsCritical    DebugWithQObjectHeader(qCritical(), staticMetaObject.className())
#else
#define arsDebug qDebug()
#define arsWarning qWarning()
#define arsCritical qCritical()
#endif

double DBFromPercent(double percent, double reference = 100);
double DBToPercent(double db, double reference = 100);
double DistanceFromTime(double time, double speed);
double TimeFromDistance(double distance, double speed);

double InchToMillimetre(double inch);
double MillimetreToInch(double mm);

class QString;
class QByteArray;
class QJsonValue;
QByteArray ReadFileAsBinary(const QString &path);
bool WriteFileAsBinary(const QString &path, const QByteArray &data);
QString ReadFileAsString(const QString &path);
bool WriteFileAsString(const QString &path, const QString &data);
QJsonValue ReadFileAsJson(const QString &path);
bool WriteFileAsJson(const QString &path, const QJsonValue &value, bool indent = true);

#include <QtContainerFwd>
QList<QList<double> > ReadCsvAsDouble(const QString &path, double invalidValue = 0);

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include "Util/JsonConverts.h"
#endif

#if QT_VERTION < QT_VERSION_CHECK(5, 0, 0) && !defined(Q_STATIC_ASSERT)
#    define Q_STATIC_ASSERT()
#endif

#include <QMetaEnum>
template<typename T>
T KeyToEnum(const QString &key, T defaultValue)
{
    const QMetaEnum meta = QMetaEnum::fromType<T>();
    bool ok = false;
    int v = meta.keyToValue(qPrintable(key), &ok);
    return ok ? T(v) : defaultValue;
}

template<typename T>
QString EnumToKey(T value)
{
    const QMetaEnum meta = QMetaEnum::fromType<T>();
    return meta.valueToKey(value);
}

template<typename T>
int arsSign(T value)
{
    if (value == 0) {
        return 0;
    } else {
        return value > 0 ? 1 : -1;
    }
}

template<typename T>
int arsSignF(T value)
{
    if (qFuzzyIsNull(value)) {
        return 0;
    } else {
        return value > 0 ? 1 : -1;
    }
}

#endif                                                                          // ARSCORE_UTIL_H
