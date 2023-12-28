#include "WheelPosition.h"
#include <arsCore/Platform.h>
#include <Wheel/ControlPanel.h>

bool WheelPosition::isValid(int position) const
{
    const auto &config = controlPanel()->getGlobalConfig().wheelConfig;

    if (position > currentPosition - config.radius &&
        position < currentPosition + config.radius) {
        return true;
    }

    return false;
}

bool WheelPosition::add(int time, int position)
{
    if (hasFinished || !isValid(position)) {
        return false;
    }

    const auto &config = controlPanel()->getGlobalConfig().wheelConfig;

    if (window.size() < config.average) {
        currentPosition = window.append(position);
    } else {
        currentPosition = window.next(position);
    }

    positions[time] = currentPosition;
    addConfidence(config.rate);

    return true;
}

void WheelPosition::addConfidence(int num)
{
    const auto &config = controlPanel()->getGlobalConfig().wheelConfig;
    confidence = qBound(0, confidence + num, config.standard);

    if (confidence == config.standard) {
        isReal = true;
    } else if (confidence == 0) {
        hasFinished = true;
    }
}
