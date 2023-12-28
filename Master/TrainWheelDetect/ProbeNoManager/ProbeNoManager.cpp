#include "ProbeNoManager.h"
#include "ProbeNoInfo.h"
#include <allrising/Util.h>
#include <QDebug>

static ProbeNoManager *m_instance = NULL;

ProbeNoManager::ProbeNoManager(QObject *parent) :
    QObject(parent),
    m_strEyeAreaFile("ProbeEyeArea")
{
    m_probeNoInfoLeft = new ProbeNoInfo(this, ProbeNoInfo::TrackLeft);
    m_probeNoInfoRight = new ProbeNoInfo(this, ProbeNoInfo::TrackRight);
    InitMap();

    m_instance = this;
}

void ProbeNoManager::InitMap()
{
    InitMapByProbeNoInfo(m_probeNoInfoLeft);
    InitMapByProbeNoInfo(m_probeNoInfoRight);
    InitMapEyeArea();

    m_ProbeNoList = GetProbeNoList();
}

void ProbeNoManager::InsertTypeMap(qint32 nMin, qint32 qMax, PROBE_DEFECT_TYPE_e type)
{
    for(int i = nMin; i < qMax + 1; ++i)
    {
        m_mapType.insert(i, type);
    }
}

ProbeNoManager::MapType ProbeNoManager::GetProbeTypeMap()
{
    return m_mapType;
}

/*!
 * \brief 由探头序号得出所属的一组探头检测的缺陷类型
 * \param nProbeNo
 * \return
 */
PROBE_DEFECT_TYPE_e ProbeNoManager::ProbeNo2Type(qint32 nProbeNo)
{
    return m_mapType.value(nProbeNo);
}

/*!
 * \brief 由探头序号得出所属的一类缺陷的探头个数
 * \param nProbeNo
 * \return
 */
qint32 ProbeNoManager::ProbeNo2TypeProbeNum(qint32 nProbeNo)
{
    return GetProbeNumByType(ProbeNo2Type(nProbeNo));
}

/*!
 * \brief
 * \param nProbeNo
 * \return
 */
