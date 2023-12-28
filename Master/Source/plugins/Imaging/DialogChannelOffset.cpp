#include "DialogChannelOffset.h"
#include "ui_DialogChannelOffset.h"
#include <Motion/Axis.h>
#include <QDoubleSpinBox>
#include <Motion/MotionManager.h>
#include <Ultrasonic/UltrasonicManager.h>

DialogChannelOffset::DialogChannelOffset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChannelOffset)
{
    ui->setupUi(this);
    initUi();
}

DialogChannelOffset::~DialogChannelOffset()
{
    delete ui;
}

void DialogChannelOffset::initUi()
{
    // 加载通道
    for (auto *channel: UltrasonicManager::channels()) {
        ui->channel->addItem(channel->name(), channel->channelId());
    }
    // 动态生成轴偏移
    for (Axis *axis: MotionManager::axisList()) {
        QLabel *title = new QLabel(axis->name(), this);
        auto *box = new QDoubleSpinBox(this);
        double reverseLimit = axis->config().limitReverse;
        double forwardLimit = axis->config().limitForward;
        box->setRange(reverseLimit, forwardLimit);
        box->setSuffix(axis->axisUnit());
        ui->axesLayout->addRow(title, box);
    }
}

QPair<int, QVector<float> > DialogChannelOffset::channelOffset()
{
    int channel = ui->channel->currentData().toInt();
    int axisCount = MotionManager::axisCount();
    QVector<float> offset;
    offset.resize(axisCount);
    for (QDoubleSpinBox *box: ui->AxesGroup->findChildren<QDoubleSpinBox *>()) {
        int row = -1;
        QFormLayout::ItemRole role = QFormLayout::FieldRole;
        ui->axesLayout->getWidgetPosition(box, &row, &role);
        if (row < 0 || row > axisCount) {
            continue;
        }
        offset.replace(row, box->value());
    }
    return qMakePair(channel, offset);
}

void DialogChannelOffset::setChannelOffset(const QPair<int, QVector<float> > &offset)
{
    ui->channel->setCurrentIndex(offset.first);
    ui->channel->setEnabled(false);
    for (QDoubleSpinBox *box: ui->AxesGroup->findChildren<QDoubleSpinBox *>()) {
        int row = -1;
        QFormLayout::ItemRole role = QFormLayout::FieldRole;
        ui->axesLayout->getWidgetPosition(box, &row, &role);
        if (row < 0) {
            continue;
        }
        box->setValue(offset.second.value(row));
    }
}
