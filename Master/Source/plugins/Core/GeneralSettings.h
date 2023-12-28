#pragma once

#include "IOptionsPage.h"
#include <QPointer>
#include <QWidget>

namespace Ui {
class GeneralSettings;
}

class GeneralSettings : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralSettings(QWidget *parent = 0);
    ~GeneralSettings();

    void apply();
    void refreshStatus();

private slots:
    void on_docPathBrowse_clicked();
    void on_docPath_textChanged(const QString &arg1);

private:
    Ui::GeneralSettings *ui;
};

class GeneralSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    GeneralSettingsPage(QObject *parent = 0);
    QWidget *widget();
    void apply();
    void finish();

private:
    QPointer<GeneralSettings> m_widget;
};
