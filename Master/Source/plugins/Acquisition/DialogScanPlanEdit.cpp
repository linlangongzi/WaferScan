#include "DialogScanPlanEdit.h"
#include "ui_DialogScanPlanEdit.h"
#include "ScanPathAdvanced.h"
#include "ScanPathFactory.h"
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "acquire.path.edit");

DialogScanPlanEdit::DialogScanPlanEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScanPlanEdit)
{
    ui->setupUi(this);

    m_scan = new ScanPlan(this);
}

DialogScanPlanEdit::~DialogScanPlanEdit()
{
    delete ui;
}

void DialogScanPlanEdit::setNameReadonly(bool readOnly)
{
    ui->name->setDisabled(readOnly);
}

ScanPlan *DialogScanPlanEdit::scanPlan() const
{
    ScanPlan *scan = m_scan;
    QString type = scan->pathType();
    auto *factory = ScanPathFactory::fromName(type);
    if (factory) {
        scan->setOptions(factory->widgetOptions(m_widget));
    }
    scan->setPathType(type);
    scan->setName(ui->name->text());
    return scan;
}

void DialogScanPlanEdit::setScanPlan(ScanPlan *scanPlan)
{
    if (m_widget) {
        delete m_widget;
        m_widget = nullptr;
    }
    m_scan->fromJson(scanPlan->toJson());
    ui->name->setText(scanPlan->name());

    auto *factory = ScanPathFactory::fromName(scanPlan->pathType());
    if (!factory) {
        qCCritical(debug) << "Invalid scan path type" << scanPlan->pathType() << ", fallback to advanced";
        factory = ScanPathFactory::advancedFactory();
        if (!factory) {
            qCCritical(debug) << "Can't load advanced scan path";
            return;
        }
        m_scan->setPathType(factory->name());
    }
    qCDebug(debug) << "Building ui for" << scanPlan->pathType() << factory;
    m_widget = factory->widget(scanPlan->options());
    if (!m_widget) {
        qCCritical(debug) << "Scan path type" << factory << "has no widget";
        return;
    }
    m_widget->setParent(this);
    ui->mainLayout->addWidget(m_widget);
    setWindowTitle(tr("编辑%1 - %2").arg(factory->displayName(), scanPlan->name()));
}