qint32 ProbeNoManager::ProbeNo2EyeArea(qint32 nProbeNo)
{
    qint32 nEyeArea = 0;
    if (m_mapEyeArea.contains(nProbeNo))
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
 * \brief ProbeNoManager::EyeArea2ProbeNoList
 * \param area
 * \return 返回一个传感器区域内所有的探头编号list
 */
ProbeNoManager::ProbeNoList ProbeNoManager::EyeArea2ProbeNoList(qint32 area)
{
    return m_multiMapEyeArea.values(area);
}

/*!
 * \brief ProbeNoManager::GetMinimumProbeNo
 * \param list
 * \return:list中最小探头编号，如果list为空，则返回-1
 */
qint32 ProbeNoManager::GetMinimumProbeNo(ProbeNoManager::ProbeNoList list)
{
    if (list.size())
    {
        qint32 probeNo = list.value(0);
        foreach (auto value, list)
        {
            probeNo = probeNo < value ? probeNo : value;
        }

        return probeNo;
    }

    return INVALID_VALUE;
}

/*!
 * \brief 初始化探头号（通道号）与光眼区域的映射表
 */
void ProbeNoManager::InitMapEyeArea()
{
    const QJsonObject json = ReadFileAsJson(m_strEyeAreaFile + ".json").toObject();
    QJsonArray array = json["ProbeEyeArea"].toArray();
    qint32 nProbeNo = 0;
    qint32 nEyeArea = 0;
    foreach (QJsonValue value, array)
    {
        QJsonObject obj = value.toObject();
        nProbeNo = obj["ProbeNo"].toInt();
        nEyeArea = obj["EyeArea"].toInt();
        m_mapEyeArea.insert(nProbeNo, nEyeArea);
        m_multiMapEyeArea.insert(nEyeArea, nProbeNo);
    }
}

ProbeNoInfo* ProbeNoManager::GetProbeNoInfoLeft()
{
    return m_probeNoInfoLeft;
}

ProbeNoInfo* ProbeNoManager::GetProbeNoInfoRight()
{
    return m_probeNoInfoRight;
}

/*!
 * \brief ProbeNoManager::IsProbeFlange 是否是轮缘探头
 * \param chnNo 探头号
 * \return bool
 */
bool ProbeNoManager::IsProbeFlange(int probeNo)
{
    PROBE_DEFECT_TYPE_e probeType = ProbeNo2Type(probeNo);
    if (probeType == LEFT_FLANGE_RADIAL ||
            probeType == RIGHT_FLANGE_RADIAL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*!
 * \brief ProbeNoManager::IsProbeRadialPositive 是否是21度轮辋轮辐径向
 * \param chnNo 探头号
 * \return bool
 */
bool ProbeNoManager::IsProbeRadialPositive(int probeNo)
{
    PROBE_DEFECT_TYPE_e probeType = ProbeNo2Type(probeNo);
    if (probeType == LEFT_RIM_RADIAL_PLUS1 || probeType == LEFT_RIM_RADIAL_PLUS2
            || probeType == RIGHT_RIM_RADIAL_PLUS1 || probeType == RIGHT_RIM_RADIAL_PLUS2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*!
 * \brief ProbeNoManager::IsProbeSensor 是否传感器探头
 * \param chnNo 探头号
 * \return bool
 */
bool ProbeNoManager::IsProbeSensor(int probeNo)
{
    PROBE_DEFECT_TYPE_e probeType = ProbeNo2Type(probeNo);
    if (probeType == LEFT_SENSOR || probeType == RIGHT_SENSOR)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*!
 * \brief ProbeNoManager::IsProbeStraightOutside 是否直探头外侧
 * \param chnNo 探头号
 * \return bool
 */
bool ProbeNoManager::IsProbeStraightOutside(int probeNo)
{
    PROBE_DEFECT_TYPE_e probeType = ProbeNo2Type(probeNo);
    if(probeType == LEFT_RIM_TREAD ||
            probeType == RIGHT_RIM_TREAD)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*!
 * \brief ProbeNoManager::GetProbeMin 编号最小探头号
 * \return int 探头号
 */
int ProbeNoManager::GetProbeMin()
{
    return m_ProbeNoList.first();
}

/*!
 * \brief ProbeNoManager::GetProbeMax 编号最大探头号
 * \return int 探头号
 */
int ProbeNoManager::GetProbeMax()
{
    return m_ProbeNoList.last();
}

/*!
 * \brief ProbeNoManager::GetProbeNoList 获取探头列表
 * \return ProbeNoList
 */
ProbeNoManager::ProbeNoList ProbeNoManager::GetProbeNoList()
{
    ProbeNoList list;
    for (qint32 value : m_mapType.keys())
    {
        list << value;
    }
    return list;
}

int ProbeNoManager::GetTypeOffsetByTrack(ProbeNoInfo::Track track)
{
    int offset = 0;
    if (track == ProbeNoInfo::TrackRight)
    {
        offset = 8;
    }

    return offset;
}

void ProbeNoManager::InitMapProbeNoType(ProbeNoInfo* info)
{
    int offset = GetTypeOffsetByTrack(info->GetTrack());
    // 轮辋轮辐轴向踏面
    for(int i = 0; i <= LEFT_SENSOR; ++i)
    {
        PROBE_DEFECT_TYPE_e type = PROBE_DEFECT_TYPE_e(i + offset);
        InsertTypeMap(GetProbeMinByType(type), GetProbeMaxByType(type), type);
    }
}

void ProbeNoManager::InitMapTypeProbeMin(ProbeNoInfo* info)
{
    int offset = GetTypeOffsetByTrack(info->GetTrack());

    m_mapTypeProbeMin.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_TREAD + offset),
                             info->axialOutSideMin());
    m_mapTypeProbeMin.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_ARM_TREAD + offset),
                             info->axialInSideMin());

    m_mapTypeProbeMin.insert(PROBE_DEFECT_TYPE_e(LEFT_FLANGE_RADIAL + offset),
                             info->radialFlangeMin());

    m_mapTypeProbeMin.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_RADIAL_PLUS1 + offset),
                             info->radialPositiveOutSideMin());
    m_mapTypeProbeMin.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_RADIAL_PLUS2 + offset),
                             info->radialPositiveInSideMin());
    m_mapTypeProbeMin.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_RADIAL_MINUS1 + offset),
                             info->radialNegativeOutSideMin());
    m_mapTypeProbeMin.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_RADIAL_MINUS2 + offset),
                             info->radialNegativeInSideMin());
    m_mapTypeProbeMin.insert(PROBE_DEFECT_TYPE_e(LEFT_SENSOR + offset),
                             info->sensorMin());
}

void ProbeNoManager::InitMapTypeProbeMax(ProbeNoInfo* info)
{
    int offset = GetTypeOffsetByTrack(info->GetTrack());

    m_mapTypeProbeMax.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_TREAD + offset),
                             info->axialOutSideMax());
    m_mapTypeProbeMax.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_ARM_TREAD + offset),
                             info->axialInSideMax());

    m_mapTypeProbeMax.insert(PROBE_DEFECT_TYPE_e(LEFT_FLANGE_RADIAL + offset),
                             info->radialFlangeMax());

    m_mapTypeProbeMax.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_RADIAL_PLUS1 + offset),
                             info->radialPositiveOutSideMax());
    m_mapTypeProbeMax.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_RADIAL_PLUS2 + offset),
                             info->radialPositiveInSideMax());
    m_mapTypeProbeMax.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_RADIAL_MINUS1 + offset),
                             info->radialNegativeOutSideMax());
    m_mapTypeProbeMax.insert(PROBE_DEFECT_TYPE_e(LEFT_RIM_RADIAL_MINUS2 + offset),
                             info->radialNegativeInSideMax());
    m_mapTypeProbeMax.insert(PROBE_DEFECT_TYPE_e(LEFT_SENSOR + offset),
                             info->sensorMax());
}

ProbeNoManager *ProbeNoManager::instance()
{
    return m_instance;
}

