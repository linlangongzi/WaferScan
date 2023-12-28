#include "DataManagerBase.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <QFile>
#include <QMessageBox>
#include <Ultrasonic/UTData.h>
#include <Ultrasonic/UTPose.h>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>

DataManagerBase::DataManagerBase(QObject *parent) :
    QObject(parent)
{
}

void DataManagerBase::Connect()
{
    for (UTChannel *channel: UltrasonicManager::channels()) {
        connect(channel, &UTChannel::dataArrived, this, &DataManagerBase::dataArrived);
    }
}

void DataManagerBase::Disconnect()
{
    for (UTChannel *channel: UltrasonicManager::channels()) {
        disconnect(channel, nullptr, this, nullptr);
    }
}

void DataManagerBase::SetProbeBoxStatus(int status)
{
    if (status == probeBoxStatus) {
        return;
    }

    probeBoxStatus = status;

    probeBoxIndexVector.clear();
    for (int i = 0; i < 5; ++i) {
        if ((status >> i) & 0x1) {
            probeBoxIndexVector.push_back(i);
        }
    }
}

QVector<QVector<float> > DataManagerBase::GetChannelOffsetList()
{
    QVector<QVector<float> > offset;
    for (const PipeChannelConfig &channel: config.channels) {
        offset << QVector<float>(1, channel.baseOffset);
    }
    return offset;
}

QList<qreal> DataManagerBase::channelOffset(int featureIndex)
{
    int gateIndex = featureIndex / 2;
    QList<qreal> offset;
    for (auto &channel: config.channels) {
        offset << channel.baseOffset + channel.gateOffset.value(gateIndex);
    }
    return offset;
}

int DataManagerBase::GetProbeBoxStatus() const
{
    return probeBoxStatus;
}

void DataManagerBase::dataArrived(const UTDataSegment &segment)
{
    if (startFlag) {
        emit SegmentArrived(segment);
        document()->OnDataArrived(segment);
    }
}

void DataManagerBase::OnStartFlagArrived(quint16 flag)
{
    if (startFlag != flag) {
        startFlag = flag;
    }
}

void DataManagerBase::setConfig(const PipeGlobalConfig &globalConfig)
{
    config = globalConfig;
}
