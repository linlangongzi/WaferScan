#include "DialogScanTaskInfo.h"
#include "ui_DialogScanTaskInfo.h"
#include <Acquisition/ScanPathFactory.h>
#include <Acquisition/ScanPlan.h>
#include <Acquisition/ScanPlanModel.h>
#include <Acquisition/ScanTaskCustomField.h>
#include <Core/Platform.h>
#include <QInputDialog>
#include <QLineEdit>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "acquire.task.info");
static const char CUSTOM_INFO_KEY[] = "customInfoKey";

DialogScanTaskInfo::DialogScanTaskInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScanTaskInfo)
{
    ui->setupUi(this);
}

DialogScanTaskInfo::~DialogScanTaskInfo()
{
    delete ui;
}

//!* 显示ScanTask属性信息相关函数
void DialogScanTaskInfo::addCustomInfo(const QString &name, const QString &key, const QString &value)
{
    auto *valueEdit = new QLineEdit(value, this);
    ui->taskInfoLayout->addRow(name, valueEdit);
    valueEdit->setProperty(CUSTOM_INFO_KEY, key);
    m_customInfoEdits << valueEdit;
}

ScanTaskData DialogScanTaskInfo::scanTaskData() const
{
    ScanTaskData data;
    if (m_task) {
        data = m_task->data();
    }
    data.taskOperator = ui->taskOperator->text();
    for (auto *edit: m_customInfoEdits) {
        data.customData.insert(edit->property(CUSTOM_INFO_KEY).toString(), edit->text());
    }
    return data;
}

void DialogScanTaskInfo::setScanTaskData(const ScanTaskData &data)
{
    ui->createTime->setText(data.createTime.toString());
    ui->startTime->setText(data.startTime.toString());
    ui->endTime->setText(data.finishTime.toString());
    ui->filePath->setText(data.filePath);
    ui->taskOperator->setText(data.taskOperator);
    setCustomInfo(data.customData);
}

void DialogScanTaskInfo::setTask(ScanTask *task)
{
    m_task = task;
    if (!task) {
        qCWarning(debug) << "Can't set task info, Invalid task";
        return;
    }
    const auto data = task->data();
    setScanTaskData(data);
    setScanPlan(task->scanPlan());
    if (data != scanTaskData() ) {
        qFatal("ScanTaskData mismatch");
    }
}

void DialogScanTaskInfo::setEditable(bool enable)
{
    for (auto *lineEdit: m_customInfoEdits) {
        lineEdit->setEnabled(enable);
    }
}

void DialogScanTaskInfo::setCustomInfo(QMap<QString, QString> customInfo)
{
    for (auto *widget: m_customInfoEdits) {
        ui->taskInfoLayout->removeRow(widget);
    }
    m_customInfoEdits.clear();
    ScanTaskCustomFieldModel m_customFieldModel;
    m_customFieldModel.loadJson(platform()->Get("ReportKeys"));
    for (int rowIndex = 0; rowIndex < m_customFieldModel.rootItem()->childCount(); ++rowIndex) {
        ScanTaskCustomField *field = m_customFieldModel.rootItem()->childAt(rowIndex);
        if (!field) {
            qCWarning(debug) << "Invalid field";
            continue;
        }
        if (customInfo.contains(field->key)) {
            addCustomInfo(field->displayName, field->key, customInfo.value(field->key));
            customInfo.remove(field->key);
        }
    }
    customInfo.remove("Date");
    customInfo.remove("Time");
    customInfo.remove("Surface");
    for (auto it = customInfo.constBegin(); it != customInfo.constEnd(); ++it) {
        addCustomInfo(it.key(), it.key(), it.value());
    }
}

//!* 显示ScanPlan属性信息相关函数
void DialogScanTaskInfo::setScanPlan(const ScanPlan *plan)
{
    if (!plan) {
        qCCritical(debug) << "Can't show scan plan details: Invalid plan";
        return;
    }
    if (m_widget) {
        delete m_widget;
        m_widget = nullptr;
    }
    ui->name->setText(plan->name());
    auto *factory = ScanPathFactory::fromName(plan->pathType());
    if (factory) {
        m_widget = factory->widget(plan->options());
        factory->setWidgetEnabled(m_widget, false);
    } else {
        m_widget = new QLabel(tr("无法显示 %1 扫描计划").arg(plan->pathType()), this);
    }
    if (!m_widget) {
        qCCritical(debug) << "Scan path type" << factory << "has no widget";
        return;
    }
    m_widget->setParent(this);
    ui->scanPlan->addWidget(m_widget);
}

void DialogScanTaskInfo::on_buttonBox_accepted()
{
    if (m_task.isNull()) {
        qCCritical(debug) << "Invalid pointer";
        return;
    }
    m_task->setData(scanTaskData());
    accept();
}

static void searchFolder(ScanPlan *plan, QList<ScanPlan *> &list, QStringList &nameList, const int depth = 0)
{
    foreach (auto *child, plan->children()) {
        if (child->isFolder()) {
            list << child;
            nameList << QString("%1 %2").arg(nameList.count()).arg(QString(" ").insert(depth," ").append(child->name()));
            searchFolder(child, list, nameList, depth + 1);
        }
    }
}

void DialogScanTaskInfo::on_exportPlan_clicked()
{
    QList<ScanPlan *> optionList;
    QStringList nameList;
    searchFolder(ScanPlanModel::instance()->root(), optionList, nameList);

    bool ok = false;
    QString item = QInputDialog::getItem(this, tr("导出计划"), tr("候选项:"), nameList, 1, false, &ok);
    ScanPlan *selectedItem = optionList.value(nameList.indexOf(item));
    QModelIndex index = ScanPlanModel::instance()->indexOf(selectedItem);
    if (ok) {
        ScanPlanModel::instance()->insertNewScanPlan(m_task->scanPlan()->toJson(), index);
    }
}
