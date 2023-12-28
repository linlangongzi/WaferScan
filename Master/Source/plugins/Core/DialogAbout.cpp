#include "DialogAbout.h"
#include "ui_DialogAbout.h"
#include "ui_DialogCredits.h"
#include "ui_DialogLicenseList.h"
#include "LicenseManager.h"
#include <QInputDialog>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMessageBox>
#include <QStatusBar>
#include <allrising/Util.h>

static const QList<QPair<QString, QString> > creditModules = {
    {"Qt", "LGPL"},
    {"Qt Xlsx", "MIT"},
    {"OpenCV", "OpenCV"},
    {"StackWalker", "StackWalker"},
    {"Fatcow Icons", "CC-BY-4.0"}
};

static Q_LOGGING_CATEGORY(debug, "core.about")

#define lic LicenseManager::instance()

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
//    setWindowModality(Qt::ApplicationModal);
    ui->setupUi(this);
    ui->device->SetReadOnly(true);

    QString appTitle = tr("<b><big>%1 %2</big></b> (%3)").arg(
        qApp->applicationDisplayName(),
        qApp->applicationVersion(),
        qApp->property("RevisionString").toString()
    );
    ui->appTitle->setText(appTitle);
    ui->copyright->setText(qApp->property("CopyrightTr").toString());
    ui->platform->setText(tr("Qt %1 %2 (Runtime %3)").arg(QT_VERSION_STR, qApp->property("MakeSpec").toString(), qVersion()));
    ui->buildTime->setText(tr("%1 %2").arg(qApp->property("BuildDate").toString(), qApp->property("BuildTime").toString()));
    ui->webportal->setText(tr("<a href=\"%1/\">%1").arg(qApp->property("WebPortal").toString()));
    ui->telephone->setText(qApp->property("Telephone").toString());

    connect(lic, &LicenseManager::licenseUpdated, this, &DialogAbout::refreshLicense);

    connect(ui->device, &LicenseEditor::RequestRecharge, this, [this](QString code) {
        qCDebug(debug) << "Recharging:" << code;
        QString ret = lic->rechargeDevice(ui->device->GetDevice().deviceId, code);
        if (ret.isEmpty()) {
            QMessageBox::information(this, QString(), tr("更新成功"));
        } else {
            QMessageBox::warning(this, QString(), ret);
        }
    });

    refreshLicense();
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

int DialogAbout::exec()
{
    isAutoOpened = false;
    return QDialog::exec();
}

void DialogAbout::done(int r)
{
    if (!lic->hasProductLicense() && parentWidget()) {
        parentWidget()->close();
    } else {
        QDialog::done(r);
    }
}

void DialogAbout::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);
    refreshLicense();
}

/*! 刷新授权信息 */
void DialogAbout::refreshLicense()
{
    LicenseManager *l = lic;

    LicenseDevice currentDev = l->currentDevice();
    QString currentId = l->currentDeviceId();
    ui->expired->hide();
    QWidget *page = ui->pageNormal;
    bool hasViewOnlyLicense = QApplication::arguments().contains("-viewonly");
    if (currentId.isEmpty() && !hasViewOnlyLicense) {
        show();
        l->pickUserLicense();
        page = ui->pageNotInitialized;
    } else if (!currentDev.IsValid()) {
        page = ui->pageNoDevice;
//        QDateTime lastSeen = l->lastSeenValid();
//        qint64 secs = lastSeen.secsTo(QDateTime::currentDateTime());
//        ui->countdown->setText(tr("%1").arg(secs)); // 设置倒计时显示
    }
    ui->stackedWidget->setCurrentWidget(page);

    bool hasLicense = l->hasProductLicense();
    LicenseLevel currentLevel = l->productLicenseLevel();
    ui->expired->setVisible(!hasLicense && currentDev.IsValid() && currentLevel.IsValid());
    if (!hasLicense && !hasViewOnlyLicense) {                                                          /* 没有检测到唯一的有效设备 */
        if (!isVisible()) {
            isAutoOpened = true;
            show();
        }
    } else {
        if (!parent()) {                                                        /* parent不存在，说明还在Platform还未加载 */
            accept();                                                           /* accept会让main()中about->exec()返回，继续加载Platform */
        }
        if (isAutoOpened) {
            close();
        }
    }

    if (isVisible()) {
        bool hasDevLicense = l->hasDevelopLicense();
        ui->device->SetDevice(currentDev);
        ui->editLicenseContainer->setVisible(hasDevLicense);
    }

    auto *mw = qobject_cast<QMainWindow *>(parent());
    if (mw && !statusLabel) {
        statusLabel = new QLabel;
        mw->statusBar()->addPermanentWidget(statusLabel);
        statusLabel->hide();
    }
    QDateTime expire = l->productLicenseExpire();
    auto daysRemaining = QDateTime::currentDateTime().daysTo(expire);
    if (statusLabel && hasLicense && daysRemaining < 7 && daysRemaining > 0) {
        statusLabel->show();
        statusLabel->setText(tr("<font color=red>授权剩余%1天</font>").arg(daysRemaining));
    }
}

