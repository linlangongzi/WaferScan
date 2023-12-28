#include "UTSetupToJson.h"
#include "ui_UTSetupToJson.h"
#include "ConfigObject/JsonFile.h"
#include <QFile>
#include <QDebug>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>

UTSetupToJson::UTSetupToJson(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UTSetupToJson),
    filePath("./"),
    fileNameUTSetup("Setup.json"),
    fileNameChannelList("ChannelList.json"),
    fileNameInstrument("Instrument.json"),
    m_strProbeConnectFile("ProbeConnect.json")
{
    ui->setupUi(this);
    m_probeNoManager = new ProbeNoManager(this);
    InitUI();
    Connect();
    memset(AecChannel, -1, sizeof(AecChannel));
#if PUS_DOUBLE
    SetProbeConnectDouble(ProbeNoInfo::TrackLeft);
    SetProbeConnectDouble(ProbeNoInfo::TrackRight);
#else
    SetProbeConnect(10000);
    SetProbeConnect(20000);
#endif

    LoadProbeNoInfoJsonFile();
}

UTSetupToJson::~UTSetupToJson()
{
    delete ui;
}

void UTSetupToJson::DoJsonToFileUTSetup()
{
    if (GetFileType() == None)
    {
        qDebug() << "pleace check 10000 or 20000";
        return;
    }

    QJsonObject objChannel;
    objChannel.insert(QString("TimeBaseUnit"), QJsonValue("Distance"));
    objChannel.insert(QString("Gain"), QJsonValue(40.0));
    objChannel.insert(QString("Velocity"), QJsonValue(6000));
    objChannel.insert(QString("NumGates"), QJsonValue(4));
    objChannel.insert(QString("GainRef"), QJsonValue(0.0));
    objChannel.insert(QString("Range"), QJsonValue(2000.0));
    objChannel.insert(QString("Delay"), QJsonValue(0.0));
    objChannel.insert(QString("MaterialID"), QJsonValue(1));
    objChannel.insert(QString("Type"), QJsonValue("UTSetup"));
    objChannel.insert(QString("ID"), QJsonValue(QString::number(0)));
    objChannel.insert(QString("FlawDelay"), QJsonValue(0.0));
    objChannel.insert(QString("FlawSuppress"), QJsonValue(0.0));
    objChannel.insert(QString("FlawGate1Amp"), QJsonValue(10.0));
    objChannel.insert(QString("FlawGate2Amp"), QJsonValue(20.0));
    objChannel.insert(QString("FlawGate3Amp"), QJsonValue(30.0));
    objChannel.insert(QString("FlawGate4Amp"), QJsonValue(40.0));
    objChannel.insert(QString("FlawGate1Start"), QJsonValue(10.0));
    objChannel.insert(QString("FlawGate2Start"), QJsonValue(10.0));
    objChannel.insert(QString("FlawGate3Start"), QJsonValue(20.0));
    objChannel.insert(QString("FlawGate4Start"), QJsonValue(30.0));
    objChannel.insert(QString("FlawGate1Width"), QJsonValue(10.0));
    objChannel.insert(QString("FlawGate2Width"), QJsonValue(10.0));
    objChannel.insert(QString("FlawGate3Width"), QJsonValue(10.0));
    objChannel.insert(QString("FlawGate4Width"), QJsonValue(10.0));
    objChannel.insert(QString("FlawGate1Color"), QJsonValue("#ff0000"));
    objChannel.insert(QString("FlawGate2Color"), QJsonValue("#00ff00"));
    objChannel.insert(QString("FlawGate3Color"), QJsonValue("#0000ff"));
    objChannel.insert(QString("FlawGate4Color"), QJsonValue("#ffff00"));
    objChannel.insert(QString("FlawRectifierMode"), QJsonValue(2));
    objChannel.insert(QString("SurfaceDelay"), QJsonValue(0.0));
    objChannel.insert(QString("SurfaceSuppress"), QJsonValue(0.0));
    objChannel.insert(QString("SurfaceGate1Amp"), QJsonValue(10.0));
    objChannel.insert(QString("SurfaceGate2Amp"), QJsonValue(20.0));
    objChannel.insert(QString("SurfaceGate3Amp"), QJsonValue(30.0));
    objChannel.insert(QString("SurfaceGate4Amp"), QJsonValue(40.0));
    objChannel.insert(QString("SurfaceGate1Start"), QJsonValue(10.0));
    objChannel.insert(QString("SurfaceGate2Start"), QJsonValue(10.0));
    objChannel.insert(QString("SurfaceGate3Start"), QJsonValue(20.0));
    objChannel.insert(QString("SurfaceGate4Start"), QJsonValue(30.0));
    objChannel.insert(QString("SurfaceGate1Width"), QJsonValue(10.0));
    objChannel.insert(QString("SurfaceGate2Width"), QJsonValue(10.0));
    objChannel.insert(QString("SurfaceGate3Width"), QJsonValue(10.0));
    objChannel.insert(QString("SurfaceGate4Width"), QJsonValue(10.0));
    objChannel.insert(QString("SurfaceGate1Color"), QJsonValue("#ff0000"));
    objChannel.insert(QString("SurfaceGate2Color"), QJsonValue("#00ff00"));
    objChannel.insert(QString("SurfaceGate3Color"), QJsonValue("#0000ff"));
    objChannel.insert(QString("SurfaceGate4Color"), QJsonValue("#ffff00"));
    objChannel.insert(QString("SurfaceRectifierMode"), QJsonValue(2));
    objChannel.insert(QString("GainDelta"), QJsonValue(0));

    QJsonObject objTCG;
    QJsonArray arrayTCG;
    objTCG.insert(QString("Gain"), QJsonValue(70.0));
    objTCG.insert(QString("Time"), QJsonValue(10.0));
    arrayTCG.append(objTCG);
    objTCG["Gain"] = QJsonValue(60.0);
    objTCG["Time"] = QJsonValue(20.0);
    arrayTCG.append(objTCG);
    objTCG["Gain"] = QJsonValue(50.0);
    objTCG["Time"] = QJsonValue(30.0);
    arrayTCG.append(objTCG);
    objTCG["Gain"] = QJsonValue(60.0);
    objTCG["Time"] = QJsonValue(40.0);
    arrayTCG.append(objTCG);
    objTCG["Gain"] = QJsonValue(70.0);
    objTCG["Time"] = QJsonValue(50.0);
    arrayTCG.append(objTCG);
    objChannel.insert(QString("TCG"), QJsonValue(arrayTCG));

    QJsonArray arrayChannel;
    QString str("");
    FileType type;
    int start = 0;
    int end = 0;
    //int nSensorStart = 0;
    //int nSensorEnd = 0;

    if (GetFileType() == LowerComputer1)
    {
        start = m_probeNoManager->GetProbeMinLeft();
        end = m_probeNoManager->GetProbeMaxLeft();
        str = "LowerComputer1 creat succeed!";
        type = LowerComputer1;
    }
    else if (GetFileType() == LowerComputer2)
    {
        start = m_probeNoManager->GetProbeMinRight();
        end = m_probeNoManager->GetProbeMaxRight();
        str = "LowerComputer2 creat succeed!";
        type = LowerComputer2;
    }
    else if (GetFileType() == UpperComputer)
    {
        start = m_probeNoManager->GetProbeMin();
        end = m_probeNoManager->GetProbeMax();
        str = "UpperComputer creat succeed!";
        type = UpperComputer;
    }

    for (int channelID = start; channelID <= end; )
    {
        objChannel["ID"] = QJsonValue(QString::number(channelID));
        arrayChannel.append(objChannel);
        channelID = m_probeNoManager->GetNextProbeNo(channelID);
        if(channelID == INVALID_VALUE)
        {
            break;
        }
    }

    QJsonDocument jsonDoc(arrayChannel);
    QString path(filePath);
    QFile file(path.append(fileNameUTSetup));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QByteArray jsonStr = jsonDoc.toJson(QJsonDocument::Indented);   /* 根据Qt文档，将JsonDocument转为字符串的过程不会失败 */
        qDebug() << QString("Writing json to config file %1").arg(path);
        file.write(jsonStr);
        file.close();
    }
    else
    {
        qCritical() << QString("Failed open config file %1: %2").arg(path, file.errorString());
    }
}

