/* 强制 MSVC 以 UTF-8 作为执行字符集。这个头文件应该作为预编译头使用 */
#ifdef _MSC_VER
#   if _MSC_VER >= 1600
#       pragma execution_character_set("utf-8")
#   endif
#endif

#ifdef __cplusplus

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4100)
#endif
#include <functional>
#ifdef _MSC_VER
#   pragma warning(pop)
#endif

#include <QtGlobal>

#include <QCoreApplication>
#include <QList>
#include <QVariant>
#include <QObject>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QPointer>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QLoggingCategory>

#include <stdlib.h>

#endif // __cplusplus
