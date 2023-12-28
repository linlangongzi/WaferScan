#pragma once

#include "ScanPathRectangle.h"
#include <allrising/ConnectionHelper.h>
#include <QWidget>

namespace Ui {
class ScanPathRectangleEdit;
}

class ScanPathRectangleEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ScanPathRectangleEdit(QWidget *parent = nullptr);
    ~ScanPathRectangleEdit() override;

    void setOptions(const ScanPathRectangleOptions &options);
    ScanPathRectangleOptions options() const;

private slots:
    void on_waveformSavePart_toggled(bool checked);
    void on_featureSavePart_toggled(bool checked);
    void on_setupEdit_clicked();
    void on_setupCopyFrom_clicked();
    void on_setupCopyTo_clicked();
    void on_setupEnable_toggled(bool checked);
    void on_single_toggled(bool checked);
    void on_scanLink_toggled(bool checked);
    void on_parallelList_toggled(bool checked);
    void on_parallel_toggled(bool checked);

private:
    void onLicenseUpdated();
    void resetSpeedBoxRange();
    UTSetup setup() const;
    void addChannel(int channelId);
    int currentChannelId() const;
    void initHandler();
    void updateLinearSpeedValue();

private:
    Ui::ScanPathRectangleEdit *ui;
    ConnectionHelper<ScanPathRectangleEdit, ScanPathRectangleOptions> m_handler;
    ScanPathRectangleOptions m_options;
};
