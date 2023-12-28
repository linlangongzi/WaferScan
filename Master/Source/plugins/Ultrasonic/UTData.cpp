#include "UTData.h"
#include "UltrasonicManager.h"
#include <Motion/Axis.h>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTDataHeader.h>
#include <Ultrasonic/UTInstrument.h>
#include <Ultrasonic/UTSetup.h>
#include <allrising/Util.h>
#include <QFile>

struct UTDataFrameData {
    ADataFrame waveform;
    CDataFrame features;
};

Q_DECL_UNUSED static const int frameId = qRegisterMetaType<UTDataFrame>("UTDataFrame");
Q_DECL_UNUSED static const int segmentId = qRegisterMetaType<UTDataSegment>("UTDataSegment");

QAtomicInt UTDataFrame::globalCount = 0;
using UTDataHeaderMap = QHash<int, UTDataHeader>;
Q_GLOBAL_STATIC(UTDataHeaderMap, headers)

UTDataFrame::UTDataFrame()
{
//    data.embed = new UTDataFrameData;
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

bool UTDataFrame::isEmpty() const
{
    return !( IsCDataValid() || hasWaveform() );
}

bool UTDataFrame::hasWaveform() const
{
    return !AData.isEmpty();
}

int UTDataFrame::waveformLength() const
{
    return AData.count();
}

ADataValue UTDataFrame::waveformAt(int index) const
{
    return AData.value(index);
}

ADataFrame UTDataFrame::waveform() const
{
    return AData;
}

ADataValue UTDataFrame::waveformMax()
{
    return 1023;
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
    return syncTime() > 0;
}

CDataValue UTDataFrame::syncTime() const
{
    return CData.value(GATE_SYNC_TIME_INDEX);
}

/*!
 * \brief 根据水中的声速，计算当前取样帧中的水距深度(mm)
 * \param waterSpeed 水中的声速(m/s)
 * \return 水距(mm)，永远返回非负值
 */
qreal UTDataFrame::waterPathLength(qreal waterSpeed) const
{
    if ( !IsCDataValid() )
    {
        return qSNaN();
    }
    double waterPathTime = Context.GetDelay() + CData.value(GATE_SYNC_TIME_INDEX);  /* 水程的耗时(μs) */
    return qMax<qreal>(waterPathTime * waterSpeed * 0.5e-3, 0);                 /* = 水程耗时(μs) * 声速(m/s) * 1e-3(m/s -> mm/μs) / 2，这个值应当是非负值 */
}

/*!
 * \brief 根据水中的声速和当前取样帧的姿态，计算声束在水中的水程向量
 * \param waterSpeed 水中的声速(m/s)
 * \return 水程向量(mm)
 */
QVector3D UTDataFrame::GetWaterPathVector(float waterSpeed) const
{
    return Pose.GetDirection().normalized() * waterPathLength(waterSpeed);   /* = 位姿指向.单位向量 * 水距深度(mm) */
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
    return waterPathLength(waterSpeed) + GetMaterialPathLength(materialSpeed, CDataIndex);
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

QPair<QVector3D, QVector3D> UTDataFrame::UpdatePosition() const
{
    UTChannel *channel = UltrasonicManager::channel(Context.GetChannelID());
    if (!channel) {
        return qMakePair(QVector3D(), QVector3D());
    }
    QMatrix4x4 transform;
    channel->addTransform(transform, Pose, waterPathLength());
    const QVector3D position = transform.map(QVector3D());
    const QVector3D direction = transform.map(QVector3D(0, 0, 1)) - position;
    return qMakePair(position, direction);
}

UTPoseValue UTDataFrame::GetPose(int axis) const
{
    return Pose.value(axis);
}

bool UTDataFrame::isGateTriggered(int gateIndex) const
{
    int index = GetHeader().GetTimeIndex(gateIndex, GateTrigger);
    return !qFuzzyIsNull(CData.value(index));
}

float UTDataFrame::gateTime(int gateIndex, UTGateType gt) const
{
    int index = GetHeader().GetTimeIndex(gateIndex, gt);
    return CData.value(index);
}

float UTDataFrame::gateAmp(int gateIndex) const
{
    int index = GetHeader().GetAmpIndex(gateIndex, GateMaxPeak);
    return CData.value(index);
}

float UTDataFrame::GetFPTime(int gateIndex) const
{
    int index = GetHeader().GetTimeIndex(gateIndex, GateFirstPeak);
    return CData.value(index);
}

float UTDataFrame::GetFPPercent(int gateIndex) const
{
    int index = GetHeader().GetAmpIndex(gateIndex, GateFirstPeak);
    return CData.value(index);
}

/*! 获取通道号信息 */
QString UTDataFrame::channelName() const
{
    return UltrasonicManager::channelName(Context.GetChannelID());
}

int UTDataFrame::channelId() const
{
    return Context.GetChannelID();
}

int UTDataFrame::RegisterHeader(const UTDataHeader &header, int id)
{
    if (headers->contains(id)) {
        qCritical() << "UTData: headerId" << id << "already registered";
    }
    headers->insert(id, header);
    return id;
}

static const UTDataHeader dummyHeader;
const UTDataHeader &UTDataFrame::GetHeaderById(int id)
{
    if (headers->contains(id)) {
        return (*headers)[id];
    }
    return dummyHeader;
}

QString UTDataFrame::GetFeatureText(int featureIndex, bool useTime, bool usePercent, qreal ampReference) const
{
    CDataValue v = CData.value(featureIndex);
    UTDataType dt = GetHeader().GetDataType(featureIndex);

    if (dt == DataAmp) {
        if (usePercent) {
            return tr("%1 %").arg(v, 6, 'f', 4);
        } else {
            return tr("%1 dB").arg(UltrasonicManager::ampToDecibel(v, ampReference), 6, 'f', 2);
        }
    } else if (dt == DataTime) {
        if (v == 0) {
            return tr("未触发");
        }
        if (useTime) {
            return tr("%1 μs").arg(v, 6, 'f', 4);
        } else {
            qreal speed = materialSpeed();
            return tr("%1 mm").arg(DistanceFromTime(v, speed), 6, 'f', 2);
        }
    }
    return QString();
}

/*! 根据轴序号获取轴位置文字 */
QString UTDataFrame::GetPosInfo(int axisID) const
{
    return QObject::tr("%1 (%2)").arg(Pose.value(axisID), 6, 'f', 2).arg(Context.IsReverse(axisID) ? "-" : "+");
}

QString UTDataFrame::exportCsv() const
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
    list << QString::number(quint32(Context.GetDirections()));
    foreach (ADataValue value, AData)
    {
        list << QString::number(value);
    }

    return list.join(", ");
}

QString UTDataFrame::exportCsv(UTDataFrame::DataType type) const
{
    QStringList list;
    switch (type)
    {
    case DataType::AllDataFrame:
        foreach (CDataValue value, CData)
        {
            list << QString::number(value);
        }
        foreach (UTPoseValue value, Pose)
        {
            list << QString::number(value);
        }
        list << QString::number(quint32(Context.GetDirections()));
        foreach (ADataValue value, AData)
        {
            list << QString::number(value);
        }
        break;
    case DataType::AScanDataFrame:
        foreach (UTPoseValue value, Pose)
        {
            list << QString::number(value);
        }
        foreach (ADataValue value, AData)
        {
            list << QString::number(value);
        }
        break;
    case DataType::CScanDataFrame:
        foreach (CDataValue value, CData)
        {
            list << QString::number(value);
        }
        break;
    case DataType::PoseDataFrame:
        foreach (UTPoseValue value, Pose)
        {
            list << QString::number(value);
        }
        break;
    default:
        return QString();
    }

    return list.join(", ");
}

bool UTDataFrame::operator ==(const UTDataFrame &other) const
{
    return (AData == other.AData) && (CData == other.CData) && (Pose == other.Pose) && (Context == other.Context);
}

void UTDataFrame::setContext(UTDataContext::ContextIndex index, qreal value)
{
    Context[index] = value;
}

/*! 获取材料声速 */
qreal UTDataFrame::materialSpeed() const
{
    int channelID = Context.GetChannelID();
    UTSetup setup = UltrasonicManager::channelSetup(channelID);
    return setup.materialSpeed();
}

CDataValue UTDataFrame::GetFeature(int featureIndex) const
{
    const UTDataHeader &header = GetHeader();
    CDataValue value = CData.value(featureIndex);
    for (int recursion = 10; recursion; --recursion) {
        featureIndex = header.GetSyncIndex(featureIndex);
        if (featureIndex < 0) {
            return value;
        }
        value += CData.value(featureIndex);
    }
    return 0;
}

CDataValue UTDataFrame::GetFeature(int gate, UTGateType gt, UTDataType dt) const
{
    int featureIndex = GetHeader().dataIndex(gate, gt, dt);
    return GetFeature(featureIndex);
}

CDataValue UTDataFrame::FeatureValue(int featureIndex) const
{
    return CData.value(featureIndex);
}

CDataValue UTDataFrame::FeatureValue(int gate, UTGateType gt, UTDataType dt) const
{
    int featureIndex = GetHeader().dataIndex(gate, gt, dt);
    return CData.value(featureIndex);
}

void UTDataFrame::SetFeature(CDataValue value, int gate, UTGateType gt, UTDataType dt)
{
    int featureIndex = GetHeader().dataIndex(gate, gt, dt);
    if (uint(featureIndex) < uint(CData.size())) {
        CData.replace(featureIndex, value);
    }
}

/*! 查询数据帧中的闸门个数 */
int UTDataFrame::GetNumGates() const
{
    return GetHeader().GetNumGates();
}

void DumpSegment(const QString &fileName, UTDataSegment segment)
{
    qDebug () << "DumpSegment to" << fileName;
    QFile dump(fileName);
    if (dump.open(QIODevice::WriteOnly | QIODevice::Append)) {
        foreach (UTDataFrame frame, segment) {
            dump.write(qPrintable(frame.exportCsv()));
            dump.write("\n");
        }
    } else {
        qWarning() << "Error: " << dump.errorString();
    }
}
