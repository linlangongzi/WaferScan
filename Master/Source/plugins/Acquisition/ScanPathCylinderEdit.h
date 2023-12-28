#pragma once

#include "ScanPathCylinder.h"
#include <QWidget>

namespace Ui {
class ScanPathCylinderEdit;
}

class ScanPathCylinderEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ScanPathCylinderEdit(QWidget *parent = nullptr);
    ~ScanPathCylinderEdit() override;

    void setOptions(const ScanPathCylinderOptions &options);
    ScanPathCylinderOptions options() const;
    QList<int> channelsInListWidget() const;

private slots:
    void on_scanAxis_currentIndexChanged(int index);

    void on_setup_toggled(bool checked);
    void on_multiChannel_toggled(bool checked);
    void on_addChannel_clicked();
    void on_removeChannel_clicked();
    void on_editSetup_clicked();
    void onLicenseUpdated();

private:
    void addChannel(int channelId);
    UTSetup setup() const;
    void refreshControls();

private:
    Ui::ScanPathCylinderEdit *ui;
    UTSetup m_setup;
};
