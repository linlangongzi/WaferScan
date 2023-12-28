#include "WheelConfig.h"
#include <QRegularExpression>

FeatureFilterConfig FeatureFilterConfig::fromString(const QString &string)
{
    static const QRegularExpression regex("(\\d+)\\(([^-]+)-([^-]+)\\)");
    auto match = regex.match(string);
    if (!match.hasMatch()) {
        return FeatureFilterConfig();
    }

    FeatureFilterConfig c;
    c.featureType = FeatureType(match.captured(1).toInt());
    c.min = match.captured(2).toDouble();
    c.max = match.captured(3).toDouble();

    return c;
}
