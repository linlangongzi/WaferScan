#ifndef JOINT_H
#define JOINT_H

#include <QVector3D>
#include <osg/MatrixTransform>

/*!
 * \brief The Joint class
 *          派生自MatrixTransform
 *          1、保存MotionPerfect中的关节信息
 *          2、利用从MatrixTransfor中继承来的特性，保存关节平移或旋转、偏移量等属性
 *          3、利用Group的特性，保存设备所有子节点
 */
class Joint : public osg::MatrixTransform
{
public:
    Joint();
    enum MotionType {
        None,
        Linear,
        Rotary
    };
    void SetType(MotionType t) {type = t;}
    MotionType GetType() const {return type;}
    void SetPointBase(const QVector3D &pb);
    void SetAxisZ(const QVector3D &axis);
    void SetMoveBase(qreal base) {moveBase = base;}
    void SetMoveMin(qreal min) {moveMin = min;}
    void SetMoveMax(qreal max) {moveMax = max;}

    void SetPos(qreal pos);
    void UpdateMatrix();                                                        //更新baseMastrixBefore, baseMatrixAfter
private:
//    QString name;                                                             //保存关节名称，采用了继承的SetName(), GetName()的接口
    MotionType type = None;                                                     //关节运动类型
    qreal moveBase = 0;                                                         //关节零点偏移量
    qreal moveMin = -Q_INFINITY;                                                //关节运动范围最小值
    qreal moveMax = Q_INFINITY;                                                 //关节运动范围最大值
    QVector3D pointBase;                                                        //关节的参考点，对平移轴来说，没有用处，对旋转轴来说，是旋转中心
    QVector3D axisZ;                                                            //关节轴线，平移方向或旋转轴线方向
    osg::Matrix matrix;                                                         //关节平移或旋转对应的矩阵
    osg::Matrix baseMatrixBefore;                                               //关节参考点往原点平移时的矩阵：包括了关节零点偏移的变换矩阵
    osg::Matrix baseMatrixAfter;                                                //关节参考点从原点往对应位置平移返回时的矩阵

};

#endif // JOINT_H
