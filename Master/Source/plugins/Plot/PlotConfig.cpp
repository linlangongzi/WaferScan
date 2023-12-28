#include "PlotConfig.h"
#include <allrising/Util.h>

REGISTER_JSON_CONVERTER(PlotFeature);
REGISTER_JSON_CONVERTER(PlotLine);
REGISTER_JSON_CONVERTER(PlotAlarmIndicator);
REGISTER_JSON_CONVERTER(PlotAlarmGroup);
REGISTER_JSON_CONVERTER(PlotCurve);
REGISTER_JSON_CONVERTER(PlotView);

bool PlotCurve::isValid() const
{
    for (int index = 0; index < channels.count(); ++index) {
        if (channels.hasAxis(index)) {
            return true;
        }
    }
    return false;
}

bool PlotLine::shouldAlarm(qreal v) const
{
    if (alarmMode == AlarmHigh) {
        return v >= value;
    } else if (alarmMode == AlarmLow) {
        return v <= value;
    }

    return false;
}

/*!
 * \brief PlotView::isDeadZone
 * \param position:X方向编码器值
 * \return 是否在成像盲区内，盲区内数据不进行报警处理
 */
bool PlotView::hasValidCurve() const
{
    for (const auto &curve: curves) {
        if (curve.isValid()) {
            return true;
        }
    }
    return false;
}

bool PlotView::isDeadZone(qreal position) const
{
    return (position < range.left() + deadZoneLeft) || (position > range.right() - deadZoneRight);
}
