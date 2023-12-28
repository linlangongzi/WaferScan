#include "Part.h"
#include "ui_DialogPartConfig.h"
#include <osg/Geometry>
#include <osg/Material>
#include <QFile>
#include <QFileInfo>
#include <QMenu>
#include <QProgressDialog>
#include <QPushButton>
#include <QString>
#include <QTextStream>
#include <QVBoxLayout>
#include <QtMath>
#include <osgUtil/DelaunayTriangulator>
#include "PathPlanning.h"
#include "PathPlanningWizard.h"
#include "qtosgUtils.h"

using namespace osg;

Part::Part()
{
    actionDraggerTranslate = new QAction(tr("平移"), this);
    actionDraggerRotate = new QAction(tr("旋转"), this);
    actionSampleOnPart = new QAction(tr("在工件上采样"), this);
    actionPartConfig = new QAction(tr("位姿配置"), this);
    actionPathPlanning = new QAction(tr("对此工件进行路径规划"), this);
    actionRemovePart = new QAction(tr("清除该工件"), this);

    connect(actionDraggerTranslate, &QAction::triggered, this, &Part::on_DraggerTranslate, Qt::QueuedConnection);
    connect(actionDraggerRotate, &QAction::triggered, this, &Part::on_DraggerRotate, Qt::QueuedConnection);
    connect(actionSampleOnPart, &QAction::triggered, this, &Part::on_SampleOnPart, Qt::QueuedConnection);
    connect(actionPartConfig, &QAction::triggered, this, &Part::on_DialogTranslateRotate, Qt::QueuedConnection);
    connect(actionPathPlanning, &QAction::triggered, this, &Part::on_PathPlanning, Qt::QueuedConnection);
    connect(actionRemovePart, &QAction::triggered, this, &Part::on_RemovePart, Qt::QueuedConnection);
}
/*!
 * \brief Part::CreatepartNode  创建工件对象，根据文件后缀，选择合适的方式，支持STL和点云
 * \param path
 * \return
 */
bool Part::CreatePartNode(const QString &path)
{
    QFileInfo dir(path);
    QString fileName = dir.suffix();                                            //文件扩展名
    if (fileName.toLower() == "stl") {
        return CreatePartNodeFromStl(path);
    } else if (fileName.toLower() == "csv") {
        return CreatePartNodeFromCsv(path);
    }
    return false;
}

bool Part::CreatePartNodeFromStl(const QString &path) {
    QFile fileSTL(path);
    if (!fileSTL.open(QIODevice::ReadOnly)) {
        return false;
    }
    QProgressDialog progress;
    progress.setLabelText(QString("正在读取STL文件：%1").arg(path));
    progress.setWindowModality(Qt::ApplicationModal);
    if (fileSTL.size() > 3 * 1024000) {                                            //文件大于3M的话，需要显示读取进度
        progress.setMinimumDuration(0);
    }
    qDebug() << "STL file size is: " << fileSTL.size();

    QTextStream stream(&fileSTL);

    ref_ptr<Geometry> geometryTriangle = new Geometry;                          //三角片元几何

    ref_ptr<Vec3Array> vertexArray = new Vec3Array;                                                //顶点集合，即所有的三角片元的顶点，每三个构成一个三角片元
    geometryTriangle->setVertexArray(vertexArray.get());
    ref_ptr<Vec3Array> normArray = new Vec3Array;                               //法向量集合
    geometryTriangle->setNormalArray(normArray.get());
    normArray->setBinding(Array::BIND_PER_VERTEX);
    geometryTriangle->setNormalBinding(Geometry::BIND_PER_VERTEX);
    this->geometry = geometryTriangle;

    /* 处理stlname */
    QString line = stream.readLine().toLocal8Bit();
    QString name = line.mid(6);                                                 //从第6位字符开始为stdyywr名
    qDebug() << "stl file name is: "<< name;
    line = stream.readLine();

    QString csvName = QString("blade.csv");
    QFile csvFile(csvName);
    if (!csvFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QTextStream streamOut(&csvFile);


    /* 提取三角片元 */
    while (!stream.atEnd() && line != "endsolid" && !progress.wasCanceled()) {
        if (fileSTL.pos() < fileSTL.size()) {
            progress.setValue(qreal(fileSTL.pos()) / qreal(fileSTL.size()) * 100);
        }
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
            streamOut << point.x() << "," <<
                        point.y() << "," <<
                        point.z() << endl;

        }
        line = stream.readLine();
    }
    qDebug() << "Triangle number is " << vertexArray->size() / 3 << ", " << normArray->size();
    geometryTriangle->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, vertexArray->size()));

    ref_ptr<Geode> stlNode = new Geode;
    stlNode->addDrawable(geometryTriangle.get());
