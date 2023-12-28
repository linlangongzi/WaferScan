#pragma once

#include <QStandardItem>
#include <QtMath>
#include <Imaging/ImageOptions.h>
#include "Motion/MotionPath.h"

class ScanPlanModel;
class Range;
class Axis;
namespace Motion {
class TrioQueue;
}

class ScanPlan : public QObject
{
    Q_OBJECT

    friend class ScanPlanModel;
    friend class UTDocument;

//    Q_PROPERTY(QString name READ name WRITE setName)

public:
    enum MotionType {
        NONE,
        RECTANGLE,
        CYLINDER,
        DISK,
        CYLINDERROTATION,
        DISKROTATION,
        CAM,
        ANNULUS
    };
    Q_ENUM(MotionType)

public:
    QString name() const;
    bool isFolder() const;
    bool isEditable() const;
    QString pathType() const;
    QVariant options() const;

    QJsonObject toJson() const;

    ScanPlan *child(int index);
    int childrenCount() const;
    QList<ScanPlan *> children() const;

    ScanPlan *findChild(const QString &name);
    bool isRoot() const;
    bool hasValidPath() const;
    QStringList pathError() const;
    QStringList pathWarning() const;
    ScanPlan *root();
    int depth() const;
    qreal timeExpected() const;
    MotionType motionType() const;
    ScanPlan *parent() const;
    ScanPlan *next(ScanPlan *root, bool skipChildren = false);
    QRectF boundingRect(int axisX, int axisY);
    Range boundingRange(int axisIndex);
    ScanPlan *nextValid(ScanPlan *root = nullptr, bool allowSelf = true);
    ImageOptions imageOptionsByIndex(int index);
    MotionAxisList associatedAxes();
    QString planPath() const;

public:
    MotionPath motionPath() const;
    QHash<int, UTSetup> setups() const;
    void updateTotalTime();

public slots:
    void setName(const QString &name);
    void setPathType(const QString &name);
    void setOptions(const QVariant &option);

public:
    explicit ScanPlan(QObject *parent = nullptr);
    void fromJson(const QJsonObject &source);

protected:
    void removeChild(ScanPlan *scan);
    void attachChild(ScanPlan *scan, int index = -1);
    void detachChild(ScanPlan *scan);

private:
    QList<ScanPlan *> m_children;
    QString m_name;
    qreal m_totalTime = 0;
    QString m_pathType;
    QVariant m_pathOptions;
};
