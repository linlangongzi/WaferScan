#include "UTData.h"

QAtomicInt UTDataFrame::count = 0;

UTDataFrame::UTDataFrame()
{
    ++count;
}

UTDataFrame::UTDataFrame(const UTDataFrame &other) :
    AData(other.AData),
    CData(other.CData),
    Pose(other.Pose),
    Context(other.Context),
    point(other.point),
    FileOffset(other.FileOffset)
{
    ++count;
}

UTDataFrame::~UTDataFrame()
{
    --count;
}

/*!
 * \brief 根据水中的声速，计算当前取样帧中的水距深度(mm)
 * \param waterSpeed 水中的声速(m/s)
 * \return 水距(mm)，永远返回非负值
 */
float UTDataFrame::GetWaterPathLength(float waterSpeed)
{
    double waterPathTime = Context.GetDelay() + CData.value(GATE_SYNC_TIME_INDEX);      /* 水程的耗时(us) */
    return qMax<float>(waterPathTime * waterSpeed * 1e-3, 0);                   /* = 水程耗时(us) * 声速(m/s) * 1e-3(m/s -> mm/us)，这个值应当是非负值 */
}

/*!
 * \brief 根据水中的声速和当前取样帧的姿态，计算声束在水中的水程向量
 * \param waterSpeed 水中的声速(m/s)
 * \return 水程向量(mm)
 */
QVector3D UTDataFrame::GetWaterPathVector(float waterSpeed)
{
    return Pose.GetDirection().normalized() * GetWaterPathLength(waterSpeed);   /* = 位姿指向.单位向量 * 水距深度(mm) */
}

/*!
 * \brief 根据水中的声速和当前姿态，计算声束在工件表面入射点的位置
 * \param waterSpeed 水中的声速(m/s)
 * \return 声束入射点的空间坐标
 */
QVector3D UTDataFrame::GetWaterPathPosition(float waterSpeed)
{
    return GetWaterPathVector(waterSpeed) + Pose.GetPosition();
}

/*!
 * \brief 计算声束在工件中的行程
 * \param speed 材料声速(m/s)
 * \param CDataIndex C扫描特征值的索引
 * \return 行程(mm)，永远返回非负值
 */
float UTDataFrame::GetMaterialPathLength(float speed, uint CDataIndex)
{
    Q_ASSERT_X(speed > 0, "UTDataFrame::GetMaterialPathLength", "Negative material speed");
    return qMax<float>(CData.value(CDataIndex) * speed * 1e-3, 0);              /* 由于传入的速度和特征值可能小于0，最终的距离应当是非负值 */
}

/*!
 * \brief 计算声束在工件中的行程向量
 * \param speed 材料声速(m/s)
 * \param CDataIndex C扫描特征值的索引
 * \return 行程向量(mm)
 */
QVector3D UTDataFrame::GetMaterialPathVector(float speed, uint CDataIndex)
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
float UTDataFrame::GetFullPathLength(float waterSpeed, float materialSpeed, uint CDataIndex)
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
QVector3D UTDataFrame::GetFullPathVector(float waterSpeed, float materialSpeed, uint CDataIndex)
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
QVector3D UTDataFrame::GetFullPathPosition(float waterSpeed, float materialSpeed, uint CDataIndex)
{
    return Pose.GetPosition() + GetFullPathVector(waterSpeed, materialSpeed, CDataIndex);
}
