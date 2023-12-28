#include "Machine.h"
#include "MachinePart.h"
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>
#include <QtMath>
#include <QFileInfo>
#include <QProgressDialog>

using namespace osg;

/*! 载入STL文件，返回Geometry */
Geometry *LoadStl(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed opening file" << path << file.errorString();
        return nullptr;
    }
    QTextStream stream(&file);

    ref_ptr<Geometry> geometry = new Geometry;                          //三角片元几何

    ref_ptr<Vec3Array> vertexArray = new Vec3Array;                             //顶点集合
    geometry->setVertexArray(vertexArray.get());

    ref_ptr<Vec3Array> normArray = new Vec3Array;                               //法向量集合
    geometry->setNormalArray(normArray.get());
    geometry->setNormalBinding(Geometry::BIND_PER_VERTEX);

    ref_ptr<Vec4Array> colorArray = new Vec4Array;
    geometry->setColorArray(colorArray);
    geometry->setColorBinding(Geometry::BIND_OVERALL);
    colorArray->push_back(Vec4(1.f, 1.f, 1.f, 1.f));

    /* 处理stlname */
    stream.setCodec("UTF-8");
    QString line = stream.readLine();
    QString name = line.mid(6);                                                 //从第6位字符开始为std名
    line = stream.readLine();

    /* 提取三角片元 */
    while (!stream.atEnd() && line != "endsolid") {
        const QStringList elementList = line.split(" ");
        const int size = elementList.size();
        if (size >= 4 && elementList[size - 4] == "normal") {
            QVector3D norm = QVector3D(elementList[size - 3].toDouble(),
                                 elementList[size - 2].toDouble(),
                                 elementList[size - 1].toDouble());
            normArray->push_back(Vec3(norm.x(), norm.y(), norm.z()));
            normArray->push_back(Vec3(norm.x(), norm.y(), norm.z()));
            normArray->push_back(Vec3(norm.x(), norm.y(), norm.z()));
        } else if (size >= 4 && elementList[size - 4] == "vertex") {
            QVector3D point = QVector3D(elementList[size - 3].toDouble(),
                                        elementList[size - 2].toDouble(),
                                        elementList[size - 1].toDouble());
            vertexArray->push_back(Vec3(point.x(), point.y(), point.z()));
        }
        line = stream.readLine();
    }

    geometry->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, vertexArray->size()));
    geometry->setName(name.toStdString());

    return geometry.release();
}

/*!
 * \brief ReadStlFile   读取STL文件
 * \param fileName      文件名
 * \return              返回STL的Header中的零件名称和OSG节点
 */
QPair<QString, ref_ptr<Node> > ReadStlFile(const QString fileName){
    qDebug() << "Reading STL" << fileName;
#ifdef OSG_USE_UTF8_FILENAME
    Node *node = osgDB::readNodeFile(fileName.toUtf8().toStdString());
#else
    Node *node = osgDB::readNodeFile(fileName.toLocal8Bit().toStdString());                    //读取STL文件到节点
#endif
    if (!node) {
        qDebug() << "Failed reading node" << fileName ;
        return QPair<QString, ref_ptr<Node> >();
    }
    ref_ptr<Group> group = node->asGroup();                                     //将STL节点转换成Group
    if (!group && group->getNumChildren() == 0) {
        qDebug() << "Failed convering node as group";
        return QPair<QString, ref_ptr<Node> >();
    }
    ref_ptr<Node> child = group->getChild(0);                                   //获取Group的子节点
    ref_ptr<Geode> geode = child->asGeode();
    if (!geode) {
        qDebug() << "Failed convering node as geode";
        return QPair<QString, ref_ptr<Node> >();
    }
    QString name = QString::fromStdString(geode->getName());                    //获取子节点中的Geode名称，即为STL文件中的header name，可能为中文乱码
    if (name.isEmpty()) {
        qDebug() << "STL header name is empty";
        return QPair<QString, ref_ptr<Node> >();
    }
    return QPair<QString, ref_ptr<Node> >(name, node);
}

QVector3D StringToVector3D(QString string)
{
    QStringList localSplit = string.split(' ');
    return QVector3D(localSplit.value(0).toDouble(),                            //value()可以保证越界安全
                     localSplit.value(1).toDouble(),
                     localSplit.value(2).toDouble());
}

Joint::MotionType StringToMotionType(QString tp)
{
    if (tp == "Linear") {
        return Joint::Linear;
    } else if (tp == "Rotary") {
        return Joint::Rotary;
    } else {
        return Joint::None;
    }
}

