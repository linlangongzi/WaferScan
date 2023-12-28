#include "Machine.h"
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>

#pragma execution_character_set("utf-8")
using namespace osg;


/*********************************************************************************以下为非成员函数
 */
/*!
 * \brief ReadStlFile   读取STL文件
 * \param fileName      文件名
 * \return              返回STL的Header中的零件名称和OSG节点
 */
QPair<QString, ref_ptr<Node> > ReadStlFile(const QString fileName){
    qDebug() << "StlFile input name:" << fileName;
#ifdef OSG_USE_UTF8_FILENAME
    Node *node = osgDB::readNodeFile(fileName.toUtf8().toStdString());
#else
    Node *node = osgDB::readNodeFile(fileName.toLocal8Bit().toStdString());                    //读取STL文件到节点
#endif
    if (node) {
        ref_ptr<Group> group = node->asGroup();                                 //将STL节点转换成Group
        if (group->getNumChildren() == 0) {
            return QPair<QString, ref_ptr<Node> >();
        }
        ref_ptr<Node> nodeChild = group->getChild(0);                           //获取Group的子节点
        QString name = QString::fromStdString(
                            nodeChild->asGeode()->getName());                   //获取子节点中的Geode名称，即为STL文件中的header name，可能为中文乱码
        qDebug() << "StlFile header name:" << name;
        if (name.isEmpty()) {
            qDebug() << "stl header name is empty";
            return QPair<QString, ref_ptr<Node> >();
        }
        return QPair<QString, ref_ptr<Node> >(name, node);
    } else {
        qDebug() << "Failed reading node" << fileName ;
        return QPair<QString, ref_ptr<Node> >();
    }
}

QMap<QString, ref_ptr<Node> > ReadGeometries(QXmlStreamReader &reader)
{
    QMap<QString, ref_ptr<Node> > geometries;
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == "geometries") {
            break;
        } else if (token == QXmlStreamReader::StartElement && reader.name() == "geometry") {
            QXmlStreamAttributes attrs = reader.attributes();
            QString stlFilePath = attrs.value(QLatin1String("file")).toString();
            QPair<QString, ref_ptr<Node> > pairNode = ReadStlFile(stlFilePath); //读取STL文件
            if(!pairNode.first.isEmpty() && pairNode.second) {                  //确保数据不为空
                geometries[pairNode.first] = pairNode.second;
            }
        }
    }
    return geometries;
}


QVector3D StringToVector3D(QString string)
{
    QVector<QString> stringVector = string.split(" ").toVector();
    int count = stringVector.size();
    QVector<qreal> vec;
    foreach (QString it, stringVector) {
        vec.append(it.toDouble());
    }
    if (count == 3) {
        return QVector3D(vec[0], vec[1], vec[2]);
    } else {
        return QVector3D();
    }
}

Joint::MotionType StringToMotionType(QString tp)
{
    if (tp == "Linear") {
        return Joint::Linear;
        qDebug() << "motion type:" << tp;
    } else if (tp == "Rotary") {
        return Joint::Rotary;
    } else {
        return Joint::None;
    }
}

/*!
 * \brief Machine::ReadFromXml
 * \param path
 * \return
 */
ref_ptr<Machine> Machine::ReadFromXml(const QString &path)
{
    QFile fileMode(path);
    if (!fileMode.open(QIODevice::ReadOnly)) {
        return NULL;
    }
    ref_ptr<Machine> machine = new Machine;                                     //joint根组节点
    ref_ptr<Group> parent = machine;                                            //当前的父节点
    QXmlStreamReader reader(&fileMode);
    QMap<QString, ref_ptr<Node> > mapGeode;                                      //将所有的STL文件保存在此Map中
    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == "geometries") {                                //读取所有的STL文件
                qDebug() << "Reading geometries";
                mapGeode = ReadGeometries(reader);
                qDebug() << "Finished reading geometries" << mapGeode.count();
            } else if (reader.name() == "tree") {
                qDebug() << "Get into: " << reader.name();
            } else if (reader.name() == "assoc") {                              //添加几何节点
                QXmlStreamAttributes attrs = reader.attributes();
                QString stlName = attrs.value(QLatin1String("name")).toString();
                qDebug() << "stl header name: " << stlName;
                const ref_ptr<Node> node = mapGeode.value(stlName);
                if (node) {
                    node->setName(stlName.toLocal8Bit().toStdString());
                    parent->addChild(node.get());
                } else {
                    qDebug() << "Cann't find STL file by header name in mapGeode" ;
                }
            } else if (reader.name() == "joint") {                              //添加joint
                qDebug() << "Get into: " << reader.name();
                QXmlStreamAttributes attrs = reader.attributes();
                ref_ptr<Joint> joint = new Joint;
                qDebug() << "Name: " << attrs.value(QLatin1String("name")).toString();
                joint->setName(attrs.value(QLatin1String("name")).toString().toStdString());
                joint->SetType(StringToMotionType(attrs.value(QLatin1String("type")).toString()));
                joint->SetMoveBase(attrs.value(QLatin1String("move_base")).toDouble());
                joint->SetMoveMin(attrs.value(QLatin1String("move_min")).toDouble());
                joint->SetMoveMax(attrs.value(QLatin1String("move_max")).toDouble());
                joint->SetPointBase(StringToVector3D(attrs.value(QLatin1String("pointBase")).toString()));
                joint->SetAxisZ(StringToVector3D(attrs.value(QLatin1String("axisZ")).toString()));
                joint->SetPos(0);                                               //设备开启后移动到原点
                machine->jointList.append(joint);
                parent->addChild(joint.get());                                  //此joint节点为一个MatrixTransform
                parent = joint;                                                 //将父节点修改为此joint
            }
        } else if (token == QXmlStreamReader::EndElement) {
            qDebug() << "out of " << reader.name();
            if (reader.name() == "joint") {
                parent = parent->getParent(0);                                  //退出时将父节点上移
            }
        }
    }
    return machine;
}

void Machine::SetJointPos(int ID, qreal pos)
{
    auto jointIterator = jointMap.find(ID);
    if (jointIterator != jointMap.end()) {
        (*jointIterator)->SetPos(pos);
    }
}

void Machine::SetJointId(int ID, const QString &jointName)
{
    foreach (auto joint, jointList) {
        if (joint->getName() == jointName.toStdString()) {
            jointMap[ID] = joint;
            return;
        }
    }
}

osg::ref_ptr<Joint> Machine::GetJointById(int ID)
{
    auto idFind = jointMap.find(ID);
    if (idFind != jointMap.end()) {
        return jointMap[ID];
    } else {
        return NULL;
    }
}

osg::ref_ptr<Joint> Machine::GetJointByName(QString name)
{
    foreach (auto joint, jointList) {
        if(joint->getName() == name.toStdString()) {
            return joint;
            break;
        }
    }
    return NULL;
}