void UTSetupToJson::DoJsonToFileChannelList()
{
    if (GetFileType() == None)
    {
        qDebug() << "pleace check 10000 or 20000";
        return;
    }

    QJsonArray arrayChannel;

    int nTypeNum = 3;
#if PROBE_SENSOR
    nTypeNum = 4;
#endif
    for (int type = 0; type <= nTypeNum; ++type)
    {
        QJsonArray arrayChild;
        QJsonObject objLeft;
        QJsonObject objRight;
        QJsonObject objChannel;

        if (type == 0)
        {
            QJsonValue valueLeft = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoLeft()->axialMin(),
                                                        m_probeNoManager->GetProbeNoInfoLeft()->axialMax()));
            QJsonValue valueRight = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoRight()->axialMin(),
                                                         m_probeNoManager->GetProbeNoInfoRight()->axialMax()));

            switch (GetFileType())
            {
            case LowerComputer1:
            {
                objChannel.insert(QString("Name"), QJsonValue("轮辋轮辐轴向踏面"));
                objChannel.insert(QString("Children"), valueLeft);
                arrayChannel.push_back(objChannel);
                break;
            }
            case LowerComputer2:
            {
                objChannel.insert(QString("Name"), QJsonValue("轮辋轮辐轴向踏面"));
                objChannel.insert(QString("Children"), valueRight);
                arrayChannel.push_back(objChannel);
                break;
            }
            case UpperComputer:
                objLeft.insert(QString("Name"), QJsonValue("左"));
                objLeft.insert(QString("Children"), valueLeft);
                arrayChild.push_back(objLeft);
                objRight.insert(QString("Name"), QJsonValue("右"));
                objRight.insert(QString("Children"), valueRight);
                arrayChild.push_back(objRight);
                objChannel.insert(QString("Name"), QJsonValue("轮辋轮辐轴向踏面"));
                objChannel.insert(QString("Children"), QJsonValue(arrayChild));
                arrayChannel.push_back(objChannel);
                break;

            default:
                break;
            }
        }
        else if (type == 1)
        {
            QJsonValue valueLeft = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoLeft()->radialFlangeMin(),
                                                        m_probeNoManager->GetProbeNoInfoLeft()->radialFlangeMax()));
            QJsonValue valueRight = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoRight()->radialFlangeMin(),
                                                         m_probeNoManager->GetProbeNoInfoRight()->radialFlangeMax()));

            switch (GetFileType())
            {
            case LowerComputer1:
                objChannel.insert(QString("Name"), QJsonValue("轮缘径向"));
                objChannel.insert(QString("Children"), valueLeft);
                arrayChannel.push_back(objChannel);
                break;

            case LowerComputer2:
                objChannel.insert(QString("Name"), QJsonValue("轮缘径向"));
                objChannel.insert(QString("Children"), valueRight);
                arrayChannel.push_back(objChannel);
                break;

            case UpperComputer:
                objLeft.insert(QString("Name"), QJsonValue("左"));
                objLeft.insert(QString("Children"), valueLeft);
                arrayChild.push_back(objLeft);
                objRight.insert(QString("Name"), QJsonValue("右"));
                objRight.insert(QString("Children"), valueRight);
                arrayChild.push_back(objRight);
                objChannel.insert(QString("Name"), QJsonValue("轮缘径向"));
                objChannel.insert(QString("Children"), QJsonValue(arrayChild));
                arrayChannel.push_back(objChannel);
                break;

            default:
                break;
            }
        }
        else if (type == 2)
        {
            QJsonValue valueLeft = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoLeft()->radialPositiveMin(),
                                                        m_probeNoManager->GetProbeNoInfoLeft()->radialPositiveMax()));
            QJsonValue valueRight = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoRight()->radialPositiveMin(),
                                                         m_probeNoManager->GetProbeNoInfoRight()->radialPositiveMax()));

            switch (GetFileType())
            {
            case LowerComputer1:
                objChannel.insert(QString("Name"), QJsonValue("轮辋轮辐径向正向"));
                objChannel.insert(QString("Children"), valueLeft);
                arrayChannel.push_back(objChannel);
                break;

            case LowerComputer2:
                objChannel.insert(QString("Name"), QJsonValue("轮辋轮辐径向正向"));
                objChannel.insert(QString("Children"), valueRight);
                arrayChannel.push_back(objChannel);
                break;

            case UpperComputer:
                objLeft.insert(QString("Name"), QJsonValue("左"));
                objLeft.insert(QString("Children"), valueLeft);
                arrayChild.push_back(objLeft);
                objRight.insert(QString("Name"), QJsonValue("右"));
                objRight.insert(QString("Children"), valueRight);
                arrayChild.push_back(objRight);
                objChannel.insert(QString("Name"), QJsonValue("轮辋轮辐径向正向"));
                objChannel.insert(QString("Children"), QJsonValue(arrayChild));
                arrayChannel.push_back(objChannel);
                break;

            default:
                break;
            }
        }
        else if (type == 3)
        {
            QJsonValue valueLeft = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoLeft()->radialNegativeMin(),
                                                        m_probeNoManager->GetProbeNoInfoLeft()->radialNegativeMax()));
            QJsonValue valueRight = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoRight()->radialNegativeMin(),
                                                         m_probeNoManager->GetProbeNoInfoRight()->radialNegativeMax()));

            switch (GetFileType())
            {
            case LowerComputer1:
                objChannel.insert(QString("Name"), QJsonValue("轮辋轮辐径向负向"));
                objChannel.insert(QString("Children"), valueLeft);
                arrayChannel.push_back(objChannel);
                break;

            case LowerComputer2:
                objChannel.insert(QString("Name"), QJsonValue("轮辋轮辐径向负向"));
                objChannel.insert(QString("Children"), valueRight);
                arrayChannel.push_back(objChannel);
                break;

            case UpperComputer:
                objLeft.insert(QString("Name"), QJsonValue("左"));
                objLeft.insert(QString("Children"), valueLeft);
                arrayChild.push_back(objLeft);
                objRight.insert(QString("Name"), QJsonValue("右"));
                objRight.insert(QString("Children"), valueRight);
                arrayChild.push_back(objRight);
                objChannel.insert(QString("Name"), QJsonValue("轮辋轮辐径向负向"));
                objChannel.insert(QString("Children"), QJsonValue(arrayChild));
                arrayChannel.push_back(objChannel);
                break;

            default:
                break;
            }
        }
        else if (type == 4) // 传感器探头
        {
            QJsonValue valueLeft = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoLeft()->sensorMin(),
                                                        m_probeNoManager->GetProbeNoInfoLeft()->sensorMax()));
            QJsonValue valueRight = QJsonValue(MakeArray(m_probeNoManager->GetProbeNoInfoRight()->sensorMin(),
                                                         m_probeNoManager->GetProbeNoInfoRight()->sensorMax()));

            switch (GetFileType())
            {
            case LowerComputer1:
                objChannel.insert(QString("Name"), QJsonValue("传感器"));
                objChannel.insert(QString("Children"), valueLeft);
                arrayChannel.push_back(objChannel);
                break;

            case LowerComputer2:
                objChannel.insert(QString("Name"), QJsonValue("传感器"));
                objChannel.insert(QString("Children"), valueRight);
                arrayChannel.push_back(objChannel);
                break;

            case UpperComputer:
                objLeft.insert(QString("Name"), QJsonValue("左"));
                objLeft.insert(QString("Children"), valueLeft);
                arrayChild.push_back(objLeft);
                objRight.insert(QString("Name"), QJsonValue("右"));
                objRight.insert(QString("Children"), valueRight);
                arrayChild.push_back(objRight);
                objChannel.insert(QString("Name"), QJsonValue("传感器"));
                objChannel.insert(QString("Children"), QJsonValue(arrayChild));
                arrayChannel.push_back(objChannel);
                break;

            default:
                break;
            }
        }
    }

    QJsonObject obj;
    obj.insert(QString("ChannelList"), QJsonValue(arrayChannel));
    QJsonDocument jsonDoc(obj);
    QString path(filePath);
    QFile file(path.append(fileNameChannelList));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QByteArray jsonStr = jsonDoc.toJson(QJsonDocument::Indented);   /* 根据Qt文档，将JsonDocument转为字符串的过程不会失败 */
        qDebug() << QString("Writing json to config file %1").arg(path);
        file.write(jsonStr);
        file.close();
    }
    else
    {
        qCritical() << QString("Failed open config file %1: %2").arg(path, file.errorString());
    }
}

