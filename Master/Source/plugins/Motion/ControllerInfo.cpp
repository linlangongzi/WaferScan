#include "ControllerInfo.h"

using namespace Motion;

REGISTER_JSON_CONVERTER(ControllerIoConfig);

QString ControllerStatus::text() const
{
    switch (state) {
    case Disconnected:  return tr("未连接");
    case Connecting:    return tr("正在连接");
    case SwitchChannel: return tr("正在切换通讯模式");
    case RequestInfo:   return tr("正在获取控制器信息");
    case ConfigureAxis: return tr("正在配置轴参数");
    case SwitchingMode: return tr("正在切换模式");
    case Normal:        return tr("已连接");
    default:            return QString();
    }
}

QString ControllerInfo::featureEnabledText() const
{
    QStringList features;
    if (featureEnabled & IecRuntime) {
        features << tr("IEC");
    }
    if (featureEnabled & KinematicRuntime) {
        features << tr("机器人");
    }
    return features.join(", ");
}