//    stlNode->setName(name.toStdString());
    addChild(stlNode.get());
    return true;
}

bool Part::CreatePartNodeFromCsv(const QString &path)
{
    QFile fileSTL(path);
    if (!fileSTL.open(QIODevice::ReadOnly)) {
        return false;
    }
    QProgressDialog progress;
    progress.setLabelText(QString("正在读取仿形数据：%1").arg(path));
    progress.setWindowModality(Qt::ApplicationModal);
    if (fileSTL.size() > 3 * 1024000) {                                            //文件大于3M的话，需要显示读取进度
        progress.setMinimumDuration(0);
    }
    QTextStream csv(&fileSTL);

    //从csv中读取点云数据
    QVector<QVector3D> points;
    while (!csv.atEnd()) {
        QString line = csv.readLine();
        QStringList stringList = line.split(",");
        QVector3D point;
        for (int i = 0; i < 3; ++i) {
            point[i] = stringList.value(i).toDouble();
        }
        points.append(point);
    }
    return CreatePartNodeFromCloud(points);
}

bool Part::CreatePartNodeFromCloud(const QVector<QVector3D> &points)
{
    if (points.isEmpty()) {
        return false;
    }
    qDebug() << "Creating input array";
    ref_ptr<Vec3Array> vertex = new Vec3Array(points.size());
    std::transform(points.begin(), points.end(), vertex->begin(), &toVec3);

    /* 进行delaunay三角片元化 */
    ref_ptr<osgUtil::DelaunayTriangulator> delaunayTri = new osgUtil::DelaunayTriangulator;
    delaunayTri->setInputPointArray(vertex);
    delaunayTri->setOutputNormalArray(new Vec3Array);

    qDebug() << "Triangulating...";
    delaunayTri->triangulate();                                                 //进行三角片元化后，points的顺序和实际尺寸均会发生改变
    qDebug() << "points size: " << vertex->size();
    qDebug() << "norm size: " << delaunayTri->getOutputNormalArray()->size();

    /*提取delaunay三角片元中的三角形及法向量 */
    ref_ptr<Geometry> geometry = new Geometry;
    geometry->setVertexArray(vertex);
    geometry->setNormalArray(delaunayTri->getOutputNormalArray());
    geometry->setNormalBinding(Geometry::BIND_PER_PRIMITIVE_SET);
    geometry->addPrimitiveSet(delaunayTri->getTriangles());
    this->geometry = geometry;

    ref_ptr<Geode> stlNode = new Geode;
    stlNode->addDrawable(geometry.get());
    addChild(stlNode.get());
    return true;
}

void Part::SetTranslate(const QVector3D &t)
{
    x = t.x();
    y = t.y();
    z = t.z();
    UpdateMatrixTransform();
}

/*!
 * \brief Part::SetRotate
 * \param r             分别为(a, b, c)的角度值（角度制）
 */
void Part::SetRotate(const QVector3D &r)
{
    a = r.x();
    b = r.y();
    c = r.z();
    UpdateMatrixTransform();
}

