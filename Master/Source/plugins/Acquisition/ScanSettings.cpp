#include "ScanSettings.h"
#include "ui_ScanSettings.h"
#include "AcquisitionConstants.h"
#include <Core/Platform.h>
#include <QFileDialog>
#include <QInputDialog>
#include <QLoggingCategory>
#include <Ultrasonic/UltrasonicManager.h>

static Q_LOGGING_CATEGORY(debug, "acquire.task.settings");

ScanSettings::ScanSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanSettings)
{
    ui->setupUi(this);

    //! 初始化并添加编辑字段的Model到TreeView中
    m_customFieldModel = new ScanTaskCustomFieldModel(this);
    m_customFieldModel->setHeader(QStringList{tr("必填项"), tr("字段名"), tr("名称"), tr("备选项")});
    ui->editField->setModel(m_customFieldModel);

    ui->scanWithoutDatum->setChecked(platform()->Get("MotionScanWithoutDatum").toBool());
    ui->showLegacyScanPlan->setChecked(platform()->Get("ShowLegacyScanPlan").toBool());
    ui->neverAskFileName->setChecked(platform()->Get("neverAskFileName").toBool());
    ui->dataLevel->setCurrentIndex(ui->dataLevel->findData(platform()->Get("DataLevel").toInt()));
    ui->outputFormat->setText(platform()->Get("ReportFilePattern").toString());
    ui->dataLevel->addItem(tr("A扫数据"), IInstrumentDriver::ASCAN_ONLY);
    ui->dataLevel->addItem(tr("C扫数据"), IInstrumentDriver::CSCAN_ONLY);
    ui->dataLevel->setCurrentIndex(0);

    ui->templatePath->setReadOnly(true);
    ui->templatePath->setText(platform()->Get("ReportTemplate").toString());

    m_customFieldModel->loadJson(platform()->Get("ReportKeys"));
}

ScanSettings::~ScanSettings()
{
    delete ui;
}

void ScanSettings::apply()
{
    platform()->Set("ReportKeys", m_customFieldModel->toJson());
    platform()->Set("ReportFilePattern", ui->outputFormat->text());
    platform()->Set("neverAskFileName", ui->neverAskFileName->isChecked());
    platform()->Set("DataLevel", ui->dataLevel->currentData().toInt());
    platform()->Set("MotionScanWithoutDatum", ui->scanWithoutDatum->isChecked());
    platform()->Set("ShowLegacyScanPlan", ui->showLegacyScanPlan->isChecked());
    platform()->Set("ReportTemplate", ui->templatePath->text());
}

//! 编辑字段增加行Button槽函数
void ScanSettings::on_addField_clicked()
{
    const QString name = QInputDialog::getText(this, QString(), tr("字段名 (限英文):"));
    if (name.isEmpty()) {
        return;
    }
    auto *field = new ScanTaskCustomField;
    field->displayName = name;
    field->key = name;
    m_customFieldModel->rootItem()->appendChild(field);
}

//! 编辑字段减少行Button槽函数
void ScanSettings::on_deleteField_clicked()
{
    auto *item = m_customFieldModel->itemForIndex(ui->editField->currentIndex());
    if (!item) {
        qCDebug(debug) << "Can't remove row: invalid item";
        return;
    }
    m_customFieldModel->destroyItem(item);
}

void ScanSettings::on_dataLevel_currentIndexChanged(int index)
{
    int level = ui->dataLevel->itemData(index).toInt();
    for (auto *channel: UltrasonicManager::channels()) {
        channel->setDataLevel(level);
    }
}

void ScanSettings::on_editTemplatePath_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("选择模板文件"), QString(), tr("Excel报表模板 (*.xlsx)"));
    if (!path.isEmpty()) {
        ui->templatePath->setText(path);
    }
}

ScanSettingsPage::ScanSettingsPage(QObject *parent) :
    IOptionsPage(parent)
{
    using namespace Acquisition;
    setId(SETTINGS_ID_ACQUISITION);
    setDisplayName(tr("扫描"));
    setCategory(SETTINGS_CATEGORY_ACQUISITION);
    setDisplayCategory(tr("扫描设置"));
    setCategoryIcon(QIcon(":/fatcow/16x16/resource_usage.png"));
}

QWidget *ScanSettingsPage::widget()
{
    if (!m_widget) {
        m_widget = new ScanSettings;
    }
    return m_widget;
}

void ScanSettingsPage::apply()
{
    m_widget->apply();
}

void ScanSettingsPage::finish()
{
    delete m_widget;
}