QStringList readGeometryList(QFile &file)
{
    file.reset();

    QXmlStreamReader reader(&file);
    QStringList geometries;
    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if (token == QXmlStreamReader::StartElement && reader.name() == "geometries") {
            qDebug() << "Reading geometry list";
            while (!reader.atEnd()) {             //读取geometries，直到读取完成
                auto token = reader.readNext();
                if (token == QXmlStreamReader::StartElement && reader.name() == "geometry") {
                    QString name = reader.attributes().value(QLatin1String("file")).toString();
                    geometries << QFileInfo(file).dir().path() + "/" + name;
                }
            }
        } else if (token == QXmlStreamReader::EndElement && reader.name() == "geometries") {
            return geometries;
        }
    }
    return geometries;
}

Machine::Machine()
{
    connect(&watcher, &QFutureWatcherBase::finished, this, &Machine::onGeometryFinished);
}

ref_ptr<Machine> Machine::importXml(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return NULL;
    }
    QProgressDialog progress(QApplication::activeWindow());
    progress.setLabelText(QString("正在读取设备模型..."));
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setMinimumDuration(0);

    ref_ptr<Machine> machine = new Machine;                               //joint根组节点
    machine->setName("Machine");
    ref_ptr<Group> parent = machine;                                            //当前的父节点
    QXmlStreamReader reader(&file);

    qDebug() << "Reading geometry list...";
    QStringList geometries = readGeometryList(file);
    file.reset();

    qDebug() << "Resolving tree...";
    int assocCount = 0;
    int jointCount = 0;
    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if (token == QXmlStreamReader::StartElement && reader.name() == "assoc") {
            QString name = reader.attributes().value(QLatin1String("name")).toString();
            Associate assoc;
            assoc.name = name;
            assoc.parent = parent;
            machine->unresolvedGeometries << assoc;
        } else if (token == QXmlStreamReader::StartElement && reader.name() == "joint") {                                                          //添加joint
            QXmlStreamAttributes attrs = reader.attributes();
            qDebug() << "Parsing joint" << attrs.value("name");
            ref_ptr<Joint> joint = new Joint;
            joint->setName(attrs.value("name").toString().toStdString());
            joint->setMotionType(StringToMotionType(attrs.value("type").toString()));
            joint->setMoveBase(attrs.value("move_base").toDouble());
            joint->setMoveMin(attrs.value("move_min").toDouble());
            joint->setMoveMax(attrs.value("move_max").toDouble());
            joint->setPointBase(StringToVector3D(attrs.value("pointBase").toString()));
            joint->setAxisZ(StringToVector3D(attrs.value("axisZ").toString()));
            joint->setPos(0);                                                   // 设备开启后移动到原点

            machine->jointList << joint;
            parent->addChild(joint.get());                                      // 此joint节点为一个MatrixTransform
            parent = joint;                                                     // 将父节点修改为此joint
            jointCount++;
        } else if (token == QXmlStreamReader::EndElement && reader.name() == "joint") {
            parent = parent->getParent(0);                        //退出时将父节点上移
        }
    }

    QFuture<Geometry *> future = QtConcurrent::mapped(geometries, &LoadStl);
    machine->watcher.setFuture(future);

    qDebug() << "Finished parsing machine, got" << jointCount << "joints," << assocCount << "geometries";
    return machine;
}

void Machine::setTranslate(const QVector3D &t)
{
    translate.setTrans(Vec3d(t.x(), t.y(), t.z()));
    UpdateMatrixTransform();
}

void Machine::setRotate(const QVector3D &r)
{
    rotateX.makeRotate(qDegreesToRadians(r.x()), Vec3d(1, 0, 0));
    rotateY.makeRotate(qDegreesToRadians(r.y()), Vec3d(0, 1, 0));
    rotateZ.makeRotate(qDegreesToRadians(r.z()), Vec3d(0, 0, 1));
    UpdateMatrixTransform();
}

void Machine::onGeometryFinished()
{
    QHash<QString, Geometry *> geometries;
    for (const auto &geometry: watcher.future()) {
        if (geometry) {
            QString name = QString::fromStdString(geometry->getName());
            geometries[name] = geometry;
        }
    }
    for (const auto &geo: unresolvedGeometries) {
        Geometry *geometry = geometries[geo.name];
        if (geometry) {
            Geode *geode = new Geode;
            geode->addDrawable(geometry);
            geo.parent->addChild(geode);
        } else {
            qWarning() << "Missing geometry with name" << geo.name;
        }
    }
    unresolvedGeometries.clear();
}

void Machine::UpdateMatrixTransform()
{
    setMatrix(rotateX * rotateY * rotateZ * translate);
}
