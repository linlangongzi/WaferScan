#include "UTSetupGate.h"
#include <allrising/Util.h>

static const bool initType = UTSetupGate::registerType();

bool UTSetupGate::isEnabled() const
{
    return m_enabled;
}

int UTSetupGate::index() const
{
    return m_index;
}

QString UTSetupGate::name() const
{
    if (m_name.isEmpty()) {
        return tr("闸门 %1").arg(m_index + 1);
    }
    return m_name;
}

qreal UTSetupGate::start() const
{
    return m_start;
}

qreal UTSetupGate::width() const
{
    return m_width;
}

qreal UTSetupGate::threshold() const
{
    return m_threshold;
}

UTSetupGate::Mode UTSetupGate::ampMode() const
{
    return m_ampMode;
}

UTSetupGate::Mode UTSetupGate::timeMode() const
{
    return m_timeMode;
}

int UTSetupGate::syncIndex() const
{
    return m_syncIndex;
}

QColor UTSetupGate::color() const
{
    return m_color;
}

void UTSetupGate::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void UTSetupGate::setName(QString name)
{
    m_name = name;
}

void UTSetupGate::setStart(qreal start)
{
    m_start = start;
}

void UTSetupGate::setWidth(qreal width)
{
    m_width = width;
}

void UTSetupGate::setThreshold(qreal threshold)
{
    m_threshold = threshold;
}

void UTSetupGate::setAmpMode(UTSetupGate::Mode ampMode)
{
    m_ampMode = ampMode;
}

void UTSetupGate::setTimeMode(UTSetupGate::Mode timeMode)
{
    m_timeMode = timeMode;
}

void UTSetupGate::setSyncIndex(int syncIndex)
{
    m_syncIndex = syncIndex;
}

void UTSetupGate::setColor(QColor color)
{
    m_color = color;
}

bool UTSetupGate::registerType()
{
    qRegisterMetaType<UTSetupGate>();
    qRegisterMetaType<UTSetupGate::Mode>();
    registerJsonConverter<UTSetupGate>();
    QMetaType::registerConverter(&UTSetupGate::name);

    MetaType meta(qMetaTypeId<UTSetupGate>());
    meta.setTr("enabled", tr("启用"));
    meta.setTr("name", tr("名称"));
    meta.setTr("color", tr("颜色"));
    meta.setTr("start", tr("前沿"));
    meta.setTr("width", tr("宽度"));
    meta.setTr("threshold", tr("阈值"));
    meta.setTr("timeMode", tr("时间捕获模式"));
    meta.setTr("ampMode", tr("幅值捕获模式"));
    meta.setTr("syncIndex", tr("同步"));

    MetaType::setTr(Trigger, tr("触发"));
    MetaType::setTr(MaxPeak, tr("波峰"));
    MetaType::setTr(FirstPeak, tr("首次波峰"));

    meta.setEditorProperty("start", "minimum", 0);
    meta.setEditorProperty("width", "minimum", 0);
    meta.setEditorProperty("threshold", "minimum", 0);
    meta.setEditorProperty("threshold", "maximum", 100);
    meta.setEditorProperty("threshold", "suffix", "%");
    meta.setEditorProperty("syncIndex", "minimum", -1);
    meta.setEditorProperty("syncIndex", "specialValueText", tr("禁用"));

    MetaType::registerMeta(meta);

    return true;
}
