#include "ScanPathRingSettings.h"
#include "ui_ScanPathRingSettings.h"
#include "AcquisitionConstants.h"
#include <Core/Platform.h>

ScanPathRingSettings::ScanPathRingSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanPathRingSettings)
{
    ui->setupUi(this);

    ui->datumPointX->setValue(platform()->Get("AnnulusDatumPointX").toDouble());
    ui->datumPointY->setValue(platform()->Get("AnnulusDatumPointY").toDouble());
    ui->directionX->setValue(platform()->Get("AnnulusDirectionX").toDouble());
    ui->directionY->setValue(platform()->Get("AnnulusDirectionY").toDouble());
    ui->pressingDistance->setValue(platform()->Get("AnnulusPressingDistance").toDouble());
    ui->probeBoxThickness->setValue(platform()->Get("AnnulusProbeBoxThickness").toDouble());
}

ScanPathRingSettings::~ScanPathRingSettings()
{
    delete ui;
}

void ScanPathRingSettings::apply()
{
    platform()->Set("AnnulusDatumPointX", ui->datumPointX->value());
    platform()->Set("AnnulusDatumPointY", ui->datumPointY->value());
    platform()->Set("AnnulusDirectionX", ui->directionX->value());
    platform()->Set("AnnulusDirectionY", ui->directionY->value());
    platform()->Set("AnnulusPressingDistance", ui->pressingDistance->value());
    platform()->Set("AnnulusProbeBoxThickness", ui->probeBoxThickness->value());
}

ScanPathRingSettingsPage::ScanPathRingSettingsPage(QObject *parent) :
    IOptionsPage(parent)
{
    using namespace Acquisition;
    setId(SETTINGS_ID_RING);
    setDisplayName(tr("圆环扫描"));
    setCategory(SETTINGS_CATEGORY_ACQUISITION);
    setDisplayCategory(tr("扫描"));
}

QWidget *ScanPathRingSettingsPage::widget()
{
    if (!m_widget) {
        m_widget = new ScanPathRingSettings;
    }
    return m_widget;
}

void ScanPathRingSettingsPage::apply()
{
    m_widget->apply();
}

void ScanPathRingSettingsPage::finish()
{
    delete m_widget;
}
