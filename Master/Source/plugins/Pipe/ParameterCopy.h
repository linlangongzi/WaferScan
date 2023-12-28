#pragma once

#include <QDialog>
#include "Ars1000/PipeDataDef.h"

class UTSetup;

namespace Ui {
class ParameterCopy;
}

class ParameterCopy : public QDialog
{
    Q_OBJECT

public:
    typedef QList<QString> UTSetupKeysList;
    typedef PipeDetect::ProcessInfo ProcessInfo;

public:
    explicit ParameterCopy(QWidget *parent = 0);
    ~ParameterCopy();

signals:
    void ProcessInfoChanged(const ProcessInfo &info);

private slots:
    void Yes();
    void Copy();
    void LoadSetup();
    void SaveSetup();

private:
    void initConnections();

private:
    Ui::ParameterCopy *ui;
    ProcessInfo processInfo;
};
