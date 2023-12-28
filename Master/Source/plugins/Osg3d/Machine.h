#pragma once

#include <QString>
#include <osg/MatrixTransform>
#include <QList>
#include "Joint.h"
#include "Item3d.h"
#include <QtConcurrent>

/*!
 * \brief The Machine class
 *          1、维护一个设备仿真模型
 *          2、实现对设备各运动轴位置的控制
 */
class Machine : public Item3d
{
    Q_OBJECT

public:
    explicit Machine();

    static osg::ref_ptr<Machine> importXml(const QString &path);

    void setTranslate(const QVector3D &t);                                      //设置设备坐标系与OSG坐标系之间的平移
    void setRotate(const QVector3D &r);                                         //设置设备坐标系与OSG坐标系之间的旋转
    QFuture<void> loadProgress() const {return watcher.future();}

    QList<osg::ref_ptr<Joint> > jointList;                                      //保存所有的设备轴Joint，每个Joint有唯一的轴名称Name，可用来进行查找

private slots:
    void onGeometryFinished();

private:
    void UpdateMatrixTransform();                                               //更新设备坐标系与OSG坐标系之间的变换
    osg::Matrix rotateX;                                                        //绕X轴旋转矩阵
    osg::Matrix rotateY;
    osg::Matrix rotateZ;
    osg::Matrix translate;                                                      //平移矩阵

    struct Associate {
        QString name;
        osg::ref_ptr<Group> parent;
    };
    QList<Associate> unresolvedGeometries;
    QFutureWatcher<osg::Geometry *> watcher;
};
