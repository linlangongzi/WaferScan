#include "UTSetup.h"
#include "UTChannel.h"
#include "UTInstrument.h"
#include "Platform/Platform.h"
#include <QDebug>

UTChannel::UTChannel(QObject *parent) :
    QObject(parent)
{
    bindSetup = NULL;
}

void UTChannel::BindUTSetup(ConfigID id)
{
    UTSetup * setup = platform()->Setups()->GetByID<UTSetup>(id);
    if (setup == NULL)
    {
        qWarning() << "UTChannel::BindUTSetup id is not valible";
        return;
    }
    if (bindSetup == setup)
    {
        return;
    }
    UnbindUTSetup();
    // 让工艺绑定
    setup->BindChannel(this);
    UTInstrument *instrument = dynamic_cast<UTInstrument *>(this->parent());
    if (instrument->Get("ID") == QJsonValue("1"))
    {
        Set(QString("ChannelBindSetup%1").arg(instrument->GetChannelMap()->keys().value(channelIndex)), Config::FromObjectID(id));
    }
    else if (instrument->Get("ID") == QJsonValue("2"))
    {
        int count = instrument->Get("NumChannels").toInt();
        int index = instrument->GetChannelMap()->keys().value(channelIndex + count);
        Set(QString("ChannelBindSetup%1").arg(index), Config::FromObjectID(id));
    }

    bindSetup = setup;
    // 绑定完成需要发送绑定改变信号（绑定改变需要重新下发通道参数）
    emit BindUTSetupChanged(channelIndex, id);
    // 连接bindsetup 的信号
    ConnectSignalOnSetup();
    // 绑定之后需要重新连接仪器接收数据信号
//    connectReceivedData();
}

void UTChannel::BindUTSetup(UTSetup *setup)
{
    if (setup == NULL)
    {
        qWarning() << "UTChannel::BindUTSetup: setup is not valid";
        return;
    }
    if (bindSetup == setup)
    {
        return;
    }
    UnbindUTSetup();
    // 让工艺绑定
    setup->BindChannel(this);
    UTInstrument *instrument = dynamic_cast<UTInstrument *>(this->parent());
    if (instrument->Get("ID") == QJsonValue("1"))
    {
        Set(QString("ChannelBindSetup%1").arg(instrument->GetChannelMap()->keys().value(channelIndex)), setup->Get("ID"));
    }
    else if (instrument->Get("ID") == QJsonValue("2"))
    {
        int count = instrument->Get("NumChannels").toInt();
        int index = instrument->GetChannelMap()->keys().value(channelIndex + count);
        Set(QString("ChannelBindSetup%1").arg(index), setup->Get("ID"));
    }

    bindSetup = setup;
    // 绑定完成需要发送绑定改变信号（绑定改变需要重新下发通道参数）
    emit BindUTSetupChanged(channelIndex, setup->GetID());
    // 连接bindsetup 的信号
    ConnectSignalOnSetup();
    // 绑定之后需要重新连接仪器接收数据信号
//    connectReceivedData();
}

void UTChannel::UnbindUTSetup()
{
    // 需要断开信号连接
//    disconnectReceivedData();
    // 让工艺解绑
    if (bindSetup)
    {
        bindSetup->UnbindChannel(this);
    }
    // 使用Set 设置值为0
    UTInstrument *instrument = dynamic_cast<UTInstrument *>(this->parent());
    if (instrument->Get("ID") == QJsonValue("1"))
    {
        Set(QString("ChannelBindSetup%1").arg(instrument->GetChannelMap()->keys().value(channelIndex)), QJsonValue());
    }
    else if (instrument->Get("ID") == QJsonValue("2"))
    {
        int count = instrument->Get("NumChannels").toInt();
        int index = instrument->GetChannelMap()->keys().value(channelIndex + count);
        Set(QString("ChannelBindSetup%1").arg(index), QJsonValue());
    }

    // 断开bindsetup 的信号
    DisonnectSignalOnSetup();
    // 断开之后bindsetup 为空
    bindSetup = NULL;
}

UTSetup *UTChannel::GetBindUTSetup()
{
    return dynamic_cast<UTSetup *>(bindSetup.data());
}

UTChannel::ConfigID UTChannel::GetBindUTSetupID()
{
    return bindSetup.data()->GetID();
}

int UTChannel::GetIndex()
{
    return channelIndex;
}

void UTChannel::Set(const QString &key, const QJsonValue &value)
{
    UTInstrument *instrument = dynamic_cast<UTInstrument *>(parent());
    Q_ASSERT(instrument);
    instrument->Set(key, value);
}

void UTChannel::SetIndex(int channel, int instrument)    // 设置通道号
{
    channelIndex = channel;
    instrumentIndex = instrument;
}

void UTChannel::SetChannelParamter(const QString &key, const QJsonValue &value)
{
    emit ChannelParamterChanged(channelIndex, key, value);
}

QJsonObject UTChannel::GetChannelParamter()
{
    if (bindSetup)
    {
        QJsonObject obj = bindSetup->RequestRefresh();
        return obj;
    }
    return QJsonObject();
}

void UTChannel::ConnectSignalOnSetup()
{
    if (bindSetup)
    {
        connect(bindSetup, SIGNAL(ValueChangedSetup(const QString&, const QJsonValue&)), this, SLOT(SetChannelParamter(const QString&, const QJsonValue&)));
    }
}

void UTChannel::DisonnectSignalOnSetup()
{
    if (bindSetup)
    {
        disconnect(bindSetup, SIGNAL(ValueChangedSetup(const QString&, const QJsonValue&)), this, SLOT(SetChannelParamter(const QString&, const QJsonValue&)));
    }
}

void UTChannel::SetGlobalIndex(int index)
{
    nGlobaIndex = index;
}

int UTChannel::GetGlobalIndex()
{
    return nGlobaIndex;
}

void UTChannel::OnDriverDataArrived(UTSharedDataSegment sharedata)
{
    if (!sharedata->isEmpty() && sharedata->value(0)->Context.GetChannelID() == channelIndex)   // 一个C 扫数据包中的每一个帧都来源于同一个通道
    {
        emit DataArrived(sharedata);
    }
}

void UTChannel::OnDriverMonitorArrived(UTSharedDataFrame sharedata)
{
    if (sharedata->Context.GetChannelID() == channelIndex)
    {
        sharedata->Context.SetInstrumentID(instrumentIndex);
#ifdef ARS_ARS3D
        Q_ASSERT_X(sharedata->Context.GetChannel() == this, "UTChannel::OnDriverMonitorArrived", "channel pointer mismatch");
#endif
        emit MonitorDataArrived(sharedata);
    }
}
