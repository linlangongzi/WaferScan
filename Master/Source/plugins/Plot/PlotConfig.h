#pragma once

#include "PlotGlobal.h"
#include <allrising/Util.h>
#include <Motion/MotionAxisList.h>
#include <QColor>
#include <QJsonValue>
#include <QObject>

class PLOT_EXPORT PlotFeature
{
    Q_GADGET

    Q_PROPERTY(int   channel MEMBER channel)
    Q_PROPERTY(qreal feature MEMBER feature)
    Q_PROPERTY(qreal pos     MEMBER pos)

public:
    DECLARE_COMPARE_OPERATORS(PlotFeature)

    int channel = -1;
    qreal feature = Q_SNAN;
    int pos = 0;
};

class PLOT_EXPORT PlotCurve
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(bool enable MEMBER enable)
    Q_PROPERTY(QColor color MEMBER color)
    Q_PROPERTY(int lineWidth MEMBER lineWidth)
    Q_PROPERTY(MotionAxisList channels MEMBER channels)
    Q_PROPERTY(int axis MEMBER axis)
    Q_PROPERTY(qreal interval MEMBER interval)
    Q_PROPERTY(QList<qreal> offset MEMBER offset)
    Q_PROPERTY(int featureIndex MEMBER featureIndex)
    Q_PROPERTY(int featureBaseIndex MEMBER featureBaseIndex)
    Q_PROPERTY(int average MEMBER average)
    Q_PROPERTY(qreal scale MEMBER scale)
    Q_PROPERTY(qreal correct MEMBER correct)
    Q_PROPERTY(ReducePolicy reduce MEMBER reduce)
    Q_PROPERTY(QString comment MEMBER comment)

public:
    enum ReducePolicy {
        ReduceNone,
        ReduceByMax,
        ReduceByMin,
        ReduceByAverage,
    };
    Q_ENUM(ReducePolicy)

    bool isValid() const;

    DECLARE_COMPARE_OPERATORS(PlotCurve)

    QString name;
    bool enable = true;
    QColor color;
    int lineWidth = 0;
    MotionAxisList channels;
    qreal interval = 1;
    QList<qreal> offset;
    QList<qreal> angleCorrect;
    int axis = -1;
    int featureIndex = -1;
    int featureBaseIndex = -1;
    int average = 1;
    qreal scale = 1;
    qreal correct = 0;
    ReducePolicy reduce = ReduceNone;
    QString comment;
};

class PLOT_EXPORT PlotLine
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)

    Q_PROPERTY(Qt::Orientation orientation MEMBER orientation)
    Q_PROPERTY(qreal value MEMBER value)
    Q_PROPERTY(QColor color MEMBER color)
    Q_PROPERTY(AlarmMode alarmMode MEMBER alarmMode)
    Q_PROPERTY(QString comment MEMBER comment)

public:
    enum AlarmMode {
        AlarmNone,
        AlarmLow,
        AlarmHigh,
    };
    Q_ENUM(AlarmMode)

    bool shouldAlarm(qreal value) const;

    DECLARE_COMPARE_OPERATORS(PlotLine)

    QString name;
    qreal value = 0;
    QColor color = Qt::gray;
    Qt::Orientation orientation = Qt::Horizontal;
    AlarmMode alarmMode = AlarmNone;
    QString comment;
};

class PLOT_EXPORT PlotView
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QRectF range MEMBER range)
    Q_PROPERTY(QString unitNameH MEMBER unitNameH)
    Q_PROPERTY(QString unitNameV MEMBER unitNameV)
    Q_PROPERTY(qreal deadZoneLeft MEMBER deadZoneLeft)
    Q_PROPERTY(qreal deadZoneRight MEMBER deadZoneRight)
    Q_PROPERTY(QList<PlotCurve> curves MEMBER curves)
    Q_PROPERTY(QList<PlotLine> lines MEMBER lines)

public:
    DECLARE_COMPARE_OPERATORS(PlotView)

    bool hasValidCurve() const;
    bool isDeadZone(qreal position) const;

    QString name;
    QRectF range;
    QString unitNameH;
    QString unitNameV;
    qreal deadZoneLeft;
    qreal deadZoneRight;
    QList<PlotCurve> curves;
    QList<PlotLine> lines;
};

class PLOT_EXPORT PlotAlarmIndicator
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(int channel MEMBER channel)

public:
    DECLARE_COMPARE_OPERATORS(PlotAlarmIndicator)

    QString name;
    int channel = -1;
};

class PLOT_EXPORT PlotAlarmGroup
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QList<PlotAlarmIndicator> indicators MEMBER indicators)
    Q_PROPERTY(QColor inactiveColor MEMBER inactiveColor)
    Q_PROPERTY(QColor activeColor MEMBER activeColor)
    Q_PROPERTY(int featureIndex MEMBER featureIndex)

public:
    DECLARE_COMPARE_OPERATORS(PlotAlarmGroup)

    QString name;
    QList<PlotAlarmIndicator> indicators;
    QColor inactiveColor = Qt::green;
    QColor activeColor = Qt::red;
    int featureIndex = -1;
};

class PLOT_EXPORT PlotAlarmItem
{
    Q_GADGET

public:
    MotionAxisList channel;
    int featureIndex = -1;
    QString reason;
};