QList<Triangle> Part::GetTriangles()
{
    if (!geometry) {
        return QList<Triangle>();
    }
    Matrix matrix = getMatrix();
    /* 获取三角片元，并进行坐标变换 */
    QList<Triangle> tris;
    ref_ptr<Vec3Array> vertexArray = static_cast<Vec3Array *>(geometry->getVertexArray());
    ref_ptr<Vec3Array> normArray = static_cast<Vec3Array *>(geometry->getNormalArray());
    for (PrimitiveSet *primitive: geometry->getPrimitiveSetList()) {
        if (primitive->getMode() != PrimitiveSet::TRIANGLES) {
            continue;
        }
        DrawElements *drawElements = primitive->getDrawElements();
        if (!drawElements || drawElements->getDataType() != GL_UNSIGNED_INT) {
            continue;
        }
        for (uint index = 0; index < drawElements->getNumIndices(); index += 3) {
            uint i1 = drawElements->getElement(index);
            uint i2 = drawElements->getElement(index + 1);
            uint i3 = drawElements->getElement(index + 2);
            Triangle tri;
            tri.v1 = toVector3D(vertexArray->at(i1) * matrix);
            tri.v2 = toVector3D(vertexArray->at(i2) * matrix);
            tri.v3 = toVector3D(vertexArray->at(i3) * matrix);
            tri.normal = QVector3D::crossProduct(tri.v1 - tri.v2, tri.v2 - tri.v3);
//            tri.normal = toVector3D(normArray->at(index / 3));
            tri.normal.normalize();
            if (tri.normal.z() < 0) {
                tri.normal = -tri.normal;
            }
            tris << tri;
        }
    }
    return tris;
}

void Part::ContextMenu(QMenu *menu, const QVector3D &p)
{
    samplePoint = p;
    menu->addAction(actionSampleOnPart);
    menu->addAction(actionRemovePart);
    menu->addAction(actionDraggerTranslate);
    menu->addAction(actionDraggerRotate);
    menu->addAction(actionPartConfig);
    menu->addAction(actionPathPlanning);
}

void Part::on_DraggerTranslate()
{
    AddDragger(Item3d::TranslateDragger);
}

void Part::on_DraggerRotate()
{
    AddDragger(Item3d::TrackballDragger);
}

void Part::on_SampleOnPart()
{
    qDebug() << "Sample on part" << samplePoint;
}

void Part::on_DialogTranslateRotate()
{
    QDialog dialog(QApplication::activeWindow());
    Ui::DialogPartConfig ui;
    ui.setupUi(&dialog);
    ui.x->setRange(-2000,2000);
    ui.y->setRange(-2000,2000);
    ui.z->setRange(-2000,2000);
    ui.a->setRange(-360, 360);
    ui.b->setRange(-360, 360);
    ui.c->setRange(-360, 360);

    ui.x->setValue(x);
    ui.y->setValue(y);
    ui.z->setValue(z);
    ui.a->setValue(a);
    ui.b->setValue(b);
    ui.c->setValue(c);

    if (dialog.exec() == QDialog::Accepted) {
        x = ui.x->value();
        y = ui.y->value();
        z = ui.z->value();
        a = ui.a->value();
        b = ui.b->value();
        c = ui.c->value();
        UpdateMatrixTransform();
    }
}

void Part::on_PathPlanning()
{
    QList<Triangle> tris = GetTriangles();
    PathPlanningWizard p(tris);                                                 //打开路径规划对话框
    p.exec();
}

void Part::on_RemovePart()
{
    setNodeMask(0);
}

void Part::UpdateMatrixTransform()
{
    Matrix matrix;
    matrix *= Matrix::rotate(qDegreesToRadians(a), Vec3d(1, 0, 0));
    matrix *= Matrix::rotate(qDegreesToRadians(b), Vec3d(0, 1, 0));
    matrix *= Matrix::rotate(qDegreesToRadians(c), Vec3d(0, 0, 1));
    matrix *= Matrix::translate(x, y, z);
    setMatrix(matrix);
}
