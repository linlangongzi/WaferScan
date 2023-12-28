#pragma once

#include <QSettings>
#include <QtSql/QSqlDatabase>
#include "CoreGlobal.h"
#include "MainWindow.h"

class QWindow;
class IDocument;

namespace Core {

class MainWindow;

class CORE_EXPORT ICore : public QObject
{
    Q_OBJECT

public:
    explicit ICore(MainWindow *parent);

    static ICore *instance();

    static QSettings *settings(QSettings::Scope scope = QSettings::UserScope);
    static void saveSettings();
    static QString settingsPath();
    static QString resourcePath();
    static QString documentPath();
    static QWidget *dialogParent();
    static MainWindow *mainWindow();
    static IDocument *document();
    static QSqlDatabase database();
    static void reopenDatabase();
    static void showSettings();
    static void registerWindow(QWindow *window);
    static bool isInternalVersion();

    template <typename Func>
    static void onInternalVersion(QObject *parent, Func func)
    {
        QMetaObject::Connection connection = connect(instance(), &ICore::internalVersionChanged, func);
        if (parent) {
            connect(parent, &QObject::destroyed, [connection](QObject *) {
                QObject::disconnect(connection);
            });
        }
        func(isInternalVersion());
    }

signals:
    void aboutToShutdown();
    void internalVersionChanged(bool isInternal);
};

}   // namespace Core
