#pragma once

#include "EndPoint.h"
#include "MotionGlobal.h"
#include "MotionPose.h"
#include "SafetyBarrier.h"
#include "TrioDriver.h"
#include <QObject>
#include <Ultrasonic/UTData.h>
#include "NamedPosition.h"
#include <utils/treemodel.h>

class Axis;

class MOTION_EXPORT MotionManager : public QObject
{
    Q_OBJECT

public:
    explicit MotionManager(QObject *parent = nullptr);
    ~MotionManager() override;

    static MotionManager *instance();

    void importLegacyConfig();
    void loadConfig();
    void saveConfig();
    void loadAxisConfig(const QString &path);
    void loadEndPointConfig(const QString &path);

    static int axisCount();
    static QList<Axis *> axisList();
    static Axis *axis(const QUuid &uuid);
    static Axis *axis(int index);
    static Axis *findAxis(const QString &key, const QVariant &value);
    static QStringList axisNames();
    static int axisIndex(const QUuid &uuid);
    static bool isEqualWith(qreal value, qreal target);
    static int displayPrecision();

    void loadAxisList(const QJsonArray &json);
    void addAxis(Axis *axis);
    void removeAxis(Axis *axis);

    static QList<EndPoint *> endPoints();
    static EndPoint *endPoint(int index);
    static EndPoint *endPoint(const QUuid &id);

    void addEndPoint(EndPoint *endPoint);
    void removeEndPoint(EndPoint *endPoint);

    static QPair<QVector3D, QVector3D> robotPosition(const UTDataFrame &frame);

    static NamedPositionModel *positionModel();

    static Motion::TrioDriver *driver();
    static SafetyBarrier *safetyBarrier();

    static QVector<float> fillPose(const qint32 data[], int size, const QVector<float> &offset, quint32 *direction);                /* 使用指定的原始数据填充当前 UTPose */
    void simulateDynamicConnection(bool enable);

signals:
    void axisListChanged();
    void endPointsChanged();

private:
    static QUuid allocateUuid();
    void updateAxisList();
    void setStandardPose(const MotionPose &pose);

private:
    int m_displayPrecision = 2;
    qreal m_axisPrecision = 0.01;
    QList<Axis *> m_axisList;
    QList<EndPoint *> m_endpoints;
    EndPoint *m_globalEndpoint = nullptr;
    Motion::TrioDriver *m_trio = nullptr;
    SafetyBarrier *m_safetyBarrier = nullptr;
    NamedPositionModel m_positionModel;
};
