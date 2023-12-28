#include "InstrumentOptions.h"
#include "ui_InstrumentOptions.h"
#include "UltrasonicManager.h"
#include "UltrasonicConstants.h"
#include "InstrumentModel.h"

#include <QInputDialog>
#include <QMessageBox>

#include <Core/LicenseManager.h>

using namespace Ultrasonic;

InstrumentOptions::InstrumentOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentOptions)
{
    ui->setupUi(this);

    model = new InstrumentModel(this);
    ui->treeView->setModel(model);
    ui->edit->hide();

    LicenseManager::registerLicenseUpdate(this, &InstrumentOptions::onLicenseUpdated);
}

InstrumentOptions::~InstrumentOptions()
{
    delete ui;
}

void InstrumentOptions::onLicenseUpdated()
{
    bool develop = LicenseManager::instance()->hasDevelopLicense();
    ui->add->setEnabled(develop);
    ui->remove->setEnabled(develop);
}

void InstrumentOptions::on_treeView_activated(const QModelIndex &index)
{
    auto *instrument = model->instrument(index);
    if (instrument) {
        ui->edit->setInstrument(instrument);
        ui->edit->show();
    } else {
        ui->edit->hide();
    }
}

InstrumentOptionsPage::InstrumentOptionsPage(QObject *parent) :
    IOptionsPage(parent)
{
    setId(SETTINGS_ID_INSTRUMENT);
    setCategory(SETTINGS_CATEGORY_ULTRASONIC);
    setCategoryIcon(QIcon(SETTINGS_ICON_ULTRASONIC));
    setDisplayCategory(tr("仪器"));
    setDisplayName(tr("仪器"));
}

QWidget *InstrumentOptionsPage::widget()
{
    if (!m_widget) {
        m_widget = new InstrumentOptions;
    }
    return m_widget;
}

void InstrumentOptionsPage::apply()
{

}

void InstrumentOptionsPage::finish()
{
    delete m_widget;
}

void InstrumentOptions::on_add_clicked()
{
    QString name = QInputDialog::getText(this, QString(), tr("仪器名称:"));
    if (name.isEmpty()) {
        return;
    }
    bool ok = false;
    auto types = UltrasonicManager::availableDriverNames();
    QString type = QInputDialog::getItem(this, QString(), tr("仪器类型:"), types, 0, false, &ok);
    if (!ok) {
        return;
    }
    UTInstrument *instrument = UltrasonicManager::createInstrument();
    InstrumentConfig config;
    config.name = name;
    config.deviceType = type;
    instrument->setConfig(config);
}

void InstrumentOptions::on_clone_clicked()
{
    UTInstrument *instrument = model->instrument(ui->treeView->currentIndex());
    if (!instrument) {
        return;
    }
    QString name = QInputDialog::getText(this, QString(), tr("新名称:"));
    if (name.isEmpty()) {
        return;
    }
    InstrumentConfig config = instrument->config();
    config.name = name;
    UTInstrument *newInstrument = UltrasonicManager::createInstrument();
    newInstrument->setConfig(config);
}

void InstrumentOptions::on_remove_clicked()
{
    UTInstrument *instrument = model->instrument(ui->treeView->currentIndex());
    if (!instrument) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定要删除超声仪器 %1 吗?").arg(instrument->name()), QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok) {
        return;
    }
    if (instrument == ui->edit->instrument()) {
        ui->edit->setInstrument(nullptr);
        ui->edit->hide();
    }
    UltrasonicManager::removeInstrument(instrument);
}
