#include "SafetyBarrier.h"
#include "Axis.h"
#include "MotionManager.h"
#include "TrioDriver.h"
#include <QLoggingCategory>

using namespace Motion;

static Q_LOGGING_CATEGORY(debug, "motion.safe");

SafetyBarrier::SafetyBarrier(QObject *parent) :
    QObject(parent)
{
    Init();

    connect(MotionManager::instance(), &MotionManager::axisListChanged, this, &SafetyBarrier::onAxisListChanged);
    onAxisListChanged();
}

/*! 获取当前的安全状态 */
SafetyBarrier::Status SafetyBarrier::status()
{
    return m_status;
}

/*! 设置安全区域参数 */
void SafetyBarrier::setOptions(const SafetyBarrierOptions &options)
{
    if (options == m_options) {
        return;
    }
    m_options = options;
    // 一旦安全区域改变，需要发送安全区域改变信号
    OnMotionChanged();
    emit SafeRectChanged();
}

/*! 初始化安全模块 */
void SafetyBarrier::Init()
{
    TrioDriver *trio = MotionManager::driver();
    Q_ASSERT_X(trio != nullptr, "EndPoint::LoadCompleted()", "trio is null");
    connect(trio, &TrioDriver::statusChanged, this, &SafetyBarrier::OnMotionConnectChanged);
}

/*! 连接状态改变信号的槽函数 */
void SafetyBarrier::OnMotionConnectChanged()
{
    auto state = MotionManager::driver()->status().state;
    if ((state != Normal) && (state != SwitchingMode)) {
        setStatus(NotConnected);
    }
}

/*! 接收驱动运动状态改变信号 */
void SafetyBarrier::OnMotionChanged()
{
    if (xAxis.isNull() || yAxis.isNull() || zAxis.isNull()) {
        if (m_options.disabled) {
            setStatus(Disabled);
        } else {
            setStatus(AxisMissing);
        }
        return;
    }
    double posX = xAxis->position();
    double posY = yAxis->position();
    double posZ = zAxis->position();
    double maxZ = 0, maxX = 0, minX = 0, maxY = 0, minY = 0;
    // 根据xy坐标，设置Z轴的限位
    QRectF safeRect = m_options.rect;
    if (m_options.disabled) {
        maxZ = zAxis->config().limitForward;
    } else if (safeRect.contains(posX, posY)) {                                 // 在安全区内，Z轴可以下降到物理限位
        maxZ = qMin(zAxis->config().limitForward, m_options.maxDepth);
    } else {                                                                    // 在安全区外， Z轴只能到达空走高度
        maxZ = m_options.minDepth;
    }
    // 根据Z轴坐标， 设置xy轴限位
    if (!m_options.disabled && posZ - m_options.minDepth >= 0.01) {             // z轴大于空走高度，只能是在安全区内
        maxX = safeRect.right() + MARGIN_SAFERECT_SENT;
        minX = safeRect.left() - MARGIN_SAFERECT_SENT;
        maxY = safeRect.bottom() + MARGIN_SAFERECT_SENT;
        minY = safeRect.top() - MARGIN_SAFERECT_SENT;
    } else {                                                                    // 可以是在整个水池中运动
        maxX = xAxis->config().limitForward;
        minX = xAxis->config().limitReverse;
        maxY = yAxis->config().limitForward;
        minY = yAxis->config().limitReverse;
    }
    // 判断最新限位与上次限位是否相同，不相同则发送，相同则不发送
    if (!IsEqual(maxZ, oldZFSLimit)) {
        zAxis->SetFSLimit(maxZ);
        oldZFSLimit = maxZ;
    }
    if (!IsEqual(maxX, oldXFSLimit) || !IsEqual(minX, oldXRSLimit) || !IsEqual(maxY, oldYFSLimit) || !IsEqual(minY, oldYRSLimit)) {
        xAxis->SetFSLimit(maxX);
        oldXFSLimit = maxX;
        xAxis->SetRSLimit(minX);
        oldXRSLimit = minX;
        yAxis->SetFSLimit(maxY);
        oldYFSLimit = maxY;
        yAxis->SetRSLimit(minY);
        oldYRSLimit = minY;
    }
    // 判断新的安全状态与旧的状态是否相同，不相同则发送信号
    Status status = GetSafeStatusFromPos(posX, posY, posZ);
    setStatus(status);
}

/*! 比较两个double类型数值是否相等（采用下位机适合的分辨率） */
bool SafetyBarrier::IsEqual(double x, double y)
{
    return qAbs(x - y) < 0.01;
}

