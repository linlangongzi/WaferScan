#include "MainWindow.h"
#include <QApplication>
#include <QDebug>
#include <QRegularExpression>
#include <QTranslator>
#include "SvnRevision.inc"

#define ARS_VERSION_MAJOR 1
#define ARS_VERSION_MINOR 0
#define ARS_VERSION_STR QT_STRINGIFY(ARS_VERSION_MAJOR) "." QT_STRINGIFY(ARS_VERSION_MINOR)

QString ResolveRevision(const QString &text)
{
    /* 解析和抽取版本号 */
    /* svn:     "8547M" */
    /* git-svn: "c1a744f:git-svn-id: http://allrising207.f3322.org:8001/svn/branches/develop@8547 cc7abf6a-83b8-4a35-b4d7-34ad2fce7e68" */
    /* git:     "c1a744f:" */
    /* 尝试捕获一个Svn版本号(任意数字) */
    const QString revision = QRegularExpression("^(\\d+)").match(text).captured(1);
//    const QString revision = QRegularExpression(":\\d+").match(text).captured(0).mid(1);
    return revision;    // 9039:9083M revisionGit.hasMatch()为true, 进入git解析，暂时return
    /* 代码库可能是 git 或 git-svn */
    const QRegularExpressionMatch revisionGit = QRegularExpression("(\\w+):(git-svn-id:[^@]*/svn/([^@]*)@(\\d+))?").match(text);
    if (!revisionGit.hasMatch()) {
        if (revision.isEmpty()) {
            return text;
        } else {
            return revision;
        }
    }
    const QString gitCommit = revisionGit.captured(1);
    bool hasGitSvnId = false;
    const int gitSvnId = revisionGit.captured(4).toInt(&hasGitSvnId);
    const QString branch = revisionGit.captured(3).section('/', -1);
    if (hasGitSvnId) {
        if (branch == "trunk") {
            return QStringLiteral("r%1 - %2").arg(gitSvnId).arg(gitCommit);     /* git-svn包含了svn版本号，两个版本号均显示。主分支不显示分支名 */
        } else {
            return QStringLiteral("r%1 - %2 - %3").arg(gitSvnId).arg(gitCommit).arg(branch);
        }
    } else {
        return gitCommit;                                                       /* 未包含svn信息，只显示git commit id */
    }
}

void InitializeApplication(QApplication *a)
{
//    QTranslator *translator = new QTranslator(a);
//    translator->load("_zhCN.qm");
//    a->installTranslator(translator);
//    a->setWindowIcon(QIcon(":/Logo.ico"));
    a->setApplicationName(QObject::tr("TrainWheel_PXI"));
    a->setApplicationVersion(ARS_VERSION_STR);
    a->setProperty("Revision", ResolveRevision(ARS_REVISION_STR));
    qDebug() << ARS_REVISION_STR << a->property("Revision");
    QDate buildDate = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), "MMM d yyyy");
    QTime buildTime = QTime::fromString(__TIME__);
    a->setProperty("BuildTime", buildTime);
    a->setProperty("BuildDate", buildDate);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InitializeApplication(&a);
    MainWindow w;
    w.show();
    qDebug()<<"the main thread is: " << QThread::currentThreadId();

    return a.exec();
}
