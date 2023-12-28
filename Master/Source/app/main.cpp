#include <Acquisition/AcquisitionPlugin.h>
#include <Ars1000/Ars1000Plugin.h>
#include <Ars3000/Ars3000Plugin.h>
#include <Ars500/Ars500Plugin.h>
#include <Core/CorePlugin.h>
#include <Core/ICore.h>
#include <Gamepad/GamepadPlugin.h>
#include <Imaging/ImagingPlugin.h>
#include <Legacy/LegacyPlugin.h>
#include <Mdo3000/Mdo3000Plugin.h>
#include <Motion/MotionPlugin.h>
#include <Pipe/PipePlugin.h>
#include <Plot/PlotPlugin.h>
#include <QApplication>
#include <QDesktopServices>
#include <QDir>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QMutex>
#include <QPainter>
#include <QRegularExpression>
#include <QResource>
#include <QSettings>
#include <QTranslator>
#include <QUrl>
#include <QVersionNumber>
#include <Ultrasonic/UltrasonicPlugin.h>
#include <Waveform/WaveformPlugin.h>
#include <allrising/Util.h>
#include <app/app_version.h>
#include <extensionsystem/pluginmanager.h>
#include <iostream>
#include <utils/algorithm.h>
#include <utils/fileutils.h>

#ifdef ENABLE_QT_BREAKPAD
#   include <qtsystemexceptionhandler.h>
#endif

#if defined(PIPE_DETECT)
#   include <Pipe/PipeManager.h>
#elif defined(TRAINWHEEL)
#   include <Wheel/ControlPanel.h>
#endif

using namespace Core;
using namespace Core::Constants;
using namespace ExtensionSystem;
using App = QApplication;

static Q_LOGGING_CATEGORY(debug, "app");

class SplashScreen : public QSplashScreen
{
public:
    void drawContents(QPainter *painter) override
    {
        painter->drawText(270, 300, tr("版本 %1 (%2)").arg(qApp->applicationVersion(), qApp->property("RevisionString").toString()));
        painter->drawText(270, 315, message());
    }
};

/*! 解析Svn或Git-Svn版本号URL，将其转换为git commit id或svn revision */
static void resolveRevision(const QString &type, QString revision, QString branch)
{
//    qCDebug(debug) << "Extracting" << type << revision << branch;
    /* 解析和抽取版本号 */
    /* svn:     "8547M" */
    /* git-svn: "c1a744f:git-svn-id: http://allrising207.f3322.org:8001/svn/branches/develop@8547 cc7abf6a-83b8-4a35-b4d7-34ad2fce7e68" */
    /* git:     "c1a744f:" */
    if (type == "svn") {
        revision = QRegularExpression("^(\\d+)").match(revision).captured(1);
        if (branch.contains("branches")) {
            branch = QRegularExpression("branches/([^/]+)").match(branch).captured(1);
        } else if (branch.contains("tags")) {
            branch = QRegularExpression("tags/([^/]+)").match(branch).captured(1);
        } else if (branch.contains("trunk")) {
            branch = "master";
        }
    } else {
        const QRegularExpressionMatch revisionGit = QRegularExpression("(\\w+):(git-svn-id:[^@]*/svn/([^@]*)@(\\d+))?").match(revision);

        const QString gitCommit = revisionGit.captured(1);
        bool hasGitSvnId = false;
        const int gitSvnId = revisionGit.captured(4).toInt(&hasGitSvnId);
        branch = revisionGit.captured(3).section('/', -1);
        if (hasGitSvnId) {
            revision = QString("r%1 - %2").arg(gitSvnId).arg(gitCommit);        /* git-svn包含了svn版本号，两个版本号均显示。主分支不显示分支名 */
        } else {
            revision = gitCommit;                                               /* 未包含svn信息，只显示git commit id */
        }
    }
    if (revision.isEmpty()) {
        revision = "unknown";
    }
    qApp->setProperty("Revision", revision);
    qApp->setProperty("Branch", branch);
    qApp->setProperty("RevisionType", type);
    QString revisionString = revision;
    if (branch != "master") {
        revisionString += "-" + branch;
    }
    qApp->setProperty("RevisionString", revisionString);
}

