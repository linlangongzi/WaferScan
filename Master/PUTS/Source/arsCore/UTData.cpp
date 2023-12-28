#include "UTData.h"
#include "arsCore/Axis.h"
#include "arsCore/UTChannel.h"
#include "arsCore/UTDataHeader.h"
#include "arsCore/Util.h"
#include "arsCore/UTInstrument.h"
#include "arsCore/UTSetup.h"
#include "Platform/Platform.h"
#include <QFile>

const int frameId = qRegisterMetaType<UTSharedDataFrame>("UTSharedDataFrame");
const int segmentId = qRegisterMetaType<UTSharedDataSegment>("UTSharedDataSegment");

QAtomicInt UTDataFrame::globalCount = 0;
QList<UTDataHeader> UTDataFrame::headers;

UTDataFrame::UTDataFrame() :
    headerId(0)
{
    ++globalCount;
}

UTDataFrame::UTDataFrame(const UTDataFrame &other) :
    AData(other.AData),
    CData(other.CData),
    Pose(other.Pose),
    Context(other.Context),
    headerId(other.headerId)
{
    ++globalCount;
}

UTDataFrame::~UTDataFrame()
{
    --globalCount;
}

bool UTDataFrame::IsEmpty() const
{
    return !( IsCDataValid() || IsADataValid() );
}

bool UTDataFrame::IsADataValid() const
{
    return !AData.isEmpty();
}

bool UTDataFrame::IsCDataValid() const
{
    return CData.value(GATE_SYNC_TIME_INDEX) != 0;
}

/*!
 * \brief UTDataFrame::IsIGateTriggered I闸门是否触发
 * \return  true : 触发
 *          false : 没触发
 */
bool UTDataFrame::IsIGateTriggered() const
{
    return GetSyncTime() > 0;
}

CDataValue UTDataFrame::GetSyncTime() const
{
    return CData.value(GATE_SYNC_TIME_INDEX);
}

/*!
 * \brief 根据水中的声速，计算当前取样帧中的水距深度(mm)
 * \param waterSpeed 水中的声速(m/s)
 * \return 水距(mm)，永远返回非负值
 */
float UTDataFrame::GetWaterPathLength(float waterSpeed) const
{
    if ( !IsCDataValid() )
    {
        return qSNaN();
    }
    double waterPathTime = Context.GetDelay() + CData.value(GATE_SYNC_TIME_INDEX);  /* 水程的耗时(us) */
    return qMax<float>(waterPathTime * waterSpeed * 0.5e-3, 0);                 /* = 水程耗时(us) * 声速(m/s) * 1e-3(m/s -> mm/us) / 2，这个值应当是非负值 */
}

/*!
 * \brief 根据水中的声速和当前取样帧的姿态，计算声束在水中的水程向量
 * \param waterSpeed 水中的声速(m/s)
 * \return 水程向量(mm)
 */
QVector3D UTDataFrame::GetWaterPathVector(float waterSpeed) const
{
    return Pose.GetDirection().normalized() * GetWaterPathLength(waterSpeed);   /* = 位姿指向.单位向量 * 水距深度(mm) */
}

/*!
 * \brief 根据水中的声速和当前姿态，计算声束在工件表面入射点的位置
 * \param waterSpeed 水中的声速(m/s)
 * \return 声束入射点的空间坐标
 */
QVector3D UTDataFrame::GetWaterPathPosition(float waterSpeed) const
{
    return GetWaterPathVector(waterSpeed) + Pose.GetPosition();
}

/*!
 * \brief 计算声束在工件中的行程
 * \param speed 材料声速(m/s)
 * \param CDataIndex C扫描特征值的索引
 * \return 行程(mm)，永远返回非负值
 */
float UTDataFrame::GetMaterialPathLength(float speed, uint CDataIndex) const
{
    Q_ASSERT_X(speed > 0, "UTDataFrame::GetMaterialPathLength", "Negative material speed");
    return qMax<float>(CData.value(CDataIndex) * speed * 0.5e-3, 0);            /* 由于传入的速度和特征值可能小于0，最终的距离应当是非负值 */
}

/*!
 * \brief 计算声束在工件中的行程向量
 * \param speed 材料声速(m/s)
 * \param CDataIndex C扫描特征值的索引
 * \return 行程向量(mm)
 */
QVector3D UTDataFrame::GetMaterialPathVector(float speed, uint CDataIndex) const
{
    return Pose.GetDirection().normalized() * GetMaterialPathLength(speed, CDataIndex);
}

/*!
 * \brief 计算声束在水中和工件中的总行程
 * \param waterSpeed 水中的声速(m/s)
 * \param materialSpeed 材料声速(m/s)
 * \param CDataIndex C扫描特征值的索引
 * \return 行程(mm)
 */
