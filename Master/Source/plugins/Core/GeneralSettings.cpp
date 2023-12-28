#include "GeneralSettings.h"
#include "ICore.h"
#include "ui_GeneralSettings.h"
#include "CoreConstants.h"
#include <QDir>
#include <QSqlError>
#include <QStorageInfo>
#include <QListIterator>
#include <QFileDialog>

using namespace Core;

QString fancyFileSize(qint64 size)
{
    static const QStringList suffix({"KB", "MB", "GB", "TB"});
    qreal s = size;
    QStringListIterator it(suffix);
    QString unit = "Bytes";
    while (s >= 1024 && it.hasNext()) {
        s /= 1024;
        unit = it.next();
    }
    return QString("%1 %2").arg(s, 0, 'f', 2).arg(unit);
}

GeneralSettings::GeneralSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralSettings)
{
    ui->setupUi(this);

    auto *settings = ICore::settings();

    ui->docPath->setText(settings->value("Core/documentPath").toString());

    settings->beginGroup("Database");
    QString type = settings->value("type").toString();
    QString host = settings->value("host").toString();
    QString name = settings->value("name").toString();
    QString user = settings->value("user").toString();
    QByteArray passwordRaw = settings->value("password").toByteArray();
    settings->endGroup();

    ui->dbType->addItems(QSqlDatabase::drivers());
    ui->dbType->setCurrentText(type);
    ui->dbName->setText(name);
    ui->dbHost->setText(host);
    ui->dbUser->setText(user);
    if (!passwordRaw.isEmpty()) {
        ui->dbPassword->setText(QString::fromUtf8(qUncompress(passwordRaw)));
    }

    refreshStatus();
}

GeneralSettings::~GeneralSettings()
{
    delete ui;
}

void GeneralSettings::apply()
{
    auto *settings = ICore::settings();

    settings->setValue("Core/documentPath", ui->docPath->text());

    settings->beginGroup("Database");
    settings->setValue("type", ui->dbType->currentText());
    settings->setValue("host", ui->dbHost->text());
    settings->setValue("name", ui->dbName->text());
    settings->setValue("user", ui->dbUser->text());
    settings->setValue("password", qCompress(ui->dbPassword->text().toUtf8()));
    if (ui->dbType->currentText().isEmpty()) {
        settings->remove("type");
        settings->remove("host");
        settings->remove("name");
        settings->remove("user");
        settings->remove("password");
    }
    settings->endGroup();

    /* 重建数据库连接 */
    ui->dbStatus->setText(tr("正在连接..."));
    QApplication::processEvents();
    ICore::reopenDatabase();

    refreshStatus();
}

void GeneralSettings::refreshStatus()
{
    QDir docDir(ui->docPath->text());
    if (!docDir.exists()) {
        ui->docStatus->setText(tr("<img src=':/fatcow/16x16/error.png'> 位置不存在"));
    } else {
        QStorageInfo info(docDir);
        if (info.isReadOnly()) {
            ui->docStatus->setText(tr("<img src=':/fatcow/16x16/error.png'> 无法写入只读位置"));
        }
        ui->docStatus->setText(tr("<img src=':/fatcow/16x16/drive.png'> 可用空间 %1 / %2").arg(fancyFileSize(info.bytesAvailable())).arg(fancyFileSize(info.bytesTotal())));
    }

    auto db = ICore::database();
    QString text = db.isOpen() ? tr("<img src=':/fatcow/16x16/tick.png'> 正常") : tr("<img src=':/fatcow/16x16/error.png'> <font color='red'>错误</font>");
    auto err = db.lastError();
    if (err.isValid()) {
        text += "<br/>" + err.driverText();
        text += "<br/>" + err.databaseText();
    }
    ui->dbStatus->setText(text);
}

GeneralSettingsPage::GeneralSettingsPage(QObject *parent) :
    IOptionsPage(parent)
{
    setId(Constants::SETTINGS_ID_GENERAL);
    setDisplayName(tr("通用"));
    setCategory(Constants::SETTINGS_CATEGORY_CORE);
    setDisplayCategory(tr("通用设置"));
    setCategoryIcon(QIcon(":/fatcow/16x16/setting_tools.png"));
}

QWidget *GeneralSettingsPage::widget()
{
    if (!m_widget) {
        m_widget = new GeneralSettings;
    }
    return m_widget;
}

void GeneralSettingsPage::apply()
{
    m_widget->apply();
}

void GeneralSettingsPage::finish()
{
    delete m_widget;
}

void GeneralSettings::on_docPathBrowse_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择默认保存位置"), ui->docPath->text());
    if (dir.isEmpty()) {
        return;
    }
    ui->docPath->setText(dir);
}

void GeneralSettings::on_docPath_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    refreshStatus();
}
