#pragma once

#include <QUuid>
#include <QVector3D>
#include <QWidget>

namespace Ui {
class PointXYZ;
}

class PointXYZ : public QWidget
{
    Q_OBJECT

public:
    explicit PointXYZ(QWidget *parent = 0);
    ~PointXYZ();

    qreal angle() const;
    void setAngle(qreal angle);
    QUuid axisRotationId() const;
    void setAxisRotationId(const QUuid &id);
    QVector3D nativePos() const;
    void setNativePos(const QVector3D &pos);
    QVector3D displayPos() const;
    void setDisplayPos(const QVector3D &pos);
    void setDisplayPos(double x, double y, double z);
    void setOffset(const QVector3D offset);
    void setOffsetAndRefresh(const QVector3D offset);

public slots:
    void setAxisRotationEnable(bool enable);

signals:
    void pointChanged(const QVector3D &point);

private slots:
    void on_endPointFromTable_clicked();
    void on_getEndpoint_clicked();
    void on_toEndpoint_clicked();
    void onValueChanged();

private:
    Ui::PointXYZ *ui;
    QVector3D m_offset;
    QUuid m_axisRotationId;
    bool m_axisRotationEnable = false;
};
