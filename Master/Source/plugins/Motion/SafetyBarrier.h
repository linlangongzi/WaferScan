#pragma once

#include "SafetyBarrierOptions.h"
#include <QObject>
#include <QPointer>
#include <allrising/Range.h>

class Axis;

class MOTION_EXPORT SafetyBarrier : public QObject
{
    Q_OBJECT

    Q_PROPERTY(SafetyBarrierOptions options READ options WRITE setOptions)
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)

public:
    enum Status {
        NotConnected = 0,
        AxisMissing,
        Disabled,
        ExternalSafe,                                                           // 外面且Z轴在空走高度范围内
        ExternalUnsafe,                                                         // 外面且z轴在空走高度范围之外
        Upper,                                                                  // 上面
        Under                                                                   // 下面
    };
    Q_ENUM(Status)

public:
    explicit SafetyBarrier(QObject *parent = nullptr);

    Status status();
    void setOptions(const SafetyBarrierOptions &options);
    QPolygonF GetSafePolygon(int xAxisID, int yAxisID);

    SafetyBarrierOptions options() const;

signals:
    void statusChanged(Status status);
    void SafeRectChanged();

public slots:

    void setStatus(Status status);

private slots:
    void onAxisListChanged();
    void OnMotionChanged();
    void OnMotionConnectChanged();

private:
    void Init();
    bool IsEqual(double x, double y);
    Status GetSafeStatusFromPos(double x, double y, double z);

    Range GetAxisSafeRange(Axis *axis);
    QPolygonF GetSafePolygonWithZAxis(Axis *axisX, Axis *axisY);
    QPolygonF GetSafePolygonWithoutZAxis(Axis *axisX, Axis *axisY);

private:
    QPointer<Axis> xAxis;
    QPointer<Axis> yAxis;
    QPointer<Axis> zAxis;
    double oldZFSLimit = Q_INFINITY;
    double oldXFSLimit = Q_INFINITY;
    double oldXRSLimit = -Q_INFINITY;
    double oldYFSLimit = Q_INFINITY;
    double oldYRSLimit = -Q_INFINITY;
    const double MARGIN_SAFERECT_SENT = 0.1;
    Status m_status = NotConnected;
    SafetyBarrierOptions m_options;
};
