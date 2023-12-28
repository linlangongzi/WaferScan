#include "ScanPathAdvanced.h"
#include <utils/algorithm.h>
#include <QLoggingCategory>
#include <QWidget>
#include <extensionsystem/pluginmanager.h>

static Q_LOGGING_CATEGORY(debug, "acquire.factory");
static QList<ScanPathFactory *> m_factories;

using namespace ExtensionSystem;

ScanPathFactory::ScanPathFactory(QObject *parent) :
    QObject(parent)
{
    m_factories << this;
}

ScanPathFactory::~ScanPathFactory()
{
    m_factories.removeOne(this);
}

QString ScanPathFactory::name() const
{
    return metaObject()->className();
}

QString ScanPathFactory::displayName() const
{
    return name();
}

QIcon ScanPathFactory::icon() const
{
    return {};
}

MotionPath ScanPathFactory::motionPath(const QVariant &options) const
{
    Q_UNUSED(options);
    return {};
}

QHash<int, UTSetup> ScanPathFactory::setups(const QVariant &options) const
{
    Q_UNUSED(options);
    return {};
}

QList<ImageOptions> ScanPathFactory::imageOptions(const QVariant &options) const
{
    Q_UNUSED(options);
    return {};
}

QPair<qreal, qreal> ScanPathFactory::boundingRange(const QVariant &options, int axis) const
{
    Q_UNUSED(options);
    Q_UNUSED(axis);
    return {};
}

ScanPathFactory::CheckResult ScanPathFactory::checkOptions(const QVariant &options) const
{
    Q_UNUSED(options);
    return {};
}

//QList<RecordConfig> ScanPathFactory::recordOptions(const QVariant &options) const
//{
//    Q_UNUSED(options);
//    return QList<RecordConfig>();
//}

//SafetyBarrierOptions ScanPathFactory::safetyBarrier(const QVariant &options) const
//{
//    Q_UNUSED(options);
//    return SafetyBarrierOptions();
//}

QWidget *ScanPathFactory::widget(const QVariant &options) const
{
    Q_UNUSED(options);
    return nullptr;
}

QVariant ScanPathFactory::widgetOptions(QWidget *widget) const
{
    Q_UNUSED(widget);
    return {};
}

void ScanPathFactory::setWidgetEnabled(QWidget *widget, bool enable) const
{
    if (!widget) {
        return;
    }
    widget->setEnabled(enable);
}

QList<ScanPathFactory *> ScanPathFactory::allFactories()
{
    return m_factories;
}

ScanPathFactory *ScanPathFactory::fromName(const QString &name)
{
    auto *factory = Utils::findOrDefault(m_factories, [&] (ScanPathFactory *f) {
        return f->name() == name;
    });
    if (!factory) {
        qCWarning(debug) << "Trying to access invalid factory" << name;
    }
    return factory;
}

ScanPathFactory *ScanPathFactory::advancedFactory()
{
    return Utils::findOrDefault(m_factories, [&] (ScanPathFactory *f) {
        return f->metaObject() == &ScanPathAdvanced::staticMetaObject;
    });
}
