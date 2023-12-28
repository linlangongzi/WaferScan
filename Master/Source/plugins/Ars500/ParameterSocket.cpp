#include "ParameterSocket.h"
#include "Parser.h"
#include <QJsonObject>
#include <Ultrasonic/UTSetup.h>
using namespace Ars500;

ParameterSocket::ParameterSocket(ParserList parsers, QObject *parent) :
    QTcpSocket(parent),
    parsers(parsers)
{
    connect( this, SIGNAL( readyRead() ), this, SLOT( OnReceiveData() ) );
}

void ParameterSocket::ResetPose()
{
    Parser::SendCommand(this, CMD_RESET, 0, 0);
}

void ParameterSocket::SendParameter(const QVector<UTSetup> &parameters, int channel)
{
    Q_ASSERT(parsers.count() > channel);
//    qDebug() << "ARS500: Sending parameters" << channel;
    int dataChannel = channel;                                                  /* 数据来源通道 */
    if ((channel == 1) && parameters.value(channel).alterSource()) {  /* 当关联检测时，配置来源固定为0通道 */
        UTSetup table = parameters.value(0);
        parsers[channel]->UpdateStruct(table, dataChannel);
        dataChannel = 0;
    }
    UTSetup table = parameters.value(dataChannel);
    parsers[channel]->UpdateStruct(table, dataChannel);
    parsers[channel]->data.nVoltage = parsers[0]->data.nVoltage;
    parsers[channel]->data.stCon.nRepeatCounter = parsers[0]->data.stCon.nRepeatCounter;
    Parser::SendCommand(this, CMD_ALL_PARAM_SET, dataChannel, 0, parsers[channel]->data);
}

void ParameterSocket::setTCG(const UTSetup &TCGArray, int channel, bool enabled)
{
//    qDebug() << "ARS500: Sending TCG" << channel;
    TCGData tcg = enabled ? parsers[channel]->CalcTCG(TCGArray.toJson().toArray()) : parsers[channel]->EmptyTCG();
    int bytesTCG = sizeof(TCGData::value_type) * tcg.count();
    PacketHead head;
    head.lCommand = CMD_TCG_SWITCH;
    head.nChannelIndex = channel;
    head.nGateIndex = 0;
    head.lSize = sizeof(TCGCommuData) + bytesTCG;
    write( reinterpret_cast<char *>(&head), sizeof(head) );

    TCGCommuData stTCGCommuData;
    stTCGCommuData.bOpenFlag = enabled;                                         //需修改
    stTCGCommuData.nSamplingInterval = parsers[channel]->GetSampleInterval();   /* 下位机的SampleInterval是时钟周期的倍数，1为80MHz，2为40MHz，以此类推 */
    stTCGCommuData.nSramNum = parsers[channel]->GetTcgRamSize();
    write( reinterpret_cast<char *>(&stTCGCommuData), sizeof(stTCGCommuData) );

    write(reinterpret_cast<char *>( tcg.data() ), bytesTCG);
    flush();
}

void ParameterSocket::SendData(const QByteArray &data)
{
    if (isOpen()) {
        write(data);
        flush();
    }
}

void ParameterSocket::RequestVersion()
{
    PacketHead head;
    head.lCommand = CMD_VERSION_REQ;
    head.nChannelIndex = 0;
    head.nGateIndex = 0;
    head.lSize = sizeof(PacketHead);
    write( reinterpret_cast<char *>(&head), sizeof(head) );
    flush();
}

void ParameterSocket::OnReceiveData()
{
    PacketHead head;
    /* 需要尝试读取多次，直到缓冲区耗竭 */
    qint64 remaining = bytesAvailable() - sizeof(PacketHead);
    while (remaining) {                                                         /* 也可以使用do ... while循环，但出于栈分配的考虑，这样反而降低执行效率。 */
        /* 包头检查时，不使用read读取数据，因为read会将数据从缓冲区中弹出，在包头未对齐时将导
         * 致难以逐字节向后搜索包头。此处使用peek，将包头读出，但不影响缓冲区。 */
        peek(reinterpret_cast<char *>(&head), sizeof(PacketHead));              /* 此处可用长度可能不够，但此时由于remaining < 0，head将不被读取，不会造成异常。 */
        for (remaining = bytesAvailable() - head.lSize - sizeof(PacketHead);    /* 更新remaining字节数 */
        remaining >= 0 && !IsValidHead(head);/* 如果包长度足够，并且peek到的不是有效包头，则向后逐字节搜索有效包头。 */
        --remaining) {
//            qDebug() << "Parser::HasPacketHead: got invalid header" << head.lCommand << head.lSize << head.nChannelIndex << head.nGateIndex;
            char dummy;
            read(&dummy, 1);                                                    /* 从Socket缓冲区中弹出一个字节。这个操作效率比较低，但是不经常执行。 */
            peek( reinterpret_cast<char *>(&head), sizeof(PacketHead) );        /* 更新包头数据 */
        }

        if ( Q_UNLIKELY(remaining <= 0) ) {                                     /* 包长度不足，结束所有处理 */
            return;
        }
        read( reinterpret_cast<char *>(&head), sizeof(PacketHead) );            /* 将包头从Socket缓冲区中弹出 */
        AnalyzeData(head);                                                      /* 现在Socket缓冲区中应当是包体，调用派生类的回调函数读取。 */
    }
}

/*!
 * \brief ParameterSocket::AnalyzeData 解析数据
 * \param head
 */
void ParameterSocket::AnalyzeData(const PacketHead &head)
{
    if (head.lCommand == CMD_VERSION_REQ) {                                     //版本
        VersionData version;
        read( reinterpret_cast<char *>(&version), sizeof(VersionData) );
        QByteArray additionalVersion = read(qMax<qint64>(0, head.lSize - sizeof(VersionData))); /* 将多余数据弹出 */
        QString strVersion;
        strVersion = tr("Hardware: %1/Software: %2").arg(version.wHardWareVer, 6, 'f', 2).arg(version.wSoftWareVer, 6, 'f', 2);
        qDebug() << "Instrument version: " << strVersion << QString(additionalVersion);
        emit VersionUpdate(strVersion);
    } else {
        read(head.lSize);
    }
}

/*!
 * \brief ParameterSocket::IsValidHead 判断包头是否合法
 * \param head
 * \return  true:合法
 *          false:不合法
 */
bool ParameterSocket::IsValidHead(const PacketHead &head)
{
    switch (head.lCommand) {
    case CMD_VERSION_REQ: {
        if (head.lSize == sizeof(VersionData)) {
            return true;
        }
    }
    case CMD_WORK_MODE_SET:
        return true;
    default:
        return true;
    }
}