void UTSetupToJson::DoJsonToFileUTInstrument()
{
    if (GetFileType() == None)
    {
        return;
    }

    QJsonArray arrayInstrument;
    QJsonArray arrayChannelPar;
    QJsonObject objInstrument;
    QJsonObject objChannelPar;
    objChannelPar.insert(QString("FPGA_Frequency"), QJsonValue(100));
    objChannelPar.insert(QString("AD_Frequency"), QJsonValue(100));
    objChannelPar.insert(QString("DA_Frequency"), QJsonValue(1));
    objChannelPar.insert(QString("TCG_RAM_Size"), QJsonValue(1024));
    objChannelPar.insert(QString("FPGA_RAM_Size"), QJsonValue(16384));
    objChannelPar.insert(QString("RectifierZero"), QJsonValue(490));
    objChannelPar.insert(QString("wAPlus"), QJsonValue(20));
    objChannelPar.insert(QString("wBPlus"), QJsonValue(20));
    objChannelPar.insert(QString("wCPlus"), QJsonValue(0));
    objChannelPar.insert(QString("wDPlus"), QJsonValue(0));
    arrayChannelPar.append(objChannelPar);

    objInstrument.insert(QString("Device"), QJsonValue("PXI100"));
    objInstrument.insert(QString("WorkMode"), QJsonValue(3));
    objInstrument.insert(QString("NumChannels"),  m_probeNoManager->GetProbeNoInfoLeft()->count());
    objInstrument.insert(QString("Type"), QJsonValue("UTInstrument"));
    objInstrument.insert(QString("ChannelParameters"), QJsonValue(arrayChannelPar));

    QJsonObject objBindID1 = objInstrument;
    int probeMin = m_probeNoManager->GetProbeMinLeft();
    int probeMax = m_probeNoManager->GetProbeMaxLeft();
    for (int channelID = probeMin; channelID <= probeMax; )
    {
        objBindID1.insert(QString("ChannelBindSetup%1").arg(channelID), QJsonValue(QString::number(channelID)));
        channelID = m_probeNoManager->GetNextProbeNo(channelID);
        if(channelID == INVALID_VALUE)
        {
            break;
        }
    }

    QJsonObject objBindID2 = objInstrument;
    probeMin = m_probeNoManager->GetProbeMinRight();
    probeMax = m_probeNoManager->GetProbeMaxRight();
    for (int channelID = probeMin; channelID <= probeMax; )
    {
        objBindID2.insert(QString("ChannelBindSetup%1").arg(channelID), QJsonValue(QString::number(channelID)));
        channelID = m_probeNoManager->GetNextProbeNo(channelID);
        if(channelID == INVALID_VALUE)
        {
            break;
        }
    }

    if (GetFileType() == LowerComputer1)
    {
        objBindID1.insert(QString("ID"), QJsonValue("1"));
        objBindID1.insert(QString("Name"), QJsonValue("LowerComputer"));
        arrayInstrument.append(objBindID1);
    }
    else if (GetFileType() == LowerComputer2)
    {
        objBindID2.insert(QString("ID"), QJsonValue("1"));
        objBindID2.insert(QString("Name"), QJsonValue("LowerComputer"));
        arrayInstrument.append(objBindID2);
    }
    else
    {
        objBindID1.insert(QString("ID"), QJsonValue("1"));
        objBindID1.insert(QString("IP"), QJsonValue("192.168.1.89"));
        objBindID1.insert(QString("Name"), QJsonValue("LowerComputer1"));
        objBindID2.insert(QString("ID"), QJsonValue("2"));
        objBindID2.insert(QString("IP"), QJsonValue("192.168.1.150"));
        objBindID2.insert(QString("Name"), QJsonValue("LowerComputer2"));
        arrayInstrument.append(objBindID1);
        arrayInstrument.append(objBindID2);
    }

    QJsonDocument jsonDoc(arrayInstrument);
    QString path(filePath);
    QFile file(path.append(fileNameInstrument));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QByteArray jsonStr = jsonDoc.toJson(QJsonDocument::Indented);   /* 根据Qt文档，将JsonDocument转为字符串的过程不会失败 */
        qDebug() << QString("Writing json to config file %1").arg(path);
        file.write(jsonStr);
        file.close();
    }
    else
    {
        qCritical() << QString("Failed open config file %1: %2").arg(path, file.errorString());
    }
}

