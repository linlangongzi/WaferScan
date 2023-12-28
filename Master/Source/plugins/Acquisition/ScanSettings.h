#pragma once

#include "ScanTaskCustomField.h"
#include <QPointer>
#include <QWidget>
#include <Core/IOptionsPage.h>

namespace Ui {
class ScanSettings;
}

class ScanSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ScanSettings(QWidget *parent = nullptr);
    ~ScanSettings() override;

    void apply();

    //! 编辑字段增加行，删除行功能
    void on_addField_clicked();
    void on_deleteField_clicked();
    void on_dataLevel_currentIndexChanged(int index);
    void on_editTemplatePath_clicked();

private:
    Ui::ScanSettings *ui;
    ScanTaskCustomFieldModel *m_customFieldModel = nullptr;
};

class ScanSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    explicit ScanSettingsPage(QObject *parent = nullptr);

    QWidget *widget() override;
    void apply() override;
    void finish() override;

private:
    QPointer<ScanSettings> m_widget;
};
