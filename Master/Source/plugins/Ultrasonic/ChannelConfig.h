#pragma once

#include <QObject>
#include <allrising/Util.h>

class ChannelIdentifier
{
    Q_GADGET

    Q_PROPERTY(QString instrument MEMBER instrument)
    Q_PROPERTY(int channel MEMBER channel)
    Q_PROPERTY(int cycle MEMBER cycle)

public:
    DECLARE_COMPARE_OPERATORS(ChannelIdentifier)

    ChannelIdentifier() = default;
    ChannelIdentifier(const QString &instrument, int channel, int cycle = 0) :
        instrument(instrument),
        channel(channel),
        cycle(cycle)
    {}

    QString instrument;
    int channel = 0;
    int cycle = 0;
};

class ChannelConfig
{
    Q_GADGET

    Q_PROPERTY(bool disabled MEMBER disabled)
    Q_PROPERTY(int channelId MEMBER channelId)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(ChannelIdentifier identifier MEMBER identifier)

public:
    DECLARE_COMPARE_OPERATORS(ChannelConfig)

    bool disabled = false;
    int channelId = 0;
    QString name;
    ChannelIdentifier identifier;
};