void UTSetupToJson::InitUI()
{

}

void UTSetupToJson::Connect()
{
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(DoJsonToFileUTSetup()));
    connect(ui->channelToJson, SIGNAL(clicked()), this, SLOT(DoJsonToFileChannelList()));
    connect(ui->InstrumentToJson, SIGNAL(clicked()), this, SLOT(DoJsonToFileUTInstrument()));
}

UTSetupToJson::FileType UTSetupToJson::GetFileType()
{
    if (ui->checkBox10000->isChecked() && !ui->checkBox20000->isChecked())
    {
        return LowerComputer1;
    }
    else if (!ui->checkBox10000->isChecked() && ui->checkBox20000->isChecked())
    {
        return LowerComputer2;
    }
    else if (ui->checkBox10000->isChecked() && ui->checkBox20000->isChecked())
    {
        return UpperComputer;
    }
    else
    {
        return None;
    }
}

QJsonArray UTSetupToJson::MakeArray(int start, int end)
{
    QJsonArray array;
    for (int index = start; index <= end; ++index)
    {
        QJsonObject obj;
        obj.insert(QString("Name"), QString::number(index));
        array.append(obj);
    }

    return array;
}

/*!
 * \brief 设置探头接线
 * \param nProbeNo
 * \return
 */
void UTSetupToJson::SetProbeConnect(qint32 nStart)
{
    Q_UNUSED(nStart);
    return;
}

/*!
 * \brief 设置探头接线--双口
 * \param nProbeNo
 * \return
 */
void UTSetupToJson::SetProbeConnectDouble(ProbeNoInfo::Track track)
{
    qint32 nLR = int(track);  // 左右轨道

    qint32 nAecChannel = 0;
#if PROBE_SENSOR
    // 传感器
    SetProbeSensor(nLR, nAecChannel);
#endif
    // 轮辐径向 正16度
//    SetProbeArmRadialPositive(nLR, nAecChannel);
    int aecIdxOld = nAecChannel;
    SetProbeArmRimAxial(nLR, nAecChannel);                                                          // 轮辋轮辐轴向
    nAecChannel = aecIdxOld;                                                                        //轴向直探头和轮缘探头并用AEC卡
    SetProbeFlangeRadial(nLR, nAecChannel);                                                         // 轮缘径向
    SetProbe21Positive(nLR, nAecChannel);
    SetProbe21PositiveAndNegative(nLR, nAecChannel);
    SetProbe21Negative(nLR, nAecChannel);
    // 轮辋径向
//    SetProbeRimRadial(nLR, nAecChannel);
    // 轮辐径向 负16度
//    SetProbeArmRadialNegative(nLR, nAecChannel);

    return;
}

/*!
 * \brief 轮辋轮辐轴向、踏面探头接线
 * \param nLR   左右轨
 * \param nAecIdx   AEC卡编号
 * \return AEC卡编号
 */
void UTSetupToJson::SetProbeArmRimAxial(int nLR, int& nAecIdx)
{
    int nProbeStart = 0;
    if(nLR == 0)
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoLeft()->axialMin();
    }
    else
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoRight()->axialMin();
    }

    int nSection = 0;        // 检测区域里第几段(2米1段)
    int nBegin = 0;          // 探头起始
    int iStart = 0;          // 第几个AEC硬通道
    int iNum = 2;            // 配置多少个硬通道
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道2-3
    nBegin = 96;
    iStart = 2;
    iNum = 2;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecIdx, 6, 2);
    nAecIdx++;

    // 下一块AEC卡
    // 硬通道0-1
    nBegin = 32;
    iStart = 0;
    iNum = 2;
    nSection = 1;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道2-3
    nBegin = 128;
    iStart = 2;
    iNum = 2;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecIdx, 6, 2);
    nAecIdx++;

    // 下一块AEC卡
    // 硬通道0-1
    nBegin = 64;
    iStart = 0;
    iNum = 2;
    nSection = 2;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道2-3
    nBegin = 160;
    iStart = 2;
    iNum = 2;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecIdx, 6, 2);
    nAecIdx++;
}

/*!
 * \brief 设置轮缘径向探头接线
 * \param nLR   左右轨
 * \param nAecIdx   AEC卡编号
 * \return AEC卡编号
 */
void UTSetupToJson::SetProbeFlangeRadial(int nLR, int& nAecIdx)
{
    int nProbeStart = 0;
    if(nLR == 0)
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoLeft()->radialFlangeMin();
    }
    else
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoRight()->radialFlangeMin();
    }
    // 轮缘径向
    // 硬通道0-3
    int nBegin = 0;
    int iStart = 4;
    int iNum = 4;
    int nSection = 0;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecIdx, 6, 2);
    nAecIdx++;
    // 下一块AEC卡
    // 硬通道0-3
    nBegin = 64;
    iStart = 4;
    iNum = 4;
    nSection = 1;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecIdx, 6, 2);
    nAecIdx++;

    // 硬通道0-3
    nBegin = 128;
    iStart = 4;
    iNum = 4;
    nSection = 2;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecIdx, 6, 2);
    nAecIdx++;
}

/*!
 * \brief 设置轮辋径向探头接线,正负24度
 * \param nLR   左右轨
 * \param nAecIdx   AEC卡编号
 * \return AEC卡编号
 */
