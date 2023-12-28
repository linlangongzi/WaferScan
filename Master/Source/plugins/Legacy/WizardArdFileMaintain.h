#pragma once

#include <QWizard>
#include <QMap>
#include <QFileInfoList>
#include <Ultrasonic/UTData.h>

namespace Ui {
class WizardArdFileMaintain;
}

class WizardArdFileMaintain : public QWizard
{
    Q_OBJECT

public:
    explicit WizardArdFileMaintain(QWidget *parent = 0);
    ~WizardArdFileMaintain();

private:
    virtual bool validateCurrentPage();
    virtual int nextId() const;
    virtual void reject();

private slots:
    void OnLoopTask();
    void on_InputToolButton_clicked();
    void on_DelFile_clicked();
    void on_DefaultExportBtn_clicked();
    void on_ArdFilterAscanClear_toggled(bool checked);
    void on_ArdFilterAscanTrigger_toggled(bool checked);

private:
    Ui::WizardArdFileMaintain *ui;
    bool isStop = false;
};
