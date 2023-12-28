#pragma once

#include <QLabel>
#include <QSpinBox>
#include <QVector3D>
#include <QVector4D>
#include <QWidget>

namespace Ui {
class VectorEditor;
}

class VectorEditor : public QWidget
{
    Q_OBJECT

//    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(QVector<qreal> vector READ vector WRITE setVector NOTIFY vectorChanged)
    Q_PROPERTY(QVector3D vector3D READ vector3D WRITE setVector3D NOTIFY vector3DChanged)
    Q_PROPERTY(QVector4D vector4D READ vector4D WRITE setVector4D NOTIFY vector4DChanged)
    Q_PROPERTY(int fixedLength READ fixedLength WRITE setFixedLength)
    Q_PROPERTY(QStringList names READ names WRITE setNames)

public:
    explicit VectorEditor(QWidget *parent = 0);
    ~VectorEditor();

//    Qt::Orientation orientation() const;
    QVector<qreal> vector() const;
    QVector3D vector3D() const;
    QVector4D vector4D() const;
    int fixedLength() const;
    QStringList names() const;

public slots:
//    void setOrientation(Qt::Orientation orientation);
    void setVector(const QVector<qreal> &vector);
    void setVector3D(const QVector3D &vector3D);
    void setVector4D(const QVector4D &vector4D);
    void setFixedLength(int fixedLength);
    void setNames(const QStringList &names);

signals:
    void vectorChanged(QVector<qreal> vector);
    void vector3DChanged(QVector3D vector3D);
    void vector4DChanged(QVector4D vector4D);

private slots:
    void spinBoxChanged();

private:
    void refreshLength(int length);

private:
    Ui::VectorEditor *ui;
//    Qt::Orientation m_orientation;
    int m_fixedLength = 0;
    QList<QPair<QLabel *, QDoubleSpinBox *> > controls;
    QStringList m_names;
    QVector<qreal> m_vector;
    QVector3D m_vector3D;
    QVector4D m_vector4D;
};
