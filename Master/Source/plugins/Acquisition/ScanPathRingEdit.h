#pragma once

#include <Motion/ChannelList.h>
#include <Motion/MotionElement.h>
#include <QWidget>
#include <allrising/MetaType/MetaType.h>
#include "ScanPathRing.h"

namespace Ui {
class ScanPathRingEdit;
}

class ScanPathRingEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ScanPathRingEdit(QWidget *parent = 0);
    ~ScanPathRingEdit();

    void setOptions(const ScanPathRingOptions &options);
    ScanPathRingOptions options() const;

private slots:
    void on_addChannel_annulus_clicked();
    void on_removeChannel_annulus_clicked();
    void on_centerType_currentIndexChanged(int index);
    void refreshCenter();

private:
    void resizeChannelItems();

private:
    Ui::ScanPathRingEdit *ui;
};
