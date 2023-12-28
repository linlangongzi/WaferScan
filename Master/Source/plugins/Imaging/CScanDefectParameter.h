#pragma once

#include <QDialog>
#include <QJsonObject>
#include <Imaging/UTDefectProcessorOptions.h>

namespace Ui {
class CScanDefectParameter;
}

class CScanDefectParameter : public QDialog
{
    Q_OBJECT

public:
    explicit CScanDefectParameter(int channelIndex, QWidget *parent = 0);
    ~CScanDefectParameter();

public:
    void SetInfo(const UTDefectProcessorOptions &options);
    UTDefectProcessorOptions GetOptions();

private:
    void InitUI();

private:
    Ui::CScanDefectParameter *ui;
    int sourceChannel = 0;
};
