#include "PXI100.h"
#include <Ultrasonic/UTInstrument.h>
#include "CScanSocket.h"
#include "MonitorSocket.h"
#include "EyesStatusSocket.h"

using namespace NS_PXI100;

const QString UTDriverPXI100::version = "1.0";
const QString UTDriverPXI100::deviceType = "PXI100";

QStringList InitCDataDescription()
{
    QStringList list;
    list << "Gate1SyncTime" << "Gate1MPTime" << "Gate1MPAmp"
         << "Gate2MPTime" << "Gate2MPAmp" << "Gate2FPFirstTime" << "Gate2FPTime" << "Gate2FPAmp"
         << "Gate3MPTime" << "Gate3MPAmp" << "Gate3FPFirstTime" << "Gate3FPTime" << "Gate3FPAmp"
         << "Gate4MPTime" << "Gate4MPAmp" << "Gate4FPFirstTime" << "Gate4FPTime" << "Gate4FPAmp";
    return list;
}

const QStringList UTDriverPXI100::CDataDescription = InitCDataDescription();

UTDriverPXI100::UTDriverPXI100(const QJsonArray &config, QObject *parent):
    IInstrumentDriver(parent),
    status(IInstrumentDriver::DISCONNECTED)
{
    int nChnNum = GetNumChannels();
    for (int i = 0; i < nChnNum; i++) {
        parsers.push_back(new Parser(config[0].toObject(), this));  //config 只有一份参数
    }
    m_MonitorSocket = new MonitorSocket(parsers, this);
    m_CScanSocket = new CScanSocket(parsers, this);
    m_ParameterSocket = new ParameterSocket(parsers, this);
    m_EyesStatusSocket = new EyesStatusSocket(parsers, this);
    m_MfcEyesStatusSocket = new EyesStatusSocket(parsers, this);

    DataLevel level = CSCAN_ONLY;
    m_MonitorSocket->SetDataLevel(level);
    m_CScanSocket->SetDataLevel(level);
    m_EyesStatusSocket->SetDataLevel(level);
    m_MfcEyesStatusSocket->SetDataLevel(level);
    m_ParameterSocket->SetDataLevel(CSCAN_ONLY);

    InitSockets();
}

void UTDriverPXI100::SetHostAddr(const QHostAddress &ip)
{
    hostName = ip;
}

void UTDriverPXI100::SetHostAddr(const QString &ip)
{
    hostName = ip;
}

void UTDriverPXI100::Connect()
{
    status = INITIALIZING;
    m_MonitorSocket->abort();
    m_CScanSocket->abort();
    m_ParameterSocket->abort();
    m_EyesStatusSocket->abort();
    m_MfcEyesStatusSocket->abort();
    m_MonitorSocket->connectToHost(hostName, MONITOR_PORT);
    m_CScanSocket->connectToHost(hostName, CSCAN_PORT);
    m_ParameterSocket->connectToHost(hostName, PARAM_PORT);
    m_EyesStatusSocket->connectToHost(hostName, EYES_STATUS_PORT);
    m_MfcEyesStatusSocket->connectToHost(hostName, MFC_EYES_STATUS_PORT);
}

void UTDriverPXI100::Disconnect()
{
    m_MonitorSocket->disconnectFromHost();
    m_CScanSocket->disconnectFromHost();
    m_ParameterSocket->disconnectFromHost();
    m_EyesStatusSocket->disconnectFromHost();
    m_MfcEyesStatusSocket->disconnectFromHost();
}

void UTDriverPXI100::InitSockets()
{
    for (QTcpSocket *socket: findChildren<QTcpSocket *>()) {
        socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
        connect(socket, &QAbstractSocket::connected, this, &UTDriverPXI100::OnConnected);
        connect(socket, &QAbstractSocket::disconnected, this, &UTDriverPXI100::OnDisconnected);
    }

    connect(m_MonitorSocket, &MonitorSocket::DataArrived, this, &UTDriverPXI100::OnMonitorReceived);
    connect(m_CScanSocket, &CScanSocket::DataArrived, this, &UTDriverPXI100::OnCScanReceived);
    connect(m_EyesStatusSocket, &EyesStatusSocket::DataArrived, [this] (UTSharedDataSimulation simulation) {
        emit rawMessageArrived(MessageTrainSensor, QVariant::fromValue(simulation));
    });

//    connect(m_ParameterSocket, &ParameterSocket::VersionLowerRead, this, &UTInstrumentDriver::VersionLowerReceived);
    connect(m_ParameterSocket, &ParameterSocket::PxiInitResultRead, [this] (int result) {
        emit rawMessageArrived(MessageError, result);
    });
    connect(m_EyesStatusSocket, &EyesStatusSocket::mfcDataArrived, this, &UTDriverPXI100::onMfcDataArrived);
    connect(m_MfcEyesStatusSocket, &EyesStatusSocket::mfcDataArrived, this, &UTDriverPXI100::onMfcDataArrived);
}

