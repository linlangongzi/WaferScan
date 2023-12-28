#pragma once

#include <QObject>
#include "MotionGlobal.h"
#include <allrising/MetaType/MetaType.h>

class MOTION_EXPORT SafetyBarrierOptions
{
    Q_GADGET

    Q_PROPERTY(bool disabled MEMBER disabled)
    Q_PROPERTY(QRectF rect MEMBER rect)
    Q_PROPERTY(qreal minDepth MEMBER minDepth)
    Q_PROPERTY(qreal maxDepth MEMBER maxDepth)

public:
    DECLARE_COMPARE_OPERATORS(SafetyBarrierOptions)

    static SafetyBarrierOptions fromJson(const QJsonValue &json);

    bool disabled = false;
    QRectF rect;
    qreal minDepth = 0;
    qreal maxDepth = 0;
};