float UTDataFrame::GetFullPathLength(float waterSpeed, float materialSpeed, uint CDataIndex) const
{
    return GetWaterPathLength(waterSpeed) + GetMaterialPathLength(materialSpeed, CDataIndex);
}

/*!
 * \brief 计算声束在水中和工件中的总行程向量
 * \param waterSpeed 水中的声速(m/s)
 * \param materialSpeed 材料声速(m/s)
 * \param CDataIndex C扫描特征值的索引
 * \return 行程(mm)
 */
QVector3D UTDataFrame::GetFullPathVector(float waterSpeed, float materialSpeed, uint CDataIndex) const
{
    return Pose.GetDirection().normalized() * GetFullPathLength(waterSpeed, materialSpeed, CDataIndex);
}

/*!
 * \brief 计算C扫描特征点在工件中的空间位置
 * \param waterSpeed 水中的声速(m/s)
 * \param materialSpeed 材料声速(m/s)
 * \param CDataIndex C扫描特征值的索引
 * \return 空间位置向量(mm)
 */
QVector3D UTDataFrame::GetFullPathPosition(float waterSpeed, float materialSpeed, uint CDataIndex) const
{
    return Pose.GetPosition() + GetFullPathVector(waterSpeed, materialSpeed, CDataIndex);
}

void UTDataFrame::UpdatePosition()
{
    UTChannel *channel = UTChannel::GetChannelByGlobalIndex(Context.GetChannelID());
    if (!channel) {
        return;
    }
    QMatrix4x4 transform;
    channel->AddTransform(transform, Pose);
    position = transform.map(QVector3D());
    direction = transform.map(QVector3D(0, 0, 1)) - position;
}

UTPoseValue UTDataFrame::GetPose(int axis) const
{
    switch (axis) {
    case AXIS_X:        return position.x();
    case AXIS_Y:        return position.y();
    case AXIS_Z:        return position.z();
    case AXIS_DIR_X:    return direction.x();
    case AXIS_DIR_Y:    return direction.y();
    case AXIS_DIR_Z:    return direction.z();
    default:            return Pose.value(axis);
    }
}

float UTDataFrame::GetMPTime(int gateIndex) const
{
    if ( (gateIndex == 1) || (gateIndex == 2) || (gateIndex == 3) )
    {
        int start = (gateIndex - 1) * 5 + 3;
        return CData.value(start);
    }
    else if (gateIndex == 0)
    {
        return CData.value(1);
    }
    return 0.0;
}

float UTDataFrame::GetMPPercent(int gateIndex) const
{
    if ( (gateIndex == 1) || (gateIndex == 2) || (gateIndex == 3) )
    {
        int start = (gateIndex - 1) * 5 + 3;
        return CData.value(start + 1);
    }
    else if (gateIndex == 0)
    {
        return CData.value(2);
    }
    return 0.0;
}

float UTDataFrame::GetFPTime(int gateIndex) const
{
    if ( (gateIndex == 1) || (gateIndex == 2) || (gateIndex == 3) )
    {
        int start = (gateIndex - 1) * 5 + 3;
        return CData.value(start + 3);
    }
    return 0.0;
}

float UTDataFrame::GetFPPercent(int gateIndex) const
{
    if ( (gateIndex == 1) || (gateIndex == 2) || (gateIndex == 3) )
    {
        int start = (gateIndex - 1) * 5 + 3;
        return CData.value(start + 4);
    }
    return 0.0;
}

/*!
 * \brief UTDataFrame::GetGatePosInfo 获取闸门的位置信息
 * \param index 轴序号
 * \return 该轴位置信息
 */
QString UTDataFrame::GetGatePosInfo(int index) const
{
    QString info;
#ifndef ARS_NO_MOTION
    foreach (Axis *axis, platform()->Axes()->GetRegistry<Axis>())
    {
        if (axis->GetAxisID() == index)
        {
            info.append( axis->Get("Name").toString() );
            info.append( QObject::tr(" %1 %2").arg(Pose.value(index), 6, 'f', 2).arg( axis->GetAxisUnit() ) );
        }
    }
#else
    Q_UNUSED(index);
#endif
    return info;
}

/*!
 * \brief UTDataFrame::GetGateChannelInfo 获取通道号信息
 * \return
 */
QString UTDataFrame::GetChannelName() const
{
    UTChannel *channel = UTChannel::GetChannelByGlobalIndex(Context.GetChannelID());
    if (channel) {
        return channel->GetName();
    }
    return QString::number(Context.GetChannelID());
}

int UTDataFrame::GetGateInfoNum(int nGateIndex)
{
    if (0 == nGateIndex)
    {
        return 3;
    }
    else
    {
        return 5;
    }
}