/*
void UTSetupToJson::SetProbeRimRadial(int nLR, int& nAecIdx)
{
    int nProbeStart = 0;
    if(nLR == 0)
    {
        nProbeStart = LEFT_RIM_RADIAL_MIN;
    }
    else
    {
        nProbeStart = RIGHT_RIM_RADIAL_MIN;
    }
    // 第5、6块AEC卡
    int nBegin = 3;
    int iStart = 0;
    int iNum = 2;
    int nSection = 4;
    int jStart = 1; // 从第2行开始
    // 通道0-1
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart, jStart);
    // 通道2-3
    nBegin = 67;
    iStart = 2;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart, jStart);
    // 通道4-5
    nBegin = 133;
    iStart = 4;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart, jStart);
    // 通道6-7
    nBegin = 193;
    iStart = 6;
    jStart = 0;     // 从第1行开始
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart, jStart);

    qint32 nPorbeNo =  nProbeStart;
    AecChannel[nLR][nAecIdx][0][0][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 1;
    AecChannel[nLR][nAecIdx][0][0][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 64;
    AecChannel[nLR][nAecIdx][1][0][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 2;
    AecChannel[nLR][nAecIdx][1][0][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 128;
    AecChannel[nLR][nAecIdx][2][0][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 65;
    AecChannel[nLR][nAecIdx][2][0][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 129;
    AecChannel[nLR][nAecIdx][3][0][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 66;
    AecChannel[nLR][nAecIdx][3][0][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 130;
    AecChannel[nLR][nAecIdx][4][0][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 131;
    AecChannel[nLR][nAecIdx][4][0][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 192;
    AecChannel[nLR][nAecIdx][5][0][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 0, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 132;
    AecChannel[nLR][nAecIdx][5][0][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 0, nSection, nPorbeNo);
    nAecIdx++;
    // 下一块AEC卡
    // 通道0-1
    nBegin = 31;
    iStart = 0;
    nSection = 5;
    jStart = 0;
    int jEnd = PER_TX_CARD_CHN_NUM;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart, jStart, jEnd);
    // 通道2-3
    nBegin = 95;
    iStart = 2;
    jEnd = PER_TX_CARD_CHN_NUM - 1;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart, jStart, jEnd);
    // 通道4-5
    nBegin = 161;
    iStart = 4;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart, jStart, jEnd);
    // 通道6-7
    nBegin = 225;
    iStart = 6;
    SetProbeNo(true, nLR, nAecIdx, nBegin, iStart, iNum, nSection, nProbeStart, jStart, jEnd);
    //最后一排无规律
    nPorbeNo = nProbeStart + 123;
    AecChannel[nLR][nAecIdx][2][7][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 63;
    AecChannel[nLR][nAecIdx][2][7][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 124;
    AecChannel[nLR][nAecIdx][3][7][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 125;
    AecChannel[nLR][nAecIdx][3][7][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 189;
    AecChannel[nLR][nAecIdx][4][7][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 126;
    AecChannel[nLR][nAecIdx][4][7][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 190;
    AecChannel[nLR][nAecIdx][5][7][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 127;
    AecChannel[nLR][nAecIdx][5][7][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 253;
    AecChannel[nLR][nAecIdx][6][7][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 191;
    AecChannel[nLR][nAecIdx][6][7][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 254;
    AecChannel[nLR][nAecIdx][7][7][0] = nPorbeNo;
    InsertMapEyeDouble(true, 0, 7, nSection, nPorbeNo);
    nPorbeNo = nProbeStart + 255;
    AecChannel[nLR][nAecIdx][7][7][1] = nPorbeNo;
    InsertMapEyeDouble(true, 1, 7, nSection, nPorbeNo);
    nAecIdx++;
}
*/
/*!
 * \brief 设置轮辐径向正16度探头接线
 * \param nLR   左右轨
 * \param nAecIdx   AEC卡编号
 */
/*
void UTSetupToJson::SetProbeArmRadialPositive(int nLR, int& nAecChannel)
{
    int nProbeStart = 0;
    if(nLR == 0)
    {
        nProbeStart = LEFT_ARM_RADIAL_MIN;
    }
    else
    {
        nProbeStart = RIGHT_ARM_RADIAL_MIN;
    }

    int nBegin = 0;
    int iStart = 0;
    int iNum = 3;
    int nSection = 0;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道3-7
//    SetProbeNull(nLR, nAecChannel, 3, 5);
    nAecChannel++;
    // 下一块AEC卡
    // 硬通道0-2
    nBegin = 48;
    iStart = 0;
    iNum = 3;
    nSection = 1;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道3-7
//    SetProbeNull(nLR, nAecChannel, 3, 5);
    nAecChannel++;
}*/

/*!
 * \brief 设置轮辋轮辐径向正21度
 * \param nLR   左右轨
 * \param nAecIdx   AEC卡编号
 */

void UTSetupToJson::SetProbe21Positive(int nLR, int& nAecChannel)
{
    int nProbeStart = 0;
    if(nLR == 0)
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoLeft()->radialPositiveMin();
    }
    else
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoRight()->radialPositiveMin();
    }
    // 第5、6块AEC卡
    // 硬通道0-2
    int nBegin = 0;
    int iStart = 0;
    int iNum = 3;
    int nSection = 3;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道3-5
    nBegin = 120;
    iStart = 3;
    iNum = 3;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecChannel, 6, 2);
    nAecChannel++;
    // 下一块AEC卡
    // 硬通道0-2
    nBegin = 48;
    iStart = 0;
    iNum = 3;
    nSection = 4;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道3-5
    nBegin = 168;
    iStart = 3;
    iNum = 3;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecChannel, 6, 2);
    nAecChannel++;
}

/*!
 * \brief 设置轮辋轮辐径向正负21度（共用一组16个传感器）
 * \param nLR   左右轨
 * \param nAecIdx   AEC卡编号
 */
void UTSetupToJson::SetProbe21PositiveAndNegative(int nLR, int& nAecChannel)
{
    int nProbeStart = 0;
    if(nLR == 0)
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoLeft()->radialPositiveMin();
    }
    else
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoRight()->radialPositiveMin();
    }
    // 第7块AEC卡
    // 硬通道0-2
    int nBegin = 96;
    int iStart = 0;
    int iNum = 3;
    int nSection = 5;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart, 0, 4);
    // 硬通道3-5
    nBegin = 216;
    iStart = 3;
    iNum = 3;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart, 0, 4);

    if(nLR == 0)
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoLeft()->radialNegativeMin();
    }
    else
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoRight()->radialNegativeMin();
    }
    // 第7块AEC卡
    // 硬通道0-2
    nBegin = 0;
    iStart = 0;
    iNum = 3;
    nSection = 5;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart, 4, 8);
    // 硬通道3-5
    nBegin = 120;
    iStart = 3;
    iNum = 3;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart, 4, 8);
    nAecChannel++;
}

/*!
 * \brief 设置轮辋轮辐径向负21度
 * \param nLR   左右轨
 * \param nAecIdx   AEC卡编号
 */

