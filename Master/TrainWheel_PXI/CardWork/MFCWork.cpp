#include "MFCWork.h"
#include "HardwareAddress.h"
#include <QByteArray>

MFCWork::MFCWork(QObject *parent) :
    CardWorkBase(parent)
{
}

/*!
 * \brief 设置模拟数据MPB_SIMULATE_DATA_ADDR
 */
void MFCWork::SetSimulationData(UTSharedDataSimulation simulation)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return;
    }

    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR, simulation->usEnable);
    for(int i = 0; i < NS_PXI100::LIGHT_EYE_VAR_NUM; i++)
    {
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_DATA_ADDR + i * 2, simulation->stLightEye.usLightEye[i]);
    }
}

/*!
 * \brief MFCWork::初始化MFC卡
 * \param pParam
 * \return
 */
bool MFCWork::InitCard()
{
    return true;
}

/*!
 * \brief MFCWork::读取卡数据，火车进入和离开信号
 * \param pParam
 * \return
 */
bool MFCWork::CardReading()
{
    emit sendMfcData(readMfcData());
    return true;
}

QByteArray MFCWork::readMfcData()
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return QByteArray();
    }

    QByteArray mfcData;
    mfcData.resize(MFC_DATA_BYTE_SIZE); // mfc关键字表 有效数据 末尾地址0x110
    P9054_DMAReadWriteBlock(m_pParam->hDev, 0x0, mfcData.data(), MFC_DATA_BYTE_SIZE, TRUE, P9054_MODE_BYTE, P9054_DMA_CHANNEL_0);
    return mfcData;
}