static void initializeApplication(App *a)
{
    App::setWindowIcon(QIcon(":/Logo.ico"));
    auto *dsTranslator = new QTranslator(a);
    dsTranslator->load("WaferScan_zh_CN.qm", "translations");
    App::installTranslator(dsTranslator);
#ifdef PIPE_DETECT
    translator->load("PipeDetect_zhCN.qm");
#elif defined(TRAINWHEEL)
    translator->load("TrainWheelDetect_zhCN.qm");
#endif
    auto *qtTranslator = new QTranslator(a);
    qtTranslator->load("qt_zh_CN.qm", "translations");
    App::installTranslator(qtTranslator);

#ifdef PIPE_DETECT
    App::setApplicationName(QObject::tr("ARS-PipeDetect"));
    App::setApplicationDisplayName(QObject::tr("ARS-PipeDetect"));
    App::setApplicationVersion("2.0");
#elif defined(TRAINWHEEL)
    App::setApplicationName(App::translate("main", "TrainWheelDetect"));
    App::setApplicationDisplayName(App::translate("main", "南车机车车轮检测系统"));
    App::setApplicationVersion(DS_VERSION_STR);
#else
    App::setApplicationName(App::translate("main", "WaferScan"));
    App::setApplicationDisplayName(App::translate("main", "WaferScan"));
    App::setApplicationVersion(DS_VERSION_LONG);
#endif
    resolveRevision(DS_REVISION_TYPE, DS_REVISION_STR, DS_BRANCH_STR);
    App::setOrganizationName(DS_AUTHOR);
    App::setOrganizationDomain(DS_ORGANIZATION_DOMAIN);
    QDate buildDate = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), "MMM d yyyy");
    QTime buildTime = QTime::fromString(__TIME__);
    a->setProperty("BuildTime", buildTime);
    a->setProperty("BuildDate", buildDate);
    a->setProperty("Copyright", QString("Copyright(c) %1 - %2 %3. All rights reserved.").arg(DS_YEAR_START).arg(buildDate.year()).arg(DS_AUTHOR_LONG));
    a->setProperty("CopyrightTr", App::translate("main", "版权所有 © %1 - %2  奥瑞视(北京)科技有限公司").arg(DS_YEAR_START).arg(buildDate.year()));
    a->setProperty("MakeSpec", QString(DS_QMAKE_SPEC).section("/", -3, -3));
    a->setProperty("Telephone", DS_TELEPHONE);
    a->setProperty("WebPortal", DS_WEBPORTAL);
    a->setProperty("StartTime", QDateTime::currentDateTime());

    qCInfo(debug).noquote() << App::applicationName() << App::applicationVersion()
        << "Rev" << a->property("RevisionString").toString()
        << "Build" << buildDate.toString() << buildTime.toString();
    qCInfo(debug).noquote() << a->property("Copyright").toString();

    QFont font = App::font();
    if (font.pointSizeF() > 0 && font.pointSizeF() < 9) {                       /* 在非中文操作系统上，默认字体小于9号，难以识别 */
        QFont f("Microsoft Yahei", 9);
        App::setFont(f);                                                          /* 对于默认构造的字体，setFont只会为qApp设置在f中指定过的属性 */
    }
}

static QSettings *createSettings()
{
    QSettings dummy(QSettings::IniFormat, QSettings::UserScope, "Allrising", "WaferScan");
    return new QSettings(QFileInfo(dummy.fileName()).dir().filePath("WaferScan/WaferScan.ini"), QSettings::IniFormat);
}

/*! 检查配置文件版本, 备份低版本配置文件 */
static bool checkSettingsVersion()
{
    QSettings *settings = PluginManager::settings();

    QVersionNumber appVersion = QVersionNumber::fromString(App::applicationVersion());
    QVersionNumber profileVersion = QVersionNumber::fromString(settings->value("Version").toString());
    if (profileVersion.isNull()) {
        profileVersion = appVersion;
    }

    const QFileInfo currentPath = QFileInfo(settings->fileName()).dir().path();
    if (profileVersion > appVersion) {
        qCInfo(debug) << "Profile " << profileVersion << "higher than app version" << appVersion << ", aborting";
        auto ret = QMessageBox::critical(
            nullptr,
            QString(),
            App::translate("main", "配置文件版本错误：%1 > %2，软件即将关闭。\n\n选择'打开'查看配置文件。").arg(profileVersion.toString(), appVersion.toString()),
            QMessageBox::Open | QMessageBox::Close
        );
        if (ret == QMessageBox::Open) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(currentPath.filePath()));
        }
        return false;
    }
    if (profileVersion < appVersion) {
        PluginManager::splashMessage(App::translate("main", "正在备份配置"));
        qCInfo(debug) << "Profile" << profileVersion << "lower than app version" << appVersion;
        const QFileInfo backupPath = currentPath.filePath() + "-" + profileVersion.toString();
        qCInfo(debug) << "Backing up profile from" << currentPath.fileName() << "to" << backupPath.fileName();
        using namespace Utils;
        FileUtils::copyRecursively(FileName(currentPath), FileName(backupPath));
    }
    settings->setValue("Version", App::applicationVersion());
    return true;
}

