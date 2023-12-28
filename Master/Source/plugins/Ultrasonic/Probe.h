#pragma once

#include "Material.h"
#include "UltrasonicGlobal.h"
#include "DgsTable.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QMatrix4x4>
#include <QUuid>
#include <allrising/MetaType/MetaType.h>
#include <utils/treemodel.h>

class EndPoint;
class UTPose;

struct ULTRASONIC_EXPORT ProbeSize
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(ProbeSize)

    Q_PROPERTY(qreal length MEMBER length)
    Q_PROPERTY(qreal width MEMBER width)
    Q_PROPERTY(qreal height MEMBER height)
    Q_PROPERTY(qreal radius MEMBER radius)

public:
    DECLARE_COMPARE_OPERATORS(ProbeSize)

    qreal length = 0;
    qreal width = 0;
    qreal height = 0;
    qreal radius = 0;
};

/*!
 *  描述探头的信息：形状特征、安装位置（EndPoint class)、频率等
 */
class ULTRASONIC_EXPORT Probe
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(Probe)

public:
    DECLARE_COMPARE_OPERATORS(Probe)

    enum FocalType {
        FocusPoint,
        FocusLine,
        FocusNone,
    };
    Q_ENUM(FocalType)

    enum ProbeType {
        Straight,
        AngleBeam,
    };
    Q_ENUM(ProbeType)

    enum Shape {
        Square,
        Circle,
    };
    Q_ENUM(Shape)

    Q_PROPERTY(QUuid uuid MEMBER uuid)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString manufactureName MEMBER manufactureName)
    Q_PROPERTY(QString modelName MEMBER modelName)
    Q_PROPERTY(QString serialName MEMBER serialName)
    Q_PROPERTY(QDate calibrationDate MEMBER calibrationDate)
    Q_PROPERTY(int frequency MEMBER frequency)
    Q_PROPERTY(qreal focalInch MEMBER focalInch)
    Q_PROPERTY(qreal focalCylinderLength MEMBER focalCylinderLength)
    Q_PROPERTY(qreal crystalSize MEMBER crystalSize)
    Q_PROPERTY(qreal sensitivity MEMBER sensitivity)
    Q_PROPERTY(qreal emissionDelay MEMBER emissionDelay)
    Q_PROPERTY(qreal azimuth MEMBER azimuth)
    Q_PROPERTY(qreal elevation MEMBER elevation)
    Q_PROPERTY(FocalType focalType MEMBER focalType)
    Q_PROPERTY(ProbeType probeType MEMBER probeType)
    Q_PROPERTY(ProbeSize size MEMBER size)
    Q_PROPERTY(Shape shape MEMBER shape)
    Q_PROPERTY(QList<DgsTable> dgsTables MEMBER dgsTables)
    Q_PROPERTY(QUuid defaultDgsTableUuid MEMBER defaultDgsTableUuid)

public:
    static Probe fromJson(const QJsonValue &value);

    void addTransform(QMatrix4x4 &m, const UTPose &pose, qreal waterPath) const;/* 添加变换矩阵 */
    QList<QMatrix4x4> transformChain(const UTPose &pose, qreal waterPath) const;/* 获取变换矩阵 */
    void updateMatrix();                                                        /* 更新探头变换矩阵 */
    qreal pulseDuration() const;                                                /* 得到脉冲宽度 */
    qreal effectFocalPos(qreal materialSpeed, qreal waterPathLength, const QString &type, qreal waterSpeed = 1500) const; /* 得到探头焦点在工件中位置(μs) */
    QString focusText() const;
    QString crystalSizeText() const;
    QString focalTypeText() const;
    static bool registerType();
    DgsTable defaultDgsTable() const;
    void updateTable(const DgsTable &newDgsTable);

private:
    QMatrix4x4 transform;                                                       /* 探头的变换矩阵 */

public:
    QUuid uuid;
    QString name;
    QString manufactureName;
    QString modelName;
    QString serialName;
    QDate calibrationDate;                                                      // 校准时间（使用此探头的时间）
    qreal frequency = 0;                                                        // 频率 (MHz)
    qreal focalInch = 0;                                                        // 焦距
    qreal focalCylinderLength = 0;                                              // 焦柱长度
    qreal crystalSize = 0;                                                      // 晶片大小
    qreal sensitivity = 0;                                                      // 灵敏度
    qreal emissionDelay = 0;                                                    // 发射延时
    qreal azimuth = 0;                                                          // 方向角
    qreal elevation = 0;                                                        // 仰角
    FocalType focalType = FocusPoint;
    ProbeType probeType = Straight;
    Shape shape = Square;
    ProbeSize size;
    QList<DgsTable> dgsTables;
    QUuid defaultDgsTableUuid;
};

class ProbeItem : public Utils::TreeItem
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(ProbeItem)

public:
    enum Columns {
        ColUuid,
        ColName,
        ColManufacture,
        ColFrequency,
        ColFocalType,
        ColFocus,
        ColCrystalSize,
    };
    Q_ENUM(Columns)

    // TreeItem interface
public:
    QVariant data(int column, int role) const override;
    bool setData(int column, const QVariant &data, int role) override;

    Probe probe;
};

using ProbeRootItem = Utils::TypedTreeItem<ProbeItem>;

class ProbeModel : public Utils::TreeModel<ProbeRootItem, ProbeItem>
{
    Q_OBJECT

public:
    ProbeModel(QObject *parent = nullptr);

    void setProbes(const QList<Probe> &probes);
    Q_REQUIRED_RESULT QList<Probe> probes() const;

    Probe probe(const QUuid &uuid);
    bool updateProbe(const Probe &newProbe);

signals:
    void probesChanged();

private:
    ProbeItem *probeItem(const QUuid &uuid);
};