/*! 根据坐标判断是处于安全区域的那种状态位置 */
SafetyBarrier::Status SafetyBarrier::GetSafeStatusFromPos(double x, double y, double z)
{
    if (m_options.disabled) {
        return Disabled;
    }
    QRectF safeRect = m_options.rect;
    if (z - m_options.minDepth >= 0.1) {
        if ( safeRect.contains(x, y) ) {
            return SafetyBarrier::Under;
        }
        return SafetyBarrier::ExternalUnsafe;
    } else {
        if ( safeRect.contains(x, y) ) {
            return SafetyBarrier::Upper;
        }
        return SafetyBarrier::ExternalSafe;
    }
}

/*! 根据成像轴，获取安全区域的范围polygon */
QPolygonF SafetyBarrier::GetSafePolygon(int xAxisID, int yAxisID)
{
    Axis *axisX = MotionManager::axis(xAxisID);
    Axis *axisY = MotionManager::axis(yAxisID);
    if ( (axisX != nullptr) && (axisY != nullptr) ) {
        if (axisX->config().isLocateMaster || axisY->config().isLocateMaster) {                          // 判断是否有z轴
            return GetSafePolygonWithZAxis(axisX, axisY);
        }
        return GetSafePolygonWithoutZAxis(axisX, axisY);
    }
    return QPolygonF();
}

SafetyBarrierOptions SafetyBarrier::options() const
{
    return m_options;
}

void SafetyBarrier::setStatus(SafetyBarrier::Status status)
{
    if (m_status == status) {
        return;
    }

    qCDebug(debug) << "Status" << status;
    m_status = status;
    emit statusChanged(status);
}

void SafetyBarrier::onAxisListChanged()
{
    if (xAxis) {
        disconnect(xAxis, nullptr, this, nullptr);
    }
    if (yAxis) {
        disconnect(yAxis, nullptr, this, nullptr);
    }
    if (zAxis) {
        disconnect(zAxis, nullptr, this, nullptr);
    }

    xAxis = MotionManager::findAxis("name", QString("X"));
    yAxis = MotionManager::findAxis("name", QString("Y"));
    zAxis = MotionManager::findAxis("name", QString("Z"));
    if (xAxis.isNull() || yAxis.isNull() || zAxis.isNull()) {
        return;
    }
    connect(xAxis.data(), &Axis::statusChanged, this, &SafetyBarrier::OnMotionChanged);
    connect(yAxis.data(), &Axis::statusChanged, this, &SafetyBarrier::OnMotionChanged);
    connect(zAxis.data(), &Axis::statusChanged, this, &SafetyBarrier::OnMotionChanged);
}

/*!
 * \brief SafetyBarrier::GetAxisSafeRange 获取轴的安全范围
 * \param axis  对应轴
 * \return      范围
 */
Range SafetyBarrier::GetAxisSafeRange(Axis *axis)
{
    QRectF safeRect = m_options.rect;
    double emptyHeigh = m_options.minDepth;
    if (axis == nullptr) {
        return Range();
    }
    switch ( axis->axisId() ) {
    case 0:
        return Range( safeRect.left(), safeRect.right() );
    case 1:
        return Range( safeRect.top(), safeRect.bottom() );
    case 2:
        return Range(axis->config().limitReverse, emptyHeigh);
    default:                                                                    // xyz不可能为无限旋转轴
        return Range(axis->config().limitReverse, axis->config().limitForward);
    }
}

/*!
 * \brief SafetyBarrier::GetSafePolygonWithZAxis   获取包含Z轴的安全区polygon
 * \param xAxis x成像轴
 * \param yAxis y成像轴
 * \return      安全区的polygon
 */
QPolygonF SafetyBarrier::GetSafePolygonWithZAxis(Axis *axisX, Axis *axisY)
{
    if ( (axisX == nullptr) || (axisY == nullptr) ) {
        return QPolygonF();
    }
    Range rangeX = GetAxisSafeRange(axisX);
    Range rangeY = GetAxisSafeRange(axisY);
    Range physicalRangeX = Range(axisY->config().limitReverse, axisY->config().limitForward);
    Range physicalRangeY = Range(axisX->config().limitReverse, axisX->config().limitForward);
    QPolygonF firstPolygon( Range::toRect(rangeX, physicalRangeX) );
    return firstPolygon.united( Range::toRect(physicalRangeY, rangeY) );
}

/*! 获取不包含Z轴的安全区polygon */
QPolygonF SafetyBarrier::GetSafePolygonWithoutZAxis(Axis *axisX, Axis *axisY)
{
    if ((axisX == nullptr) || (axisY == nullptr)) {
        return QPolygonF();
    }
    Range rangeX = GetAxisSafeRange(axisX);
    Range rangeY = GetAxisSafeRange(axisY);
    QRectF rect = Range::toRect(rangeX, rangeY);
    return QPolygonF(rect);
}
