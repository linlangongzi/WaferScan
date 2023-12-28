#pragma once

#include <QPointer>
#include <QWidget>
#include <Core/IOptionsPage.h>

namespace Ui {
class ScanPathRingSettings;
}

class ScanPathRingSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ScanPathRingSettings(QWidget *parent = nullptr);
    ~ScanPathRingSettings() override;

    void apply();

private:
    Ui::ScanPathRingSettings *ui;
};

class ScanPathRingSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    explicit ScanPathRingSettingsPage(QObject *parent = nullptr);

    QWidget *widget() override;
    void apply() override;
    void finish() override;

private:
    QPointer<ScanPathRingSettings> m_widget;
};
