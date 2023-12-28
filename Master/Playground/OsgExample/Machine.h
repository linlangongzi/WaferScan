#ifndef MACHINE_H
#define MACHINE_H
#include <QString>
#include <osg/MatrixTransform>
#include <QMap>
#include <QList>

#include "Joint.h"


/*!
 * \brief The Machine class
 *          1、维护一个设备仿真模型
 *          2、实现对设备各运动轴位置的控制
 */
class Machine : public osg::MatrixTransform
{
public:
    static osg::ref_ptr<Machine> ReadFromXml(const QString &path);
    void SetJointPos(int ID, qreal pos);                                        //通过ID索引找到对应的关节，设置该关节的轴位置
    void SetJointId(int ID, const QString &jointName);                          //从jointList中通过jointName查找joint，并将其绑定到ID索引上，添加在jointMap中
    osg::ref_ptr<Joint> GetJointById(int id);                                   //通过索引号查找Joint
    osg::ref_ptr<Joint> GetJointByName(QString name);                                   //通过名称查 找Joint

    QList<osg::ref_ptr<Joint> > jointList;                                      //保存所有的设备轴Joint，每个Joint有唯一的轴名称Name，可用来进行查找
    QMap<int, osg::ref_ptr<Joint> > jointMap;                                   //key为轴号，value为Joint
};

#endif // MACHINE_H