void UTSetupToJson::SetProbe21Negative(int nLR, int& nAecChannel)
{
    int nProbeStart = 0;
    if(nLR == 0)
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoLeft()->radialNegativeMin();
    }
    else
    {
        nProbeStart = m_probeNoManager->GetProbeNoInfoRight()->radialNegativeMin();
    }
    // 第8、9块AEC卡
    // 硬通道0-2
    int nBegin = 24;
    int iStart = 0;
    int iNum = 3;
    int nSection = 6;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道3-5
    nBegin = 144;
    iStart = 3;
    iNum = 3;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecChannel, 6, 2);
    nAecChannel++;
    // 下一块AEC卡
    // 硬通道0-2
    nBegin = 72;
    iStart = 0;
    iNum = 3;
    nSection = 7;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道3-5
    nBegin = 192;
    iStart = 3;
    iNum = 3;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道6-7
//    SetProbeNull(nLR, nAecChannel, 6, 2);
    nAecChannel++;
}

/*!
 * \brief 设置轮辐径向负16度探头接线
 * \param nLR   左右轨
 * \param nAecIdx   AEC卡编号
 * \return AEC卡编号
 */
/*
void UTSetupToJson::SetProbeArmRadialNegative(int nLR, int& nAecChannel)
{
    int nProbeStart = 0;
    if(nLR == 0)
    {
        nProbeStart = LEFT_ARM_RADIAL_MIN;
    }
    else
    {
        nProbeStart = RIGHT_ARM_RADIAL_MIN;
    }
    // 硬通道0-2
    int nBegin = 96;
    int iStart = 0;
    int iNum = 3;
    int nSection = 6;
    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道3-7
//    SetProbeNull(nLR, nAecChannel, 3, 5);
    nAecChannel++;
    // 下一块AEC卡
    // 硬通道0-2
    nBegin = 144;
    iStart = 0;
    iNum = 3;
    nSection = 7;
//    SetProbeNo(true, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
    // 硬通道3-7
    SetProbeNull(nLR, nAecChannel, 3, 5);
    nAecChannel++;
}
*/
/*!
 * \brief 设置传感器探头接线
 * \param nLR   左右轨
 * \param nAecIdx   AEC卡编号
 * \return AEC卡编号
 */
void UTSetupToJson::SetProbeSensor(int nLR, int& nAecChannel)
{
    int nProbeMin = 0;
    int nProbeMax = 0;
    if(nLR == 0)
    {
        nProbeMin = m_probeNoManager->GetProbeNoInfoLeft()->sensorMin();
        nProbeMax = m_probeNoManager->GetProbeNoInfoLeft()->sensorMax();
    }
    else
    {
        nProbeMin = m_probeNoManager->GetProbeNoInfoRight()->sensorMin();
        nProbeMax = m_probeNoManager->GetProbeNoInfoRight()->sensorMax();
    }

    int nSensorCardNum = (nProbeMax - nProbeMin + 1) / 64 + 1;
    int iNum = 1;
    for(int j = 0; j < nSensorCardNum; ++j)
    {
        int nProbeStart = nProbeMin + 64 * j;
        for(int i = 0; i < 8; i++)
        {
            int nBegin = i * 8;
            int iStart = i;
            int nSection = -1;
            SetProbeNoSingle(false, nLR, nAecChannel, nBegin, iStart, iNum, nSection, nProbeStart);
        }
        nAecChannel++;
    }
}

/*!
 * \brief 设置探头号
 * \param bEyeFlag 是否有光眼编号（传感器探头无光眼编号）
 * \param nLR 左右轨
 * \param nAecIdx AEC卡编号
 * \param nBegin 探头偏移号
 * \param iStart 前置卡起始编号（0-7）
 * \param iNum 要配置的前置卡数量
 * \param nDectectArea 检测区域（0-3 正16度 直探头和轮缘、正负24度、负16度）
 * \param nSection 检测区域中的哪一段（16个一段）
 * \param nProbeStart 探头起始编号
 * \param jStart AEC卡硬通道起始（0-7）
 * \param jEnd AEC卡硬通道结束（0-7）
 * \param iNumReal 要配置的前置卡是否接在同一块AEC卡上（1：1接线方式（AEC通道都接满）时，iNum不会所有都在同一块AEC卡上）
 * \return
 */
void UTSetupToJson::SetProbeNo(bool bEyeFlag, int nLR, int nAecIdx, int nBegin,
                               int iStart, int iNum, int nSection,
                               int nProbeStart, int jStart, int jEnd, int iNumReal)
{
    int iEnd = iNum;
    if(iNumReal != 0)
    {
        iEnd = iNumReal;
    }

    for(int i = iStart; i < iStart + iEnd; i++)
    {
        for(int j = jStart; j < jEnd; j++)
        {
            for(int k = 0; k < PER_TX_CHN_PROBE_NUM; k++)
            {
                qint32 nValue = iNum * k + nBegin + (i - iStart) + (j - jStart) * iNum * 2 + nProbeStart;
                AecChannel[nLR][nAecIdx][i][j][k] = nValue;
                if(bEyeFlag)
                {
                    InsertMapEyeDouble(bEyeFlag, k, j, nSection, nValue);
                }
            }
        }
    }
}

void UTSetupToJson::SetProbeNoSingle(bool bEyeFlag, int nLR, int nAecChannel, int nBegin,
                                     int iStart, int iNum, int nSection,
                                     int nProbeStart, int jStart, int jEnd, int iNumReal)
{
    int iEnd = iNum;
    if(iNumReal != 0)
    {
        iEnd = iNumReal;
    }

    for(int i = iStart; i < iStart + iEnd; i++)
    {
        for(int j = jStart; j < jEnd; j++)
        {
            for(int k = 0; k < PER_TX_CHN_PROBE_NUM; k++)
            {
                qint32 nValue = -1;
                if(k == 0)
                {
                    nValue = iNum * k + nBegin + (i - iStart) + (j - jStart) * iNum + nProbeStart;
                }
                AecChannel[nLR][nAecChannel][i][j][k] = nValue;
                if(bEyeFlag)
                {
                    InsertMapEyeSingle(bEyeFlag, k, j, nSection, nValue);
                }
            }
        }
    }
}

void UTSetupToJson::SetProbeNull(int nLR, int nAecChannel, int iStart, int iNum)
{
    for(int i = iStart; i < iStart + iNum; i++)
    {
        for(int j = 0; j < PER_TX_CARD_CHN_NUM; j++)
        {
            for(int k = 0; k < PER_TX_CHN_PROBE_NUM; k++)
            {
                AecChannel[nLR][nAecChannel][i][j][k] = -1;
            }
        }
    }
}

/*!
 * \brief UTSetupToJson::GenProbeNoInfoJson
 * \breif 生成探头编号JSON文件
 */

