#include "PipeGlobalConfig.h"
#include <QtMath>

REGISTER_JSON_CONVERTER(PipeChannelConfig);
REGISTER_JSON_CONVERTER(PipeGlobalConfig);
REGISTER_JSON_CONVERTER(PipeModeConfig);

Q_DECL_UNUSED static const bool modeName = []() {
    QMetaType::registerConverter(&PipeModeConfig::name);
    MetaType meta(qMetaTypeId<PipeModeConfig>());
    MetaType::registerMeta(meta);
    return true;
}();

QString PipeChannelConfig::channelName() const
{
    return tr("通道%1 %2盒 %3").arg(index).arg(probeBoxIndex + 1).arg(name);
}

QString PipeChannelConfig::channelDescription() const
{
    return tr("通道序号(0~33): %1\n"
              "硬通道号(0~11): %2\n"
              "软通道号(0~3):  %3\n"
              "前置卡号(0~5):  %4\n"
              "输入序号(1~8):  %5\n"
              "输出序号(1~2):  %6")
              .arg(index)
              .arg(hardChannel)
              .arg(softChannel)
              .arg(muxCardId)
              .arg(muxInput)
              .arg(muxOutput);
}

QList<qreal> PipeGlobalConfig::plotOffset(int featureIndex) const
{
    int gateIndex = featureIndex / 2;
    QList<qreal> offset;
    for (auto &channel: channels) {
        offset << -(channel.baseOffset + channel.gateOffset.value(gateIndex));
    }
    return offset;
}

QList<PlotView> PipeGlobalConfig::plots(int mode, int groupMask, const PipeDetect::OrderInfo &info) const
{
    QList<PlotView> views = modes.value(mode).plots;
    MotionAxisList enabled = enabledChannels(groupMask);

    for (PlotView &view: views) {
        view.range.setLeft(0);
        view.range.setRight(info.materialLength);

        /* 搜索注释含有“thickness”的视图，将其按照测厚视图刷新垂直成像范围 */
        bool isThicknessView = false;
        for (auto &line: view.lines) {
            if (line.comment == "thicknessMax") {
                line.value = info.thickMax;
            } else if (line.comment == "thicknessMin") {
                line.value = info.thickMin;
            } else if (line.comment == "thickness") {
                line.value = info.materialThick;
            }
            isThicknessView |= line.comment.contains("thickness");
        }

        for (PlotCurve &curve: view.curves) {
            /* 禁用被关闭的通道 */
            curve.channels.intersect(enabled);
            curve.interval = intervalPlot;
            if (autoChannelOffset) {
                curve.offset = plotOffset(curve.featureIndex);
            }
            if (autoAngleCorrect) {
                curve.angleCorrect = angleCorrect();
            }
        }
        if (autoDeadZone) {
            view.deadZoneLeft = deadZoneLeft;
            view.deadZoneRight = deadZoneRight;
        }
        if (isThicknessView) {
            view.range.setBottom(info.thickShowMax);
            view.range.setTop(info.thickShowMin);
        }
    }

    std::remove_if(views.begin(), views.end(), [](const PlotView &view) {
        return !view.hasValidCurve();
    });

    return views;
}

QVector<QVector<float> > PipeGlobalConfig::imageOffset() const
{
    QVector<QVector<float> > offset;
    for (const auto &channel: channels) {
        offset << QVector<float>(1, -channel.baseOffset);
    }
    return offset;
}

QList<ImageOptions> PipeGlobalConfig::imageOptions(int modeId, int groupMask, PipeDetect::OrderInfo orderInfo) const
{
    const qreal circumference = orderInfo.materialDiameter * M_PI;
    qreal width = orderInfo.materialLength - deadZoneLeft - deadZoneRight;
    QRectF imageRect(deadZoneLeft, 0, width, circumference);

    const PipeModeConfig &mode = modes.value(modeId);
    const MotionAxisList enabled = enabledChannels(groupMask);

    QList<ImageOptions> options;
    for (ImageOptions image : mode.images) {
        MotionAxisList channels = image.channels();
        channels.intersect(enabled);

        image.setAxisX(0);
        image.setAxisY(1);
        image.setTransform(imageRect, QSizeF(intervalX, intervalR));
        image.setChannelOffset(imageOffset());
        image.setChannels(channels);

        options << image;
    }

    return options;
}

MotionAxisList PipeGlobalConfig::enabledChannels(int groupMask) const
{
    MotionAxisList enabled;
    for (int index = 0; index < channels.count(); ++index) {
        bool valid = (1 << channels[index].probeBoxIndex) & groupMask;

        enabled.setAxis(index, valid && channels[index].enable);
    }
    return enabled;
}

QList<qreal> PipeGlobalConfig::angleCorrect() const
{
    QList<qreal> correct;
    for (auto &channel: channels) {
        correct << channel.angleCorrect;
    }
    return correct;
}

PipeModeConfig PipeModeConfig::fromJson(const QJsonValue &json)
{
    PipeModeConfig config;
    JsonToGadget(json, PipeModeConfig::staticMetaObject, &config);
    const QJsonObject obj = json.toObject();

    if (obj.contains("imageChannels")) {
        QList<MotionAxisList> imageChannels = convertFromJson<QList<MotionAxisList>>(obj["imageChannels"]);
        config.images.clear();
        for (const auto &imageChannel: imageChannels) {
            ImageOptions transform;
            transform.setChannels(imageChannel);
            config.images << transform;
        }
    }

    return config;
}