void ProbeNoManager::InitMapByProbeNoInfo(ProbeNoInfo* info)
{
    InitMapTypeProbeMin(info);
    InitMapTypeProbeMax(info);
    InitMapProbeNoType(info);
}

/*!
 * \brief ProbeNoManager::GetNextProbeNo 获取下一个探头号
 * \param probeNo 探头号
 * \return int 探头号
 */
int ProbeNoManager::GetNextProbeNo(int probeNo)
{
    if(m_ProbeNoList.contains(probeNo))
    {
        int index = m_ProbeNoList.indexOf(probeNo);
        if ((index + 1) < m_ProbeNoList.count())
        {
            return m_ProbeNoList.value(index + 1);
        }
        else
        {
            return INVALID_VALUE;
        }
    }
    else
    {
        return INVALID_VALUE;
    }
}

/*! 探头号所在类型中的探头次序 */
int ProbeNoManager::GetIndexInProbeType(int probeNo)
{
    return probeNo - GetProbeMinByType(ProbeNo2Type(probeNo));
}

bool ProbeNoManager::IsProbeValid(int probeNo)
{
    if (probeNo >= GetProbeMin() && probeNo <= GetProbeMax()) {
        return true;
    } else {
        return false;
    }
}

int ProbeNoManager::GetProbeMinLeft()
{
    return m_ProbeNoList.first();
}

int ProbeNoManager::GetProbeMinRight()
{
    int count = m_probeNoInfoLeft->count();
    return m_ProbeNoList.value(count);
}

int ProbeNoManager::GetProbeMaxLeft()
{
    int count = m_probeNoInfoLeft->count();
    return m_ProbeNoList.value(count - 1);
}

int ProbeNoManager::GetProbeMaxRight()
{
    return m_ProbeNoList.last();
}

bool ProbeNoManager::isProbeNoLeft(int probeNo)
{
    return (probeNo / m_probeNoInfoLeft->GetStartProbeNo() == 1);
}

ProbeNoInfo::Track ProbeNoManager::GetTrack(int probeNo)
{
    if(isProbeNoLeft(probeNo))
    {
        return m_probeNoInfoLeft->GetTrack();
    }
    else
    {
        return m_probeNoInfoRight->GetTrack();
    }
}

/*!
 * \brief ProbeNoManager::GetTrackStartProbeNo
 * \brief 获取轨道起始探头号
 * \param track
 * \return int
 */
int ProbeNoManager::GetTrackStartProbeNo(ProbeNoInfo::Track track)
{
    int probeNo = m_probeNoInfoLeft->GetStartProbeNo();
    switch (track) {
    case ProbeNoInfo::TrackLeft:
        break;
    case ProbeNoInfo::TrackRight:
        probeNo = m_probeNoInfoRight->GetStartProbeNo();
        break;
    default:
        break;
    }

    return probeNo;
}

int ProbeNoManager::GetProbeMinByType(PROBE_DEFECT_TYPE_e type)
{
    return m_mapTypeProbeMin.value(type);
}

int ProbeNoManager::GetProbeMaxByType(PROBE_DEFECT_TYPE_e type)
{
    return m_mapTypeProbeMax.value(type);
}

int ProbeNoManager::GetProbeNumByType(PROBE_DEFECT_TYPE_e type)
{
    return (GetProbeMaxByType(type) - GetProbeMinByType(type) + 1);
}

int ProbeNoManager::GetSensorMinIdx()
{
    return (m_probeNoInfoLeft->sensorMin() - m_probeNoInfoLeft->GetStartProbeNo() - 4000);
}

int ProbeNoManager::GetSensorMaxIdx()
{
    return (m_probeNoInfoLeft->sensorMax() - m_probeNoInfoLeft->GetStartProbeNo() - 4000);
}

bool ProbeNoManager::IsProbeStraightFlange(int probeNo)
{
    PROBE_DEFECT_TYPE_e probeType = ProbeNo2Type(probeNo);
    if(probeType == LEFT_RIM_TREAD || probeType == LEFT_RIM_ARM_TREAD
            || probeType == LEFT_FLANGE_RADIAL || probeType == RIGHT_RIM_TREAD
            || probeType == RIGHT_RIM_ARM_TREAD || probeType == RIGHT_FLANGE_RADIAL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ProbeNoManager::IsProbe21Degree(int probeNo)
{
    PROBE_DEFECT_TYPE_e probeType = ProbeNo2Type(probeNo);
    if(probeType == LEFT_RIM_RADIAL_PLUS1 || probeType == LEFT_RIM_RADIAL_PLUS2
            || probeType == LEFT_RIM_RADIAL_MINUS1 || probeType == LEFT_RIM_RADIAL_MINUS2
            || probeType == LEFT_RIM_RADIAL_PLUS1 || probeType == LEFT_RIM_RADIAL_PLUS2
            || probeType == LEFT_RIM_RADIAL_MINUS1 || probeType == LEFT_RIM_RADIAL_MINUS2)
    {
        return true;
    }
    else
    {
        return false;
    }
}
