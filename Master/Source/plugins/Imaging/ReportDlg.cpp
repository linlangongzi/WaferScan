#include "ReportDlg.h"
#include "ui_ReportDlg.h"
#include "DefectItem.h"
#include "DefectItemModel.h"
#include <Core/ICore.h>
#include <Core/Platform.h>
#include <Imaging/PanelCScan.h>
#include <Imaging/UTDocument.h>
#include <Imaging/UTImage.h>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QToolButton>

static Q_LOGGING_CATEGORY(debug, "acquire.report");

void ReplaceKey(QString &source, const QString &key, const QString &value)
{
    static const QString pattern("$(%1)");
    source.replace(pattern.arg(key), value);
}

ReportDlg::ReportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDlg),
    customFieldModel(new ScanTaskCustomFieldModel(this))
{
    ui->setupUi(this);
    customFieldModel->loadJson(platform()->Get("ReportKeys"));
    InitUI();
}

ReportDlg::~ReportDlg()
{
    delete ui;
}

/*!
 * \brief ReportDlg::InitUI 初始化界面
 */
void ReportDlg::InitUI()
{
    connect(ui->cancel, &QAbstractButton::clicked, this, &ReportDlg::OnCancel);
    ui->generate->setVisible(false);
    RefreshUI();
}

void ReportDlg::RefreshUI()
{
    /* 存在widgets与model数量不匹配问题，以model数量为基准 */
    auto *root = customFieldModel->rootItem();
    /* 移除多余的行 */
    while (widgets.count() > root->childCount()) {
        ReportRow row = widgets.takeLast();
        delete row.name;
        delete row.valueList;
    }
    /* 创建不足的行 */
    while (widgets.count() < root->childCount()) {
        ReportRow row;
        row.name = new QLabel(this);
        row.name->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
        row.valueList = new QComboBox(this);
        row.valueList->setEditable(true);
        row.valueList->setInsertPolicy(QComboBox::InsertAlphabetically);
        row.valueList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        ui->formLayout->addRow(row.name, row.valueList);
        widgets.append(row);
    }
    Q_ASSERT(widgets.count() == root->childCount());
    /* 刷新控件文字 */
    for (int rowIndex = 0; rowIndex < root->childCount(); ++rowIndex) {
        ScanTaskCustomField *field = root->childAt(rowIndex);
        if (!field) {
            qCWarning(debug) << "Invalid field";
            break;
        }
        ReportRow &row = widgets[rowIndex];
        row.key = field->key;
        row.name->setText(field->displayName);
        row.name->setToolTip(field->key);
        row.valueList->clear();
        row.valueList->addItems(field->valueList);
        row.valueList->setCurrentText(field->lastValue);
    }
    ui->time->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    ui->date->setText(QTime::currentTime().toString("HH.mm"));
}

QString ReportDlg::askFileName(const QMap<QString, QString> customizedInfo,const QString &filter)
{
    QString suggestPath = platform()->Get("ReportFilePattern").toString();
    for (auto it = customizedInfo.constBegin(); it != customizedInfo.constEnd(); ++it) {
        ReplaceKey(suggestPath, it.key(), it.value());
    }
    QFileInfo suggestFileInfo(suggestPath);
    QDir().mkpath(suggestFileInfo.dir().path());
    if (platform()->Get("neverAskFileName").toBool() && !suggestPath.isEmpty()) {
        return suggestPath;
    }
    return QFileDialog::getSaveFileName(QApplication::activeWindow(), QString(), suggestPath, filter);
}

QMap<QString, QString> ReportDlg::customData(ScanPlan *plan) const
{
    QMap<QString, QString> customInfo;
    customInfo.insert("Date", QDate::currentDate().toString("yyyy-MM-dd"));
    customInfo.insert("Time", QTime::currentTime().toString("HH.mm"));
    customInfo.insert("Surface", plan->name());
    for (const auto &row: widgets) {
        const QString key = row.key;
        const QString value = row.valueList->currentText();
        customInfo.insert(key, value);
    }
    return customInfo;
}

void ReportDlg::OnCancel()
{
    reject();
}

void ReportDlg::SaveSettings()
{
    //! 将页面控件内用户输入的值写入Model中
    for (int rowIndex = 0; rowIndex < widgets.count(); ++rowIndex) {
        ScanTaskCustomField *field = customFieldModel->rootItem()->childAt(rowIndex);
        if (!field) {
            qCWarning(debug) << "Invalid field";
            break;
        }
        ReportRow &row = widgets[rowIndex];
        field->lastValue = row.valueList->currentText();
        if (!row.valueList->currentText().isEmpty() && (!field->valueList.contains(row.valueList->currentText()))) {
            field->valueList.append(row.valueList->currentText());
        }
        field->update();
    }
    platform()->Set("ReportKeys", customFieldModel->toJson());
}

void ReportDlg::on_save_clicked()
{
    int ret = CheckData();
    if (ret != 0) {
        if (ret > 0) {
            const ReportRow &row = widgets[ret - 1];
            QMessageBox::warning(this, QString(), tr("%1 不能为空").arg(row.name->text()));
        } else {
            QMessageBox::warning(this, QString(), tr("文件名不能为空"));
            return;
        }
    }
    SaveSettings();
    accept();
}

/*!
 * \brief ReportDlg::CheckData    校验当前填写数据是否有效
 * \return
 */
int ReportDlg::CheckData()
{
    for (int rowIndex = 0; rowIndex < widgets.count(); ++rowIndex) {
        ScanTaskCustomField *field = customFieldModel->rootItem()->childAt(rowIndex);
        if (!field) {
            qCWarning(debug) << "Invalid field";
            break;
        }
        const ReportRow &row = widgets[rowIndex];
        if (row.valueList->currentText().isEmpty() && field->isRequired) {
            return rowIndex + 1;
        }
    }
    return 0;
}
