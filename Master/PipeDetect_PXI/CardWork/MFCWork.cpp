#include "MFCWork.h"
#include "HardwareAddress.h"

MFCWork::MFCWork(QObject *parent) :
    CardWorkBase(parent)
{
}

/*!
 * \brief 设置模拟数据MPB_SIMULATE_DATA_ADDR
 * true:开启模拟调试  false:关闭模拟调试
 */
void MFCWork::SetSimulationData(bool isOpen)
{
    if (!m_pParam->hDev) {
        return;
    }

    if (isOpen) {
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR, 1);             // 设置b0位：0关闭模拟调试；1开启模拟调试
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR + 0x2, 0);        // b2
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR + 0x2, 0xFFFF);   // b2
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR + 0x4, 0);        // b4
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR + 0x4, 0xFFFF);   // b4
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR + 0x6, 0);        // b6
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR + 0x6, 0xFFFF);   // b6
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR + 0x8, 0);        // b8
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR + 0x8, 0xFFFF);   // b8
    } else {
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MPB_SIMULATE_ENABL_ADDR, 0);             // 设置b0位：0关闭模拟调试；1开启模拟调试
    }
}

/*!
 * \brief MFCWork::SetPRF
 * \param prf 重复频率
 * 设置重复频率
 */
bool MFCWork::SetPRF(quint16 prf)
{
    if (prf < 100) {
        prf = 100;
    } else if (prf > 5000) {
        prf = 5000;
    }

    quint16 prfData = (33000000 / prf) & 0xFFFF;
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, PULSE_REPETITION_FREQUENCY_ADDRESS, prfData);   // b4
    return true;
}

void MFCWork::SetData(quint32 address, quint16 data)
{
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, address, data);
}

void MFCWork::SetDataMap(MFCDataMap &dataMap)
{
    for (MFCDataMap::iterator it = dataMap.begin(); it != dataMap.end(); ++it) {
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, it.key(), it.value());
    }
}

void MFCWork::SetCoderMark(quint16 mark)
{
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, DODER_SELECT_MARK, mark);
}

bool MFCWork::CardReading()
{
    quint16 startFlag = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, TEST_START_FLAG);
    emit SendStartFlag(startFlag);
    return true;
}
