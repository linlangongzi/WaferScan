#include "const.h"
#include "CardWorkBase.h"
#include <QDebug>

CardWorkBase::CardWorkBase(QObject *parent) :
    ThreadWork(parent),
    m_nStatus(INIT)
{
    m_pParam = SharedCardParam::create();
    m_probeNoManager = sharedProbeNoManager::create();
    ConnectTo();
}

void CardWorkBase::ConnectTo()
{
    return;
}

/*!
 * \brief CardWorkBase::卡处理槽函数，响应来自AECRunningWork循环发过来的信号
 */
void CardWorkBase::Run()
{
    if(!m_pParam.isNull() && m_pParam->hDev) {
        switch(m_nStatus)
        {
            case INIT:
            {
                if (InitCard()) {
                    Start();
                }
                break;
            }
            case START:
            {
                m_nStatus = READING;
                break;
            }
            case STOP:
            {
                bool bRet = CardStop();
                if(bRet) {
                    m_nStatus = NONE;
                }
                break;
            }
            case READING:
            {
                CardReading();
                break;
            }
            default:
                break;
        }
    }
}

void CardWorkBase::Init()
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    m_nStatus = INIT;
}

void CardWorkBase::Start()
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    m_nStatus = START;
    return;
}

void CardWorkBase::Stop()
{
    QMutexLocker lock(&mutex);
    Q_UNUSED(lock);
    m_nStatus = STOP;
    return;
}

/*!
 * \brief CardWorkBase::初始化AEC卡
 * \param pParam
 * \return
 */
bool CardWorkBase::InitCard()
{
    return false;
}

/*!
 * \brief CardWorkBase::AECCardStart
 * \param pParam
 * \return 成功返回true,否则返回false
 */
bool CardWorkBase::CardStart()
{
    return true;
}

/*!
 * \brief CardWorkBase::AECCardStop
 * \param pParam
 * \return
 */
bool CardWorkBase::CardStop()
{
    return true;
}

/*!
 * \brief CardWorkBase::根据声程和声速，计算通道采样间隔
 * \param VSound
 * \param Range
 * \param pInterSpace
 */
void CardWorkBase::RangeProcess(float VSound, float Range, unsigned short* pInterSpace)
{
    Q_UNUSED(VSound);

#ifdef PIPE_DETECT
    int nTotal= (int)(Range * 1 * ADFREQUENCY);   // us
#else
    int nTotal= (int)(Range * 2 / 1000.0 / VSound * ADFREQUENCY * 1000000.0); // mm
#endif

    if(nTotal < MONITOR_SIZE) {
        for (int i = 0; i < 512; i++) {
            pInterSpace[i]=0;
        }
    } else {
        int nInter = nTotal / 512;
        int nReset = nTotal % 512;
        int k = 1;
        for (int i = 0; i < 512; i++) {
            if(nInter < 1) {
                nInter = 1;
            }

            pInterSpace[i] = nInter - 1;
        }

        for (int i = 0;i < 512; i++) {
            float temp = i * 1.0 * nReset / 511;
            if(temp >= k ) {
                pInterSpace[i] = pInterSpace[i] + 1;
                k++;
            }
        }
    }
}

/*!
 * \brief CardWorkBase::设置AEC卡参数
 * \param aecParam
 */
void CardWorkBase::SetCardParam(SharedCardParam cardParam)
{
    m_pParam = cardParam;
    return;
}

/*!
 * \brief CardWorkBase::GetCardParam 获取AEC卡斯参数
 * \return aecParam
 */
SharedCardParam CardWorkBase::GetCardParam()
{
    return m_pParam;
}

/*!
 * \brief AECWork::读取AEC卡数据，监视通道和C扫特征数据
 * \param pParam
 * \return
 */
bool CardWorkBase::CardReading()
{
    return true;
}

/*!
 * \brief AECWork::读取C扫特征数据
 * \param pParam
 * \param nCardIndex
 * \return
 */
bool CardWorkBase::ReadFlawResult()
{
    return true;
}

/*!
 * \brief AECWork::通道选择
 * \param nChnNo 探头号
 * \param nSurOrFlaw 伤波或底波
 */
void CardWorkBase::SetSelectChannel(qint32 nChnNo, qint32 nSurOrFlaw)
{
    Q_UNUSED(nChnNo);
    Q_UNUSED(nSurOrFlaw);
    return;
}

void CardWorkBase::OnFinish()
{
    if (!m_pParam.isNull() && m_pParam->hDev) {
        qDebug() << "CardWorkBase::OnFinish() " << m_pParam->index;
    }

    ThreadWork::OnFinish();
    return;
}

bool CardWorkBase::SetPRF(quint16 prf)
{
    Q_UNUSED(prf);
    return false;
}

void CardWorkBase::SetSimulationData(bool isOpen)
{
    Q_UNUSED(isOpen);
}

void CardWorkBase::SetProbeBoxSimulationData(bool isOpen)
{
    Q_UNUSED(isOpen);
}

void CardWorkBase::SetData(quint32 address, quint16 data)
{
    Q_UNUSED(address);
    Q_UNUSED(data);
}

void CardWorkBase::SetDataMap(MFCDataMap &dataMap)
{
    Q_UNUSED(dataMap);
}

void CardWorkBase::SetCoderMark(quint16 mark)
{
    Q_UNUSED(mark);
}

quint16 CardWorkBase::Gain2Data(float Gain)
{
    quint16 nData;

#ifdef PIPE_DETECT
        nData = static_cast<quint16>(Gain / GAIN_MAX * GAIN_DATA_MAX);
#else
        nData = static_cast<quint16>((Gain * 20.0 / 3.0 * AEC_AMP_LINEAR_PARA_A) + 0.5);
#endif

    if (nData < GAIN_DATA_MIN) {
        nData = GAIN_DATA_MIN;
    } else if ( nData > GAIN_DATA_MAX) {
        nData = GAIN_DATA_MAX;
    }

    return nData;
}

/*!
 * \brief CardWorkBase::由声程(mm)和声速计算采样点数
 * \param VSound
 * \param Range
 * \return
 */
quint32 CardWorkBase::Range2Dot(float VSound, float Range)
{
    quint32 value = Range * 2 / 1000.0 / VSound * ADFREQUENCY * 1000000.0;
    return value;
}

/*!
 * \brief CardWorkBase::时间（us）计算采样点数
 * \param fMicroSecond
 * \return
 */
quint16 CardWorkBase::MicroSecond2Dot(float fMicroSecond)
{
    quint16 value = fMicroSecond * 1 * ADFREQUENCY;

    if(value == 0)
    {
        value = 1;
    }

    return value;
}

/*!
 * \brief CardWorkBase::判断选中通道是否在该卡里
 * \param nSelectChn
 * \return
 */
bool CardWorkBase::SelectChnInCard(int nSelectChn)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return false;
    }

    qint32 nChn2CardNo = m_probeNoManager->ProbNo2CardNo(nSelectChn);

    if(nChn2CardNo == m_pParam->index)
    {
        return true;
    }
    else
    {
        return false;
    }
}