/*! 显示第三方许可列表 */
void DialogAbout::on_credits_clicked()
{
    /* 创建对话框 */
    QDialog dialog(this);
    dialog.setWindowFlags(dialog.windowFlags() | Qt::Sheet);
    Ui::DialogCredits ui;
    ui.setupUi(&dialog);

    /* 创建文本 */
    QTextDocument *doc = ui.text->document();
    QTextCursor cursor(doc);
    QTextCharFormat titleFormat;
    titleFormat.setFont(QFont("Courier", 18));
    for (const auto &item: creditModules) {
        cursor.insertText(item.first, titleFormat);
        cursor.insertText("\n", QTextCharFormat());
        cursor.insertBlock();
        cursor.insertText(ReadFileAsString(tr(":/license/%1").arg(item.second)), QTextCharFormat());
        cursor.insertText("\n", QTextCharFormat());
        cursor.insertBlock();
    }
    /* 将光标移至开头 */
    ui.text->moveCursor(QTextCursor::Start);
    ui.text->ensureCursorVisible();

    dialog.exec();
}

/*! 显示“编辑授权”对话框 */
void DialogAbout::on_editLicense_clicked()
{
    QDialog dialog(this);
    Ui::DialogLicenseList ui;
    ui.setupUi(&dialog);

    connect(ui.list, &QListWidget::currentRowChanged, this, [&ui](int currentRow) {
        QListWidgetItem *item = ui.list->item(currentRow);
        if (item) {
            ui.device->SetDevice(item->data(Qt::UserRole).value<LicenseDevice>());
        }
    });

    connect(ui.device, &LicenseEditor::RequestRecharge, this, [&](QString code) {
        QString ret = lic->rechargeDevice(ui.device->GetDevice().deviceId, code);
        if (ret.isEmpty()) {
            QMessageBox::information(&dialog, QString(), tr("更新成功"));
            dialog.close();
        } else {
            QMessageBox::warning(&dialog, QString(), ret);
        }
    });

    connect(ui.device, &LicenseEditor::RequestUpdate, this, [&]() {
        const QString ret = lic->updateDevice(ui.device->GetDevice());
        if (ret.isEmpty()) {
            QMessageBox::information(&dialog, QString(), tr("写入成功"));
            dialog.close();
        } else {
            QMessageBox::warning(&dialog, QString(), ret);
        }
    });

    connect(ui.device, &LicenseEditor::RequestgenerateRecharge, this, [&](QString deviceId, QString targetPin, LicenseLevel level) {
        const QString ret = lic->generateRecharge(deviceId, targetPin, level);
        QInputDialog::getText(&dialog, QString(), tr("授权码"), QLineEdit::Normal, ret);
    });

    QFont boldFont;
    boldFont.setBold(true);
    LicenseDeviceList devices = lic->devices();
    for (const auto &device: devices) {
        auto item = new QListWidgetItem(ui.list);

        item->setText(tr("%1 (%2)").arg(device.deviceId, device.deviceDesc));
        item->setData(Qt::UserRole, QVariant::fromValue(device));
        /* 当前设备以粗体显示 */
        bool isCurrent = (device.deviceId == lic->currentDeviceId());
        item->setData(Qt::FontRole, isCurrent ? QVariant(boldFont) : QVariant());
    }

    dialog.exec();
}

void DialogAbout::on_credential_clicked()
{
    QString userName = QInputDialog::getText(this, QString(), tr("用户名"));
    if (userName.isEmpty()) {
        return;
    }
    QString password = QInputDialog::getText(this, QString(), tr("密码"), QLineEdit::Password);
    if (password.isEmpty()) {
        return;
    }
    lic->setCredential(userName, password);
}
