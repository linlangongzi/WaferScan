#include "WaveformManager.h"
#include <Waveform/PanelAScan.h>
#include <Core/ICore.h>
#include <Core/MainWindow.h>
#include <Waveform/WaveFormConstants.h>

static WaveformManager *m_instance = nullptr;

WaveformManager::WaveformManager(QObject *parent) : QObject(parent)
{
    m_instance = this;
}

WaveformManager::~WaveformManager() = default;

WaveformManager *WaveformManager::instance()
{
    return m_instance;
}

QList<PanelAScan *> WaveformManager::waveformPanels()
{
    return m_instance->ascanPanels;
}

PanelAScan *WaveformManager::waveformPanel(int index)
{
    return m_instance->ascanPanels.value(index);
}

PanelAScan *WaveformManager::createWaveformPanel()
{
    auto *ascan = new PanelAScan;
    m_instance->ascanPanels.append(ascan);
    return ascan;
}

void WaveformManager::removeWaveformPanel(PanelAScan *ascan)
{
    if (m_instance->ascanPanels.contains(ascan)) {
        m_instance->ascanPanels.removeAll(ascan);
        ascan->deleteLater();
    }
}

void WaveformManager::loadSettings()
{
    int panelCount = 1;
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(Waveform::G_WAVEFORM);
    if (settings->contains(Waveform::K_ASCAN_COUNT)) {
        panelCount = settings->value(Waveform::K_ASCAN_COUNT).toInt();
    }
    settings->endGroup();
    auto *window = Core::ICore::mainWindow();
    for (int index = 1; index <= panelCount; ++index) {
        const QString objectName = tr("AScan%1").arg(index);
        window->AddPanelToDockWidget(createWaveformPanel(), Qt::TopDockWidgetArea, tr("A扫描%1").arg(index), objectName, index == 1, true);
    }
}

void WaveformManager::saveSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(Waveform::G_WAVEFORM);
    settings->setValue(Waveform::K_ASCAN_COUNT, waveformPanels().count());
    settings->endGroup();
}

void WaveformManager::setSetupLock(bool locked)
{
    for (PanelAScan *panel: waveformPanels()) {
        panel->setSetupLocked(locked);
    }
}