void UTSetupToJson::LoadProbeNoInfoJsonFile()
{
    QFile file("probeNoInfo.json");
    if (file.open(QIODevice::ReadOnly))
    {
        if (file.size() <= 0)
        {
            file.close();
            return;
        }
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &err);
        if (err.error == QJsonParseError::NoError)
        {
            if (jsonDoc.isObject())
            {
                QJsonObject obj = jsonDoc.object();
                ui->axialMax->setValue(obj["axialMax"].toInt());
                ui->axialMin->setValue(obj["axialMin"].toInt());
                ui->axialOutSideMax->setValue(obj["axialOutSideMax"].toInt());
                ui->axialOutSideMin->setValue(obj["axialOutSideMin"].toInt());
                ui->axialInSideMax->setValue(obj["axialInSideMax"].toInt());
                ui->axialInSideMin->setValue(obj["axialInSideMin"].toInt());

                ui->radialFlangeMax->setValue(obj["radialFlangeMax"].toInt());
                ui->radialFlangeMin->setValue(obj["radialFlangeMin"].toInt());

                ui->radialPositiveMax->setValue(obj["radialPositiveMax"].toInt());
                ui->radialPositiveMin->setValue(obj["radialPositiveMin"].toInt());
                ui->radialPositiveOutSideMax->setValue(obj["radialPositiveOutSideMax"].toInt());
                ui->radialPositiveOutSideMin->setValue(obj["radialPositiveOutSideMin"].toInt());
                ui->radialPositiveInSideMax->setValue(obj["radialPositiveInSideMax"].toInt());
                ui->radialPositiveInSideMin->setValue(obj["radialPositiveInSideMin"].toInt());

                ui->radialNegativeMax->setValue(obj["radialNegativeMax"].toInt());
                ui->radialNegativeMin->setValue(obj["radialNegativeMin"].toInt());
                ui->radialNegativeOutSideMax->setValue(obj["radialNegativeOutSideMax"].toInt());
                ui->radialNegativeOutSideMin->setValue(obj["radialNegativeOutSideMin"].toInt());
                ui->radialNegativeInSideMax->setValue(obj["radialNegativeInSideMax"].toInt());
                ui->radialNegativeInSideMin->setValue(obj["radialNegativeInSideMin"].toInt());

                ui->sensorMax->setValue(obj["sensorMax"].toInt());
                ui->sensorMin->setValue(obj["sensorMin"].toInt());

            }
            file.close();
        }
    } else {
        file.close();
        qDebug() << "Open probeNoInfo.json file failed:";
    }
}

void UTSetupToJson::GenProbeNoInfoJson()
{
    QJsonObject obj;
    obj["axialMax"] = ui->axialMax->value();
    obj["axialMin"] = ui->axialMin->value();
    obj["axialOutSideMax"] = ui->axialOutSideMax->value();
    obj["axialOutSideMin"] = ui->axialOutSideMin->value();
    obj["axialInSideMax"] = ui->axialInSideMax->value();
    obj["axialInSideMin"] = ui->axialInSideMin->value();

    obj["radialFlangeMax"] = ui->radialFlangeMax->value();
    obj["radialFlangeMin"] = ui->radialFlangeMin->value();

    obj["radialPositiveMax"] = ui->radialPositiveMax->value();
    obj["radialPositiveMin"] = ui->radialPositiveMin->value();
    obj["radialPositiveOutSideMax"] = ui->radialPositiveOutSideMax->value();
    obj["radialPositiveOutSideMin"] = ui->radialPositiveOutSideMin->value();
    obj["radialPositiveInSideMax"] = ui->radialPositiveInSideMax->value();
    obj["radialPositiveInSideMin"] = ui->radialPositiveInSideMin->value();

    obj["radialNegativeMax"] = ui->radialNegativeMax->value();
    obj["radialNegativeMin"] = ui->radialNegativeMin->value();
    obj["radialNegativeOutSideMax"] = ui->radialNegativeOutSideMax->value();
    obj["radialNegativeOutSideMin"] = ui->radialNegativeOutSideMin->value();
    obj["radialNegativeInSideMax"] = ui->radialNegativeInSideMax->value();
    obj["radialNegativeInSideMin"] = ui->radialNegativeInSideMin->value();

    obj["sensorMax"] = ui->sensorMax->value();
    obj["sensorMin"] = ui->sensorMin->value();

    QJsonDocument jsonDoc(obj);
    SaveJsonConfig(jsonDoc, QJsonValue(), filePath + "probeNoInfo.json");
}

/*!
 * \brief 生成探头接线的JSON文件
 * \param nProbeNo
 * \return
 */