static QTextStream logStream;
static QMutex logMutex;
static QtMessageHandler qtDefaultMessageHandler = nullptr;

static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#ifdef ARS_FINAL_RELEASE
    const QString message = qFormatLogMessage(type, context, msg);
    logMutex.lock();
    logStream << message << endl;
    logMutex.unlock();
#else
    qtDefaultMessageHandler(type, context, msg);
    switch (type) {                                                             /* 在 return 处设置断点可以跟踪 debug 消息. */
    case QtDebugMsg:
        return;
    case QtInfoMsg:
        return;
    case QtWarningMsg:
        type = QtCriticalMsg;
        return;
    case QtCriticalMsg:
        return;
    case QtFatalMsg:
        return;
    }
#endif
}

static const char RESOURCE_PATH[] =
#ifdef Q_OS_MAC
    "/../Resources";
#else
    "/resources";
#endif

int main(int argc, char *argv[])
{
    App a(argc, argv);

#ifdef ARS_FINAL_RELEASE
    QDir dir;
    dir.mkdir("Log");
    QFile logFile("Log/" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss") + ".log");
    logFile.open(QIODevice::WriteOnly);
    logStream.setDevice(&logFile);
#endif
#if defined(ARS_FINAL_RELEASE) || defined(QT_DEBUG)
    qtDefaultMessageHandler = qInstallMessageHandler(messageHandler);
#endif

    qSetMessagePattern("%{time}[%{type}][%{threadid}]%{category}: %{message}");
    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");

    initializeApplication(&a);

    auto *settings = createSettings();

#ifdef ENABLE_QT_BREAKPAD
    const QString dumpPath = QFileInfo(settings->fileName()).dir().path() + "/CrashLog";
    QtSystemExceptionHandler handler(QCoreApplication::applicationDirPath(), dumpPath);
    QDir().mkpath(dumpPath);
    Q_UNUSED(handler);
#endif

    PluginManager pluginManager;
    PluginManager::setSettings(settings);
    if (!checkSettingsVersion()) {
        return 0;
    }

    SplashScreen splash;
    PluginManager::setSplash(&splash);
    splash.setPixmap(QPixmap(":/WaferScan/Splash.png"));
    splash.show();
    PluginManager::splashMessage(App::translate("main", "正在启动"));

    qApp->processEvents();

    PluginManager::splashMessage(App::translate("main", "正在加载资源"));
    QResource::registerResource(QCoreApplication::applicationDirPath() + RESOURCE_PATH + "/Fatcow");

    PluginManager::splashMessage(App::translate("main", "正在初始化界面"));
    QList<ExtensionSystem::IPlugin *> plugins;
    QString errorString;

    plugins << new Core::Internal::CorePlugin;
    plugins << new Core::Internal::UltrasonicPlugin;
    plugins << new Core::Internal::MotionPlugin;
    plugins << new Core::Internal::WaveformPlugin;
    plugins << new Core::Internal::ImagingPlugin;
    plugins << new Core::Internal::AcquisitionPlugin;
    plugins << new Core::Internal::GamepadPlugin;
    plugins << new Core::Internal::Ars3000Plugin;
    plugins << new Core::Internal::Mdo3000Plugin;
    plugins << new Core::Internal:: Ars500Plugin;
    plugins << new Core::Internal::Ars1000Plugin;
    plugins << new Core::Internal::PlotPlugin;
    plugins << new Core::Internal::LegacyPlugin;

#ifdef PIPE_DETECT
    plugins << new Core::Internal::PipePlugin;
#elif defined(TRAINWHEEL)
    new ControlPanel(&w);
#endif

    for (auto *plugin: plugins) {
        if (!plugin->initialize(App::arguments(), &errorString)) {
            return 0;
        }
    }

    Utils::reverseForeach(plugins, [](IPlugin *plugin) {
        plugin->extensionsInitialized();
    });

    splash.finish(ICore::mainWindow());

    int ret = App::exec();

    for (auto *plugin: plugins) {
        plugin->aboutToShutdown();
    }
    Utils::reverseForeach(plugins, [](IPlugin *plugin) {
        delete plugin;
    });
    plugins.clear();

    return ret;
}
