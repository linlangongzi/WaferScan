#pragma once

#include "Item3d.h"
#include "ars3d/TriangulatedSurface.h"
#include <osg/MatrixTransform>
#include <QAction>
#include <QVector3D>

class Part : public Item3d
{
    Q_OBJECT
public:
    Part();
    bool CreatePartNode(const QString &path);
    bool CreatePartNodeFromStl(const QString &path);
    bool CreatePartNodeFromCsv(const QString &path);
    bool CreatePartNodeFromCloud(const QVector<QVector3D> &points);
    void SetTranslate(const QVector3D &t);
    void SetRotate(const QVector3D &r);
    QList<Triangle> GetTriangles();

    virtual void ContextMenu(QMenu *menu, const QVector3D &p);

private slots:
    void on_DraggerTranslate();
    void on_DraggerRotate();
    void on_SampleOnPart();
    void on_DialogTranslateRotate();
    void on_RemovePart();
    void on_PathPlanning();

private:
    void UpdateMatrixTransform();

    qreal x = 0;
    qreal y = 0;
    qreal z = 0;
    qreal a = 0;
    qreal b = 0;
    qreal c = 0;
    QAction *actionDraggerTranslate;
    QAction *actionDraggerRotate;
    QAction *actionSampleOnPart;
    QAction *actionPartConfig;
    QAction *actionPathPlanning;
    QAction *actionRemovePart;

    osg::ref_ptr<osg::Geometry> geometry;
//    osg::ref_ptr<osg::Vec3Array> vertexArray;                                   //顶点集合，即所有的三角片元的顶点，每三个构成一个三角片元
//    osg::ref_ptr<osg::Vec3Array> normArray;
    QVector3D samplePoint;
};
