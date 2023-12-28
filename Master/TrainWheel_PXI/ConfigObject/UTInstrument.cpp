#include "UTInstrument.h"
#include "UTSetup.h"
#include "Platform/Platform.h"
#include <QDebug>

UTInstrument::UTInstrument(QObject *parent) :
    Config(parent)
{
    SetRaw("Type", "UTInstrument");
    InitInstrument();
}

ConfigObject *UTInstrument::New(QObject *parent) const
{
    return dynamic_cast<ConfigObject *>(new UTInstrument(parent));
}

void UTInstrument::InitInstrument()
{
    channels.clear();
    bInitCompleted = false;
}

void UTInstrument::LoadCompleted()
{
    InitChannelMap();
    InitChannelList();
    // 加载配置文件完成之后需要进行绑定
    int nChnNum = GetNumChannels();
    for(int i = 0; i < nChnNum; i++)
    {
        BindSetupToChannel(i, m_channelMap.key(i));;
    }

    // 初始化完成
    bInitCompleted = true;
}

UTChannel *UTInstrument::GetChannel(int index)
{
    return channels.at(index).data();
}

UTInstrument::ChannelList UTInstrument::GetChannels()
{
    return ChannelList(channels);
}

UTInstrument::ChannelList UTInstrument::GetAllChannels()
{
    ChannelList list;
    int index = 0;
    ConfigManager::Registry reg = platform()->Instruments()->GetRegistry();
    QList<ConfigID> keys = reg.keys();
    qSort(keys);
    foreach (ConfigID id, keys)
    {
        UTInstrument *instrument = dynamic_cast<UTInstrument *>(reg[id].data());
        if (instrument != NULL)
        {
            foreach (ChannelPointer channel, instrument->GetChannels())
            {
                index++;
                channel->SetGlobalIndex(index);
                list.push_back(channel);
            }
        }
    }
    return list;
}

const QStringList UTInstrument::GetCDataDescription()
{
    return QStringList();
}
UTInstrument::ChannelMap *UTInstrument::GetChannelMap()
{
    return &m_channelMap;
}

void UTInstrument::BindSetupToChannel(int channel, ConfigID id)
{
    if (id == 0)
    {
        qWarning() << "UTInstrument::BindSetupToChannel input id is not valid";
        return;
    }
    if (channel < 0 || channel >= channels.count())
    {
        qWarning() << "UTInstrument::BindSetupToChannel input channel is not valid";
        return;
    }
    channels.at(channel)->BindUTSetup(id);
}

void UTInstrument::InitChannelMap()
{
    ConfigManager::Registry reg = platform()->Setups()->GetRegistry();
    QList<ConfigID> keys = reg.keys();
    //qSort(keys);
    int nChnIdx = 0;
    foreach (ConfigID id, keys)
    {
        m_channelMap.insert(id, nChnIdx);
        nChnIdx++;
    }
}

void UTInstrument::InitChannelList()
{
    QString deviceType = Get("Device").toString();
    if (deviceType == "PXI100")
    {
        channels.reserve(GetNumChannels());
        //  开始初始化通道以及通道的参数表
        for (int index = 0; index < GetNumChannels(); index++)
        {
            ChannelPointer sharedata(new UTChannel(this));
            sharedata->SetIndex(index, GetID());
            channels.append(sharedata);

            // 查找绑定的UTSetup, 组建驱动本地的参数表
            //ConfigID setupid = GetAsID(QString("ChannelBindSetup%1").arg(m_channelMap.keys().value(index)));
            //UTSetup *setup = dynamic_cast<UTSetup*>(platform()->Setups()->GetByID(setupid));
            //Q_UNUSED(setup);
        }
    }
    else
    {
        qCritical() << "UTInstrument::InitDriver invalid device type " << Get("Device");
        return;
    }
}

void UTInstrument::Set(const QString &key, const QJsonValue &value)
{
    if (key == "Device")
    {
        qWarning() << "Cannot change device type.";
        return;
    }

    Config::Set(key, value);
}

int UTInstrument::GetNumChannels()
{
    return Get("NumChannels").toInt();
}
