#include "ProbeConnectManager.h"
#include <QJsonObject>
#include <QJsonArray>
#include "ConfigObject/JsonFile.h"
#include <QDebug>

const int ProbeConnectManager::MAX_TX_CHN_NUM = 8;

ProbeConnectManager::ProbeConnectManager(QObject *parent) :
    QObject(parent),
    m_strProbeConnectFile("./ProbeConnect"),
    m_nBasicNo(10000),
    m_strEyeAreaFile("./ProbeEyeArea")
{
    InitMapProbeAecChn();
    InitMapEyeArea();
    InitMultiMapEyeArea();
}

/*!
 * \brief 探头号（通道号）转成AEC通道编号
 * \param nProbeNo：探头号
 * \return
 */
qint32 ProbeConnectManager::ProbNo2AecChnNo(qint32 nProbeNo)
{
    if(m_mapProbeAecChn.contains(nProbeNo))
    {
        qint32 nAecChnNo = m_mapProbeAecChn.value(nProbeNo);
        return nAecChnNo;
    }
    else
    {
        qDebug() << "can not find key:" << nProbeNo;
        return -1;
    }
}

/*!
 * \brief 探头号（通道号）转成对应的AEC卡的硬通道号
 * \param nProbeNo：探头号
 * \return
 */
qint32 ProbeConnectManager::ProbNo2CardChnNo(qint32 nProbeNo)
{
    qint32 nAecChnNo = ProbNo2AecChnNo(nProbeNo);

    if(nAecChnNo >= 0)
    {
       qint32 nCardChnNo =  nAecChnNo % 10;
       return nCardChnNo;
    }
    else
    {
        return -1;
    }

}

/*!
 * \brief P探头号（通道号）转成对应的AEC卡号
 * \param nProbeNo：探头号
 * \return
 */
qint32 ProbeConnectManager::ProbNo2CardNo(qint32 nProbeNo)
{
    qint32 nAecChnNo = ProbNo2AecChnNo(nProbeNo);

    if(nAecChnNo >= 0)
    {
        qint32 nCardNo =  nAecChnNo / 10;
        return nCardNo;
    }
    else
    {
        return -1;
    }
}


/*!
 * \brief 初始化P探头号（通道号）与AEC通道编号的映射表
 */
void ProbeConnectManager::InitMapProbeAecChn()
{
    JsonFile jsonFile;
    jsonFile.setFileName(m_strProbeConnectFile);
    jsonFile.open();
    m_nBasicNo = jsonFile.Get("BasicNo").toInt();
    QJsonArray array = jsonFile.Get("Connection").toArray();
    foreach (QJsonValue value, array)   // 遍历所有AEC卡
    {
        QJsonObject obj = value.toObject();
        qint32 nAecCardNo = obj["AecCardNo"].toInt();
        QJsonArray arrayAecChn = obj["AecChannel"].toArray();
        foreach (QJsonValue value, arrayAecChn)     // 遍历所有AEC硬通道
        {
            QJsonObject obj = value.toObject();
             qint32 nAecChnNo = obj["AecChannelNo"].toInt();
             QJsonArray arrayTxChn = obj["TxChannel"].toArray();
             foreach (QJsonValue value, arrayTxChn) // 遍历1个AEC硬通道上接的所有前置通道
             {
                 QJsonObject obj = value.toObject();
                 qint32 nProbeNoFirst = obj["first"].toInt();
                 qint32 nTxChannelNo = obj["TxChannelNo"].toInt();
                 m_mapProbeAecChn.insert(nProbeNoFirst, 10 * nAecCardNo + nAecChnNo);
                 m_mapProbeTxChn0or1.insert(nProbeNoFirst, 0);
                 m_mapProbeTxChn.insert(nProbeNoFirst, nTxChannelNo);
#ifndef PIPE_DETECT
                 qint32 nProbeNoSecond = obj["second"].toInt();
                 m_mapProbeAecChn.insert(nProbeNoSecond, 10 * nAecCardNo + nAecChnNo);
                 m_mapProbeTxChn0or1.insert(nProbeNoSecond, 1);
                 m_mapProbeTxChn.insert(nProbeNoSecond, nTxChannelNo);
#endif
             }
        }
    }

    return;
}

/*!
 * \brief 探头号对应的前置通道的1或2（一个前置通道接2个探头）
 * \param nProbeNo
 * \return
 */
qint32 ProbeConnectManager::ProbNo2TxChn0or1(qint32 nProbeNo)
{
    if(m_mapProbeTxChn0or1.contains(nProbeNo))
    {
        qint32 n0or1 = m_mapProbeTxChn0or1.value(nProbeNo);
        return n0or1;
    }
    else
    {
        qDebug() << "can not find key";
        return -1;
    }
}
/*!
 * \brief 探头号对应前置通道号 0-7
 * \param nProbeNo
 * \return
 */
