#include "WheelDataset.h"
#include <Core/ICore.h>
#include "PanelProbeBScan/PanelProbeBScan.h"
#include "ProbeNoManager/ProbeNoManager.h"
#include "UTInstrumentDriver/PXI100/DataDef.h"
#include <allrising/ThrottleUpdate.h>
#include <arsCore/Platform.h>
#include <QDir>
#include <QLoggingCategory>
#include <QMessageBox>
#include <Ultrasonic/UTInstrument.h>
#include <Ultrasonic/UTSetup.h>
#include <Wheel/ControlPanel.h>

using namespace NS_PXI100;

int MovingAverage::size() const
{
    return window.size();
}

void MovingAverage::add(qreal value)
{
    if (qIsFinite(value)) {
        sum += value;
        count++;
    }

    window.append(value);
}

qreal MovingAverage::append(qreal value)
{
    add(value);
    return sum / qMax(1, count);
}

qreal MovingAverage::next(qreal value)
{
    if (window.size() == 1) {
        sum = value;
        qSwap(value, window.first());
        return value;
    }

    add(value);
    const qreal average = sum / count;
    const qreal dequeue = window.takeFirst();

    if (qIsFinite(dequeue)) {
        sum -= dequeue;
        count--;
    }

    return average;
}

static bool frameAscendingByTimestamp(const UTSharedDataFrame &first, const UTSharedDataFrame &second)
{
    return ControlPanel::timestamp(first) < ControlPanel::timestamp(second);
}

WheelDataset::WheelDataset(QObject *parent) :
    IDocumentPart(parent)
{
}

/*! 清空数据 */
bool WheelDataset::clearData()
{
    QSet<qint64> channels = QSet<qint64>::fromList(data.keys());
    data.clear();
    wheelSetMax = 0;

    emit judgeReady();
    emit wheelSetMaxChanged(wheelSetMax);
    emit channelDataUpdated(channels);

    return true;
}

void WheelDataset::add(UTSharedDataFrame frame)
{
    if (!ControlPanel::isValidFrame(frame)) {
        return;
    }

    setWheelSetMax(ControlPanel::wheelSetId(frame));
    qint64 id = ControlPanel::makeChannelId(ControlPanel::wheelId(frame), ControlPanel::channelId(frame));
    if (data.contains(id)) {
        data[id].data.append(frame);
    } else {
        ChannelData channelData;
        channelData.data.append(frame);
        data.insert(id, channelData);
    }

    dirtyChannels.insert(id);
    ThrottleUpdate::call(this, &WheelDataset::calculateExtremeFrame);
}

UTSharedDataFrame WheelDataset::extremeFrame(qint64 channelId)
{
    if (data.contains(channelId)) {
        return data[channelId].extreme;
    }

    return UTSharedDataFrame::create();
}

UTSharedDataSegment WheelDataset::wheelFrames(int wheelId)
{
    UTSharedDataSegment segment;
    for (auto it = data.cbegin(); it != data.cend(); ++it) {
        if (ControlPanel::channelIdToWheel(it.key()) == wheelId) {
            segment << it.value().extreme;
        }
    }
    return segment;
}

UTSharedDataSegment WheelDataset::channelFrames(qint64 channelId)
{
    return data.value(channelId).data;
}

void WheelDataset::modifyData(qint64 channelId)
{
    dirtyChannels.insert(channelId);
    ThrottleUpdate::call(this, &WheelDataset::calculateExtremeFrame);
//    saveDataModified();

    emit judgeReady();
}

UTSharedDataSegment WheelDataset::calculateDefects()
{
    UTSharedDataSegment segment;
    const auto control = controlPanel();
    if (!control->getGlobalConfig().judgeOpen) {
        return segment;
    }

    calculateExtremeFrame();

    const auto configs = control->getChannelConfigs();
    for (auto &it: data) {
        UTSharedDataFrame extreme = it.extreme;
        if (extreme.isNull()) {
            continue;
        }

        int channel = ControlPanel::channelId(extreme);
        ChannelConfig config = configs.value(channel);

        for (auto filter: config.judgments) {
            qreal d = extreme->CData.value(filter.featureType);
            if (d >= filter.min && d <= filter.max) {
                segment << extreme;
            }
        }
    }

    return segment;
}

