#ifndef PARSER_H
#define PARSER_H

#include "DataDef.h"
#include <QtMath>
#include <QObject>
#include <QTcpSocket>
#include <QStringList>
#include <Ultrasonic/IInstrumentDriver.h>

namespace NS_PXI100 {

/*!
 * \brief Parser 是 PXI100 驱动模块中，提供数据格式转换服务的类。
 * 由于数据格式转换涉及一些状态变量，一个 Parser 实例负责维护一个通道的参数，
 * \inmodule PXI100
 */
class Parser : QObject
{
    Q_OBJECT

public:
    enum FeatureDataType
    {
        FEATURE_ADATA,
        FEATURE_CDATA
    };

public:
    Parser(const QJsonObject &config, QObject *parent = 0);

public:
    static bool PeekPacketHead(QTcpSocket *socket, int packetLength, const CommandInt expectedCmd, const qint32 &expectedLength);
    static void ReadPacketHead(QTcpSocket *socket, PacketHead &head);
    void ReadADataFrame(QTcpSocket *socket, UTSharedDataFrame &frame);
    void ReadCDataFrame(QTcpSocket *socket, UTSharedDataFrame &frame, FeatureDataType dataType);
    TCGData CalcTCG(const QJsonArray TCGFromUTSetup);
    QByteArray fromStruct(const ParameterTable &obj, int channel);
    static ParameterTable toStruct(QByteArray& array, int channel);
    int GetSampleInterval();
    int GettcgRamSize();
    void MonitorCData2Raw(UTSharedDataFrame frame, CDataValueRaw* buffer);
    TCGData EmptyTCG();

private:
    qint16 Gain2Data(double data);
    CH_CONFIG data;
    double delayTimeFlaw;
    double delayTimeSurface;

private:
    static QStringList ParameterList;
    const static int WAVE_SPEED = 2000;        /* 补偿下位机使用的声速 */
    const static int DEFAULT_FPGA_FREQUENCY = 100;
    const static int DEFAULT_AD_FREQUENCY = 400;
    const static int DEFAULT_FPGA_RAM_SIZE = 16384;
    const static int RANGE_BORDER_C = 10;
    const double AEC_AMP_LINEAR_PARA_A = 0.9;
    int nSampleInterval;
    int FPGA_Frequency;                  /* FPGA的核心频率，单位为MHz */
    int AD_Frequency;                    /* A/D转换器的等效频率，单位为MHz */
    int DA_Frequency;                    /* D/A转换器的等效频率，单位为MHz */
    int TCG_RAM_Size;                   /*TCG的RAM大小*/
    int FPGA_RAM_Size;                 /* FPGA上A扫描数据RAM的点数 */
    enum CDataType
    {
        TIME_DATA,
        AMP_DATA
    };
    QList<CDataType> DataType;
};

typedef QList<Parser *> ParserList;   // wangwei modified

}   /* namespace NS_PXI100 */

#endif // PARSER_H
