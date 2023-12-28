#include <Ultrasonic/IInstrumentDriver.h>
#include <Ultrasonic/UTDataHeader.h>
#include <QVariantMap>

static QList<IInstrumentDriverFactory *> m_factories;

IInstrumentDriver::IInstrumentDriver(QObject *parent) :
    QObject(parent),
    status(InstrumentStatus())
{}

void IInstrumentDriver::setActiveChannel(int channel)
{
    Q_UNUSED(channel);
}

void IInstrumentDriver::sendCommand(const QString &command)
{
    Q_UNUSED(command);
}

QVariantMap IInstrumentDriver::getVariables() const
{
    return QVariantMap();
}

void IInstrumentDriver::setVariables(const QVariantMap &var)
{
    Q_UNUSED(var);
}

void IInstrumentDriver::sendRawMessage(int type, const QVariant &message)
{
    Q_UNUSED(type);
    Q_UNUSED(message);
}

void IInstrumentDriver::setIdentifier(const QHash<int, ChannelIdentifier> &identifier)
{
    Q_UNUSED(identifier);
}

InstrumentInfo IInstrumentDriver::instrumentInfo() const
{
    return info;
}

QVariant IInstrumentDriver::supportedSetup(const UTSetup &setup, const QString &key) const
{
    Q_UNUSED(setup);
    Q_UNUSED(key);
    return false;
}

void IInstrumentDriver::setChannelSetup(int channel, const UTSetup &setup)
{
    Q_UNUSED(channel);
    Q_UNUSED(setup);
}

void IInstrumentDriver::setInstrumentSetup(const InstrumentSetup &setup)
{
    Q_UNUSED(setup);
}

void IInstrumentDriver::setPoseOffset(const UTPose &offset)
{
    Q_UNUSED(offset);
}

void IInstrumentDriver::setInstrumentStatus(InstrumentStatus::State state, const QString &errorString)
{
    if (status.state != state) {
        status.state = state;
        emit instrumentStatusChanged(status);
    }
    if (status.errorString != errorString) {
        status.errorString = errorString;
        emit instrumentStatusChanged(status);
    }
}

InstrumentStatus IInstrumentDriver::instrumentStatus() const
{
    return status;
}

IInstrumentDriverFactory::IInstrumentDriverFactory(QObject *parent) :
    QObject(parent)
{
    m_factories << this;
}

IInstrumentDriverFactory::~IInstrumentDriverFactory()
{
    m_factories.removeAll(this);
}

QList<IInstrumentDriverFactory *> IInstrumentDriverFactory::allFactories()
{
    return m_factories;
}
