#pragma once

#include "OriginIndicator.h"
#include <QDialog>
#include "ars3d/TriangulatedSurface.h"
#include <Motion/MotionPath.h>
#include <osg/Group>

namespace Ui {
class SurfacePrediction;
}

class SurfacePrediction : public QDialog
{
    Q_OBJECT

public:
    explicit SurfacePrediction(QList<Triangle> tris,QWidget *parent = 0);
    ~SurfacePrediction();

    QVector<QVector3D> GetPath() {return line;}
    QVector<QVector3D> GetDirection() {return direction;}
    void DrawPath(const QVector<QVector3D> &path, QVector<QVector3D> direction = QVector<QVector3D>());
    void DrawMyText(osg::Group *parent, const QString &text, const QVector3D &pos, qreal fontSize = 2);
    bool IsValid() {return isValid;}
private slots:
    void on_update_clicked();
    void on_finish_clicked();

private:
    Ui::SurfacePrediction *ui;

    QList<Triangle> triangles;
    QVector<QVector3D> line;
    QVector<QVector3D> direction;
    bool isValid = false;

    osg::ref_ptr<OriginIndicator> indicator;
    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osg::Group> surfaceMts;
    osg::ref_ptr<osg::Group> scanPath;

};
