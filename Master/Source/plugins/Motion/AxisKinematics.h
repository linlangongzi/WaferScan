#pragma once

#include <QMatrix4x4>
#include <QJsonObject>
#include <allrising/MetaType/MetaType.h>

/*!
 * \brief AxisKinematics 是一个可转换为 JSON 的三维矩阵变换操作，可以将轴位置作为参数代入
 *
 * 矩阵变换有下面几种类型：
 *  ROTATE - 旋转
 *    绕 axis 矢量旋转，角度为 multiplier.x * pos + constant.x
 *  TRANSLATE - 平移
 *    平移的位移为 multiplier * pos + constant ，其中 multiplier 和 constant 均为矢量
 *  固定偏移轴的处理
 *    令multiplier=[0,0,0]即可
 */
class AxisKinematics
{
    Q_GADGET

    Q_PROPERTY(QVector3D multiplier MEMBER multiplier)
    Q_PROPERTY(QVector3D constant MEMBER constant)
    Q_PROPERTY(QVector3D axis MEMBER axis)
    Q_PROPERTY(Type type MEMBER type)

public:
    void addTransform(QMatrix4x4 &m, qreal pos) const;                          /* 将矩阵m平移或者旋转pos个单位(度 or mm) */
    QMatrix4x4 transform(qreal pos);                                            /* 获取变换矩阵 */
    static AxisKinematics fromJson(const QJsonValue &json);
    QJsonValue toJson() const;

    enum Type {                                                                 /*!< 矩阵变换类型 */
        NONE,                                                                   /*!< 无变换 */
        TRANSLATE,                                                              /*!< 平移 */
        ROTATE,                                                                 /*!< 旋转 */
    };
    Q_ENUM(Type)

    DECLARE_COMPARE_OPERATORS(AxisKinematics)

    QVector3D multiplier;                                                       /*!< 轴位置系数 */
    QVector3D constant;                                                         /*!< 轴位置偏移常数 */
    QVector3D axis;                                                             /*!< 旋转轴矢量 */
    Type type = NONE;                                                           /*!< 变换类型 */
};