qint32 ProbeConnectManager::ProbNo2TxChnNo(qint32 nProbeNo)
{
    if(m_mapProbeTxChn.contains(nProbeNo))
    {
        qint32 nTxChn = m_mapProbeTxChn.value(nProbeNo);
        return nTxChn;
    }
    else
    {
        qDebug() << "can not find key";
        return -1;
    }
}

/*!
 * \brief 探头号对应的软通道号
 * \param nProbeNo
 * \return
 */
qint32 ProbeConnectManager::ProbNo2SoftChnNo(qint32 nProbeNo, PUTS_TYPE_e putsType)
{
    qint32 nSoftChnNo = 0;
    qint32 nTxChnNo = ProbNo2TxChnNo(nProbeNo);
    qint32 TxChn0or1 = ProbNo2TxChn0or1(nProbeNo);

    if(nTxChnNo >= 0 && TxChn0or1 >= 0)
    {
        switch(putsType)
        {
            case SINGLE:
                // 从左上到左下 右上到右下
                nSoftChnNo = nTxChnNo + MAX_TX_CHN_NUM * TxChn0or1;
                break;
            case DOUBLE:
                // 左到右 上到下
                nSoftChnNo = nTxChnNo * 2 + TxChn0or1;
                break;
            case HALF_SINGLE:
                if(nTxChnNo > 4)
                {
                    nTxChnNo = nTxChnNo - 4;
                }
                nSoftChnNo = nTxChnNo;
                break;
            default:
                break;
        }

        return nSoftChnNo;
    }
    else
    {
        return -1;
    }
}

/*!
 * \brief 获取探头起始编号 左右轨分为 10000 和 20000
 * \return
 */
qint32 ProbeConnectManager::GetBasicNo()
{
    JsonFile jsonFile;
    jsonFile.setFileName(m_strProbeConnectFile);
    jsonFile.open();
    m_nBasicNo = jsonFile.Get("BasicNo").toInt();

    return m_nBasicNo;
}

/*!
 * \brief
 * \param nProbeNo
 * \return
 */
qint32 ProbeConnectManager::ProbeNo2EyeArea(qint32 nProbeNo)
{
    qint32 nEyeArea = 0;
    if(m_mapEyeArea.contains(nProbeNo))
    {
        nEyeArea = m_mapEyeArea.value(nProbeNo);
    }
    else
    {
        nEyeArea = -1;
        qWarning() << "probeNo is not found";
    }

    return nEyeArea;
}

/*!
 * \brief ProbeConnectManager::EyeArea2ProbeNoList
 * \param area
 * \return 返回一个传感器区域内所有的探头编号list
 */
QList<qint32> ProbeConnectManager::EyeArea2ProbeNoList(qint32 area)
{
    return m_multiMapEyeArea.values(area);
}

/*!
 * \brief 初始化探头号（通道号）与光眼区域的映射表
 */
void ProbeConnectManager::InitMapEyeArea()
{
    JsonFile jsonFile;
    jsonFile.setFileName(m_strEyeAreaFile);
    jsonFile.open();
    QJsonArray array = jsonFile.Get("ProbeEyeArea").toArray();
    qint32 nProbeNo = 0;
    qint32 nEyeArea = 0;
    foreach (QJsonValue value, array)
    {
        QJsonObject obj = value.toObject();
        nProbeNo = obj["ProbeNo"].toInt();
        nEyeArea = obj["EyeArea"].toInt();
        m_mapEyeArea.insert(nProbeNo, nEyeArea);
    }

    return;
}

/*!
 * \brief ProbeConnectManager::InitMultiMapEyeArea
 * 初始化MultiMap，用来存储传感器区域对应的探头编号List
 */
void ProbeConnectManager::InitMultiMapEyeArea()
{
    JsonFile jsonFile;
    jsonFile.setFileName(m_strEyeAreaFile);
    jsonFile.open();
    QJsonArray array = jsonFile.Get("ProbeEyeArea").toArray();
    qint32 nProbeNo = 0;
    qint32 nEyeArea = 0;
    foreach (QJsonValue value, array)
    {
        QJsonObject obj = value.toObject();
        nProbeNo = obj["ProbeNo"].toInt();
        nEyeArea = obj["EyeArea"].toInt();
        m_multiMapEyeArea.insert(nEyeArea, nProbeNo);
    }

    return;
}

QString ProbeConnectManager::GetProbeConnectFileName()
{
    return m_strProbeConnectFile;
}