/*!
 * \brief UTDataFrame::GetGateStartCDataIndex
 * \return
 */
int UTDataFrame::GetGateStartCDataIndex(int gateIndex)
{
    int start = 0;
    for (int i = 0; i < gateIndex; ++i)
    {
        start += GetGateInfoNum(i);
    }
    return start;
}

int UTDataFrame::RegisterHeader(const UTDataHeader &header)
{
    headers << header;
    return headers.count() - 1;
}

static const UTDataHeader dummyHeader;
const UTDataHeader &UTDataFrame::GetHeaderById(int id)
{
    if (id < headers.size()) {
        return headers[id];
    }
    return dummyHeader;
}

QString UTDataFrame::GetGateInfo(int featureIndex, bool useTime, bool usePercent) const
{
    if (featureIndex >= 18)
    {
        return QString();
    }
    QString info;
    if ( (featureIndex == 2) || (featureIndex == 4) || (featureIndex == 7) || (featureIndex == 9) || (featureIndex == 12) ||
    ( featureIndex == 14) || ( featureIndex == 17) )
    {
        if (usePercent == true)
        {
            info =  QObject::tr("%1 %").arg(CData.value(featureIndex), 6, 'f', 2);
        }
        else
        {
            info = QObject::tr("%1 dB").arg(DBFromPercent( CData.value(featureIndex) ), 6, 'f', 2);
        }
    }
    else
    {
        if (useTime == true)
        {
            info = QObject::tr("%1 us").arg(CData.value(featureIndex), 6, 'f', 2);
        }
        else
        {
            double materialSpeed = GetMaterialSpeed();
            info = QObject::tr("%1 mm").arg(CData.value(featureIndex) * materialSpeed * 0.5e-3, 6, 'f', 2);
        }
    }
    return info;
}

/*!
 * \brief UTDataFrame::GetPosInfo 根据轴序号获取轴位置
 * \param axisID 轴序号
 * \return 位置
 */
QString UTDataFrame::GetPosInfo(int axisID) const
{
    return QObject::tr("%1 (%2)").arg(Pose.value(axisID), 6, 'f', 2).arg(Context.IsReverse(axisID) ? "-" : "+");
}

QString UTDataFrame::ExportCsv() const
{
    QStringList list;
    foreach (CDataValue value, CData)
    {
        list << QString::number(value);
    }
    foreach (UTPoseValue value, Pose)
    {
        list << QString::number(value);
    }
    foreach (ADataValue value, AData)
    {
        list << QString::number(value);
    }

    return list.join(", ");
}

QString UTDataFrame::GetGateDisplayInfo(int dataIndex, float materialSpeed, float waterSpeed) const
{
    float value = CData.value(dataIndex);
    double speed = dataIndex < 3 ? waterSpeed : materialSpeed;
    switch (GetHeader().GetDataType(dataIndex))
    {
    case UTDataHeader::DATA_TIME:
        return QString("%1 us / %2 mm").arg(value, 7, 'f', 2).arg(DistanceFromTime(value, speed), 7, 'f', 2);
    case UTDataHeader::DATA_AMP:
    default:
        return QString("%1 %  / %2 dB").arg(value, 7, 'f', 2).arg(DBFromPercent(value), 7, 'f', 2);
    }
}

/*!
 * \brief UTDataFrame::GetMaterialSpeed 获取材料声速
 * \return  材料声速
 */
double UTDataFrame::GetMaterialSpeed() const
{
    int channelID = Context.GetChannelID();
    UTChannel *channel = UTChannel::GetChannelByGlobalIndex(channelID);
    ARS_CHECK_RETURN(channel, 0);
    UTSetup *setup = channel->GetBindUTSetup();
    ARS_CHECK_RETURN(setup, 0);
    return setup->GetMaterialSpeed();
}

/*!
 * \brief UTDataFrame::GetNumGates 根据数据计算闸门特征个数
 * \return
 */
int UTDataFrame::GetNumGates() const
{
    int CDataNum = CData.count();
    if (CDataNum > 0 && CDataNum < 4)
    {
        return 1;
    }
    else if (CDataNum >= 4 && CDataNum < 9)
    {
        return 2;
    }
    else if (CDataNum >= 9 && CDataNum < 14)
    {
        return 3;
    }
    else if (CDataNum >= 14 && CDataNum < 19)
    {
        return 4;
    }
    return 0;
}

void DumpSegment(const QString &fileName, UTSharedDataSegment segment)
{
    qDebug () << "DumpSegment to" << fileName;
    QFile dump(fileName);
    if (dump.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        foreach (UTSharedDataFrame frame, segment) {
            dump.write(qPrintable(frame->ExportCsv()));
            dump.write("\n");
        }
    }
    else
    {
        qWarning() << "Error: " << dump.errorString();
    }
}