void UTSetupToJson::GenProbeConnectJson()
{
    FileType fileType = GetFileType();
    ProbeNoInfo::Track track = ProbeNoInfo::TrackLeft;
    if (fileType == LowerComputer1)
    {
        ;
    }
    else if (fileType == LowerComputer2)
    {
        track = ProbeNoInfo::TrackRight;
    }
    else
    {
        QString str = "failed! please choose 10000 or 20000";
        QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    qint32 nLR = int(track);
    qint32 nStart = m_probeNoManager->GetTrackStartProbeNo(track);


/*   for(qint32 i = 0; i < AEC_CARD_NUM; i++)
   {
        for(qint32 j = 0; j < PER_AEC_CHN_NUM; j++)
        {
            for(qint32 k = 0; k < PER_TX_CARD_CHN_NUM; k++)
            {
                 for(int m = 0; m < PER_TX_CHN_PROBE_NUM; m++)
                 {
                    m_mapProbeAecChn.insert(AecChannel[i][j][k][m],
                                          10*i+PER_AEC_CHN_NUM);
                 }
            }
        }
    }*/
    // 根据探头数组写入JSON文件
    QJsonArray arrayPXI;
    for(qint32 i = 0; i < AEC_CARD_NUM; i++)
    {
        QJsonObject objPXI;
        QJsonArray arrayOnAecCard;      // 一块AEC卡
        for(qint32 j = 0; j < PER_AEC_CHN_NUM; j++)
        {
            QJsonObject objOneAecChn;   // 一个AEC硬通道
            objOneAecChn["AecChannelNo"] = j;

            QJsonArray arrayOneTxCard;
            for(qint32 k = 0; k < PER_TX_CARD_CHN_NUM; k++)
            {
                QJsonObject objOneTxChn;    //一个前置通道
                objOneTxChn["TxChannelNo"] = k;
                qint32 nValue1 = AecChannel[nLR][i][j][k][0];
                objOneTxChn["first"] = nValue1;
              /*  if(m_mapEye.contains(nValue1))
                {
                    objOneTxChn["firstEyeArea"] = m_mapEye.value(nValue1);
                }
                else
                {
                    objOneTxChn["firstEyeArea"] = -1;
                }*/

                qint32 nValue2 = AecChannel[nLR][i][j][k][1];
                objOneTxChn["second"] = nValue2;
              /*  if(m_mapEye.contains(nValue2))
                {
                    objOneTxChn["secondEyeArea"] = m_mapEye.value(nValue2);
                }
                else
                {
                    objOneTxChn["secondEyeArea"] = -1;
                }*/
                arrayOneTxCard.append(objOneTxChn);
            }
            objOneAecChn["TxChannel"] = arrayOneTxCard;
            arrayOnAecCard.append(objOneAecChn);
        }

        objPXI["AecCardNo"] = i;
        objPXI["AecChannel"] = arrayOnAecCard;
        arrayPXI.append(objPXI);
    }

    QJsonObject obj;
    obj["BasicNo"] = nStart;
    obj["Connection"] = arrayPXI;

    QJsonDocument jsonDoc(obj);
    SaveJsonConfig(jsonDoc, QJsonValue(), filePath + m_strProbeConnectFile);

    return;
}

void UTSetupToJson::SaveJsonConfig(const QJsonDocument &jsonDoc, const QJsonValue &path, const QString &defaultPath)
{
    QString configPath = path.toString();
    if (configPath.isEmpty())
    {
        configPath = defaultPath;
    }
    QFile file(configPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QByteArray jsonStr = jsonDoc.toJson(QJsonDocument::Indented);   /* 根据Qt文档，将JsonDocument转为字符串的过程不会失败 */
        qDebug() << QString("Writing json to config file %1").arg(configPath);
        file.write(jsonStr);
    }
    else
    {
        qCritical() << QString("Failed open config file %1: %2").arg(configPath, file.errorString());
    }
}

void UTSetupToJson::on_GenProbeConnect_clicked()
{
    GenProbeConnectJson();
}

/*!
 * \brief 写入探头编号对应的光眼区的映射表
 * \param nTxProbe0Or1 一个前置通道接出来的两个探头中的哪一个
 * \param nAecChnNo 硬通道号
 * \param nDectectArea 检测区域
 * \param nSection  检测区域中的哪一段
 * \param nProbeNo  探头编号
 */
void UTSetupToJson::InsertMapEye(qint32 nTxProbe0Or1, qint32 nAecChnNo, qint32 nDectectArea,
                                 qint32 nSection, qint32 nProbeNo)
{
    if(nTxProbe0Or1 == 0)
    {
        //qint32 nLightEye = nAecChnNo + nSection * METER2_EYE_NUM;
        qint32 nLightEye = nAecChnNo + nSection * SECTION_EYE_NUM + nDectectArea * AREA_EYE_NUM;
        m_mapEye.insert(nProbeNo, nLightEye);
    }
    else
    {
        //qint32 nLightEye = nAecChnNo + nSection * METER2_EYE_NUM + SECTION_EYE_NUM;
         qint32 nLightEye = nAecChnNo + (nSection + 1) * SECTION_EYE_NUM + nDectectArea * AREA_EYE_NUM;
        m_mapEye.insert(nProbeNo, nLightEye);
    }
}

void UTSetupToJson::InsertMapEyeSingle(bool bEyeFlag, qint32 nTxProbe0Or1, qint32 nAecChnNo,
                                 qint32 nSection, qint32 nProbeNo)
{
    if(bEyeFlag)
    {
        if(nTxProbe0Or1 == 0)
        {
            qint32 nLightEye = nAecChnNo + nSection * METER2_EYE_NUM;
            m_mapEye.insert(nProbeNo, nLightEye);
        }
        else
        {
            qint32 nLightEye = nAecChnNo + nSection * METER2_EYE_NUM + SECTION_EYE_NUM;
            m_mapEye.insert(nProbeNo, nLightEye);
        }
    }
    else
    {
       m_mapEye.insert(nProbeNo, -1);
    }
}
/*!
 * \brief 写入探头编号对应的光眼区的映射表
 * \param nTxProbe0Or1 一个前置通道接出来的两个探头中的哪一个
 * \param nAecChnNo 硬通道号
 * \param nSection  哪一段2米区域
 * \param nProbeNo  探头编号
 */
void UTSetupToJson::InsertMapEyeDouble(bool bEyeFlag, qint32 nTxProbe0Or1, qint32 nAecChnNo,
                                       qint32 nSection, qint32 nProbeNo)
{
    if(bEyeFlag)
    {
        qint32 offset = 0;
        if(nSection > 2)
        {
            offset = 2;
        }
        qint32 nLightEye = nAecChnNo * 2 + nTxProbe0Or1 + nSection * METER2_EYE_NUM + offset;

        m_mapEye.insert(nProbeNo, nLightEye);
    }
    else
    {
        m_mapEye.insert(nProbeNo, -1);
    }
}

/*!
 * \brief 生成探头所在光眼区域的JSON文件
 * \param nProbeNo
 * \return
 */
void UTSetupToJson::GenProbeEyeAreaJson()
{
    // 根据探头数组写入JSON文件
    QJsonArray array;
    qint32 nStart = 0;
    qint32 nEnd = 0;
    if (GetFileType() == LowerComputer1)
    {
        nStart = 0;
        nEnd = 1;
    }
    else if (GetFileType() == LowerComputer2)
    {
        nStart = 1;
        nEnd = 2;
    }
    else
    {
        nStart = 0;
        nEnd = 2;
    }

    for(qint32 nLR = nStart; nLR < nEnd; nLR++)
    {
        for(qint32 i = 0; i < AEC_CARD_NUM; i++)
        {
            for(qint32 j = 0; j < PER_AEC_CHN_NUM; j++)
            {
                for(qint32 k = 0; k < PER_TX_CARD_CHN_NUM; k++)
                {
                    for(qint32 m = 0; m < PER_TX_CHN_PROBE_NUM; m++)
                    {
                        QJsonObject obj;
                        qint32 nProbeNo = AecChannel[nLR][i][j][k][m];
                        if(nProbeNo > 0)
                        {
                            if(m_mapEye.contains(nProbeNo))
                            {
                                obj["ProbeNo"] = nProbeNo;
                                obj["EyeArea"] = m_mapEye.value(nProbeNo);
                                array.append(obj);
                            }
                        }
                    }
                }
            }
        }
    }

    QJsonObject obj;
    obj["ProbeEyeArea"] = array;
    QJsonDocument jsonDoc(obj);
    SaveJsonConfig(jsonDoc, QJsonValue(), filePath + "ProbeEyeArea.json");

    return;
}

void UTSetupToJson::on_GenProbeEyeArea_clicked()
{
    GenProbeEyeAreaJson();
    return;
}

void UTSetupToJson::on_GenProbeNoInfoJson_clicked()
{
    GenProbeNoInfoJson();
}
