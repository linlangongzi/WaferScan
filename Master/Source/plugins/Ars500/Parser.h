#pragma once

#include <Ultrasonic/IInstrumentDriver.h>
#include "DataDef.h"
#include <QObject>
#include <QStringList>
#include <QTcpSocket>
#include <QtMath>

namespace Ars500 {

/*!
 * \brief Parser 是 ARS500 驱动模块中，提供数据格式转换服务的类。
 * 由于数据格式转换涉及一些状态变量，一个 Parser 实例负责维护一个通道的参数，
 */
class Parser : QObject
{
    Q_OBJECT

    friend class ParameterSocket;

public:
    Parser(const QJsonObject &config, QObject *parent = nullptr);

public:
    static bool PeekPacketHead(QTcpSocket *socket, int packetLength, const CommandInt expectedCmd, const qint32 &expectedLength);
    static void ReadPacketHead(QTcpSocket *socket, PacketHead &head);
    qint64 ReadADataFrame(QTcpSocket *socket, UTDataFrame &frame);
    qint64 ReadCompressedADataFrame(QTcpSocket *socket, UTDataFrame &frame);
    qint64 ReadCDataFrame(QTcpSocket *socket, UTDataFrame &frame);
    qint64 ReadCDataFrame(QTcpSocket *socket, UTDataFrame &frame, int timeMultiplier);
    TCGData CalcTCG(const QJsonArray TCGFromUTSetup);
    TCGData EmptyTCG();
    void UpdateStruct(const UTSetup &obj, int channel);
    //static ParameterTable toStruct(const QByteArray& array, int channel);
    int GetSampleInterval();
    int GetTcgRamSize();
    void UpdateChannelOffset(const UTPose &offset);

protected:
    Config data;
    float delayTime;

private:
    static const int WAVE_SPEED = 2000;                                         /* 补偿下位机使用的声速 */
    static const int DEFAULT_FPGA_FREQUENCY = 100;
    static const int DEFAULT_AD_FREQUENCY = 400;
    static const int DEFAULT_FPGA_RAM_SIZE = 16384;
    int nSampleInterval;
    int FPGA_Frequency;                                                         /* FPGA的核心频率，单位为MHz */
    int AD_Frequency;                                                           /* A/D转换器的等效频率，单位为MHz */
    int DA_Frequency;                                                           /* D/A转换器的等效频率，单位为MHz */
    int TCG_RAM_Size;                                                           /* TCG的RAM存储点数 */
    int FPGA_RAM_Size;                                                          /* FPGA上A扫描数据RAM的点数 */
    UTPose poseOffset;

public:
    static inline void SendCommand( QTcpSocket *socket, CommandInt cmd, qint16 channel, qint16 gateIndex, const QByteArray &data = QByteArray() )
    {
        if (!socket->isOpen()) {                                                /* 连接状态检查 */
            return;
        }

        PacketHead head;
        head.lCommand = cmd;
        head.nChannelIndex = channel;
        head.nGateIndex = gateIndex;
        head.lSize = data.size();

        socket->write( reinterpret_cast<const char *>(&head), sizeof(head) );
        socket->write(data);
        socket->flush();
    }

    template <typename T>
    static inline void SendCommand(QTcpSocket *socket, CommandInt cmd, qint16 channel, qint16 gateIndex, const T &data)
    {
        SendCommand( socket, cmd, channel, gateIndex, QByteArray( reinterpret_cast<const char *>(&data), sizeof(data) ) );
    }
};

typedef QList<Parser *> ParserList;                                             // wangwei modified

}                                                                               /* namespace ARS500 */