void UTDriverPXI100::OnConnected()
{
    bool connected = true;
    for (QTcpSocket *socket: findChildren<QTcpSocket *>()) {
        if (socket->state() != QTcpSocket::ConnectedState) {
            connected = false;
        }
    }
    if (connected) {
        qDebug() << "connect successed";
        status = IInstrumentDriver::CONNECTED;
        //连接成功之后需要立即发送A扫与C扫的数据请求,以及A扫存储请求
        m_MonitorSocket->RequestData();
        m_CScanSocket->RequestData();
        m_ParameterSocket->VersionLowerRequest();
        m_ParameterSocket->ReqPLXStatus();

        // 利用写传感器状态的方式，让每个aec卡处于工作状态，这样才能写通道号成功
        UTSharedDataSimulation data = UTSharedDataSimulation::create();
        std::memset(data->sLightEye, 31, 9);
        m_ParameterSocket->SetSimulationData(data);

        std::memset(data->sLightEye, 0, 9);
        m_ParameterSocket->SetSimulationData(data);

        emit ConnectionChanged(status);
    }
}

void UTDriverPXI100::OnDisconnected()
{
    status = IInstrumentDriver::DISCONNECTED;

    emit ConnectionChanged(status);
}

void UTDriverPXI100::OnMonitorReceived(UTSharedDataFrame dataSegment)
{
    emit MonitorDataArrived(dataSegment);
}

void UTDriverPXI100::OnCScanReceived(UTSharedDataSegment dataSegment)
{
    emit DataArrived(dataSegment);
}

IInstrumentDriver::ConnectionStatus UTDriverPXI100::GetConnectionStatus()
{
    return status;
}

int UTDriverPXI100::GetNumChannels()
{
    UTInstrument* insturment = dynamic_cast<UTInstrument*>(parent());
    return insturment->GetNumChannels();
}

int UTDriverPXI100::GetNumAxes()
{
    return NUM_AXES;
}

int UTDriverPXI100::GetNumGates()
{
    return NUM_GATES;
}

const QString &UTDriverPXI100::Version()
{
    return version;
}

const QString &UTDriverPXI100::DeviceType()
{
    return deviceType;
}

void UTDriverPXI100::Reset()
{
    Disconnect();
}

void UTDriverPXI100::SetParameter(const QString &key, const QJsonValue &value)
{
    Q_UNUSED(key);
    Q_UNUSED(value);
}

void UTDriverPXI100::SetChannelParameter(int index, int channel, const QString &key, const QJsonValue &value)
{
    // TODO
    if (status != CONNECTED) {
//        qWarning() << "UTDriverPXI100::SetChannelParameter on" << key << "failed: not connected";
        return;
    }

    int nChnNum = GetNumChannels();
    if (index > nChnNum) {
        qWarning() << "UTDriverPXI100::SetChannelParameter input channel is out of limit";
        return;
    }

    if (!(key.isNull() && value.isNull())) {
       localParameter[index].insert(key, value);
    }

    m_ParameterSocket->SetParameter(localParameter[index], channel, index);

    if (key == "TcgEnable" || key == "TCG" || key == "Range") {
        bool enabled = localParameter[index].value("TcgEnable").toBool();
        m_ParameterSocket->SetTCG(localParameter[index].value("TCG").toArray(), channel, index, enabled);
    }
}

void UTDriverPXI100::SetCurChannel(qint32 channelIndex, qint16 surOrFlaw)
{
    m_ParameterSocket->SetSelectChn(channelIndex, surOrFlaw);
}

void UTDriverPXI100::SetRepeatFrequence(qint32 nValue)
{
    m_ParameterSocket->SetRepeatFrequence(nValue);
}

void UTDriverPXI100::sendRawMessage(int type, const QVariant &message)
{
    if (type == MessageTrainScanStatus) {
        m_ParameterSocket->SetScanStatus(message.toInt());
    } else if (type == MessageTrainSensor) {
        m_ParameterSocket->SetSimulationData(message.value<UTSharedDataSimulation>());
    }
}

void UTDriverPXI100::SetLocalParamterTable(QVector<ParameterTable> objList)
{
    localParameter = objList;
}

void UTDriverPXI100::UpdateLocalParamteratable(int channel, const ParameterTable& table)
{
    if ((channel < 0) || channel >= localParameter.count())
    {
        qWarning() << "UTDriverPXI100::UpdateLocalParamteratable input channel is out of limite";
        return;
    }
    localParameter.replace(channel, table);
}

QStringList UTDriverPXI100::GetCDataDescription()
{
    return CDataDescription;
}

CDataTypeList UTDriverPXI100::GetCDataType()
{
    return CDataType;
}

void NS_PXI100::UTDriverPXI100::onMfcDataArrived(const QByteArray &data)
{
    emit rawMessageArrived(MessageDigitalIo, data);
}
