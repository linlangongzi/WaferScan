#include "InstrumentSetup.h"

Q_DECL_UNUSED static const bool encoderRegistered = EncoderSetup::registerType();
Q_DECL_UNUSED static const bool infoRegistered = qRegisterMetaType<InstrumentInfo>();
Q_DECL_UNUSED static const bool statusRegistered = qRegisterMetaType<InstrumentStatus>();

bool EncoderSetup::registerType()
{
    registerJsonConverter<EncoderSetup>();

    MetaType meta(qMetaTypeId<EncoderSetup>());
    meta.setTr("type", tr("类型"));
    meta.setTr("reverse", tr("反向"));
    meta.setTr("divider", tr("单位脉冲数"));
    meta.setTr("offset", tr("偏移"));
    meta.setTr("repeatCycle", tr("回转周期"));
    meta.setTr("material", tr("回转类型"));
    meta.setTr("axis", tr("对应机械轴"));
    meta.setTr("pulserEnable", tr("启用发射"));

    MetaType::setTr(None, tr("禁用"));
    MetaType::setTr(Quadrature, tr("相差脉冲"));
    MetaType::setTr(StepDirection, tr("脉冲/方向"));

    MetaType::setTr(NoRepeat, tr("禁用"));
    MetaType::setTr(RepeatZero, tr("回到零"));
    MetaType::setTr(Repeat, tr("回镜像位置"));

    MetaType::registerMeta(meta);

    return true;
}

QString InstrumentStatus::text() const
{
    switch (state) {
    case Connected:
        return tr("已连接");
    case Initializing:
        return tr("正在连接");
    case Disconnected:
    default :
        return tr("未连接");
    }
}
