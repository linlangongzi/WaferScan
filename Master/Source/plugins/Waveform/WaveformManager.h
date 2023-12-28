#pragma once

#include <QObject>
#include <Waveform/WaveformGlobal.h>

class PanelAScan;

class WAVEFORM_EXPORT WaveformManager : public QObject
{
    Q_OBJECT

public:
    explicit WaveformManager(QObject *parent = nullptr);
    ~WaveformManager() final;

    static WaveformManager *instance();

    static QList<PanelAScan *> waveformPanels();
    static PanelAScan *waveformPanel(int index);
    static PanelAScan *createWaveformPanel();
    static void removeWaveformPanel(PanelAScan * ascan);

public slots:
    void loadSettings();
    void saveSettings();
    static void setSetupLock(bool locked);

private:
    QList<PanelAScan *> ascanPanels;
};