/*!
 * \brief WheelDataset::calculateMaxFrame
 * 重算各通道最大帧frame
 */
void WheelDataset::calculateExtremeFrame()
{
    QSet<qint64> changed;
    const auto configs = controlPanel()->getChannelConfigs();
    for (auto channelId: dirtyChannels) {
        if (!data.contains(channelId)) {
            continue;
        }

        ChannelData &d = data[channelId];

        d.location = -1;
        d.extreme.clear();
        int channel = ControlPanel::channelIdToChannel(channelId);
        ChannelConfig config = configs.value(channel);
        int start = qMin(qMax(0.0, config.removeFront), 1.0) * d.data.size();
        int end = d.data.size() * (1.0 - qMin(qMax(0.0, config.removeBack), 1.0));
        int location = 0;
        qreal gateITimeMin = Q_SNAN;

        /* 查找I闸门触发时间最小的位置 */
        for (int index = start; index < end; ++index) {
            if (!ControlPanel::isValidFrame(d.data.value(index))) {
                continue;
            }

            UTSharedDataFrame frame = d.data.value(index);
            qreal gateITime = frame->CData.value(ControlPanel::GATE_I1_TIME);
            if (qIsNaN(gateITimeMin) || (gateITimeMin > gateITime)) {
                gateITimeMin = gateITime;
                location = index;
            }
        }

        d.location = location;

        int rangeStart = qBound(start, location - config.frameNumber, end);
        int rangeEnd = qBound(start, location + config.frameNumber, end);

        UTSharedDataSegment segment;
        QVector<MovingAverage> windows;
        windows.resize(NUM_FEATURES_ADATA);

        /* 过滤算法 */
        int removeCount = 0;
        int lastSensor = -1;
        for (int index = rangeStart; index <= rangeEnd; ++index) {
            if (!ControlPanel::isValidFrame(d.data.value(index))) {
                continue;
            }

            UTSharedDataFrame frame = UTSharedDataFrame::create();
            *frame.data() = *d.data.value(index).data();

            /* 传感器切换时的过滤 */
            int newSensor = qRound(frame->CData.value(ControlPanel::SENSOR_NO));
            if (newSensor != lastSensor) {
                lastSensor = newSensor;
                removeCount = config.removeAfterSwitch;
            }

            if (removeCount > 0) {
                removeCount--;
                continue;
            }

            /* 特征值的过滤 */
            bool invalid = false;
            for (auto filter: config.featureFilters) {
                qreal feature = frame->CData.value(filter.featureType);
                if (feature >= filter.min && feature <= filter.max) {
                    invalid = true;
                    break;
                }
            }

            if (invalid) {
                continue;
            }

            /* 移动平均 */
            if (config.averageSize <= 1) {
                segment.append(frame);
                continue;
            }

            for (int wIndex = 0; wIndex < NUM_FEATURES_ADATA; ++wIndex) {
                if (windows[wIndex].size() < config.averageSize) {
                    windows[wIndex].add(frame->CData.value(wIndex));
                } else {
                    frame->CData[wIndex] = windows[wIndex].next(frame->CData.value(wIndex));
                }
            }

            segment.append(frame);
        }

        FeatureFilterConfig::FeatureType featureType = config.featureType;
        ChannelConfig::MergeType mergeType = config.merge;
        for (auto frame: segment) {
            if (d.extreme.isNull() ||
               ((d.extreme->CData.value(featureType) > frame->CData.value(featureType)) && mergeType == ChannelConfig::MergeType::Min) ||
               ((d.extreme->CData.value(featureType) < frame->CData.value(featureType)) && mergeType == ChannelConfig::MergeType::Max)) {
                d.extreme = frame;
                changed << channelId;
            }
        }
    }

    emit channelDataUpdated(changed);
    dirtyChannels.clear();
}

void WheelDataset::setWheelSetMax(int countMax)
{
    if (countMax <= wheelSetMax) {
        return;
    }

    wheelSetMax = countMax;
    emit wheelSetMaxChanged(wheelSetMax);
}
