#pragma once

#include "UltrasonicGlobal.h"
#include <allrising/MetaType/MetaType.h>
#include <QObject>
#include <QColor>
#include <QCoreApplication>

class ULTRASONIC_EXPORT UTSetupGate
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(UTSetupGate)

    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(qreal start READ start WRITE setStart)
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(qreal threshold READ threshold WRITE setThreshold)
    Q_PROPERTY(Mode timeMode READ timeMode WRITE setTimeMode)
    Q_PROPERTY(Mode ampMode READ ampMode WRITE setAmpMode)
    Q_PROPERTY(int syncIndex READ syncIndex WRITE setSyncIndex)

public:
    enum Mode {
        Trigger,
        MaxPeak,
        FirstPeak,
    };
    Q_ENUM(Mode)
    enum {ModeCount = FirstPeak + 1};

    DECLARE_COMPARE_OPERATORS(UTSetupGate)

    qreal end() const {return m_start + m_width;}
    bool hasSync() const {return m_syncIndex >= 0;}

    bool isEnabled() const;
    int index() const;
    QString name() const;
    QColor color() const;
    qreal start() const;
    qreal width() const;
    qreal threshold() const;
    Mode timeMode() const;
    Mode ampMode() const;
    int syncIndex() const;

    void setEnabled(bool enabled);
    void setName(QString name);
    void setColor(QColor color);
    void setStart(qreal start);
    void setWidth(qreal width);
    void setThreshold(qreal threshold);
    void setTimeMode(Mode timeMode);
    void setAmpMode(Mode ampMode);
    void setSyncIndex(int syncIndex);

    static bool registerType();

private:
    QString m_name;
    bool m_enabled = false;
    QColor m_color;
    qreal m_start = 0;
    qreal m_width = 0;
    qreal m_threshold = 0;
    Mode m_timeMode = MaxPeak;
    Mode m_ampMode = MaxPeak;
    int m_syncIndex = -1;

    int m_index = -1;

    friend class UTSetup;
};
