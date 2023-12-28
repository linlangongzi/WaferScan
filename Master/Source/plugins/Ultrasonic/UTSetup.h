#pragma once

#include "UTSetupTimebase.h"
#include "UTSetupGate.h"
#include "InterpolateTable.h"
#include <QColor>
#include <QObject>
#include <QSharedData>
#include <QCoreApplication>
#include "UTData.h"

class ULTRASONIC_EXPORT UTSetup
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(UTSetup)

public:
    enum Impedance {
        ImpedanceLow,
        ImpedanceHigh,
    };
    Q_ENUM(Impedance)

    enum RectifierMode {
        RectifierFull,
        RectifierPositive,
        RectifierNegative,
        RectifierRf,
    };
    Q_ENUM(RectifierMode)

    enum TransmitMode {
        TransmitThrough,
        PulseEcho,
    };
    Q_ENUM(TransmitMode)

    enum TransmitEnergy {
        EnergyLow,
        EnergyHigh,
    };
    Q_ENUM(TransmitEnergy)

    enum DacMode {
        DacDisabled,
        DacEnabled,
        DacTcg,
        DacSoftwareTcg,
    };
    Q_ENUM(DacMode)

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(UTSetupTimebase::TimebaseUnit timebaseUnit READ timebaseUnit WRITE setTimebaseUnit)
    Q_PROPERTY(qreal delay READ delay WRITE setDelay)
    Q_PROPERTY(qreal range READ range WRITE setRange)
    Q_PROPERTY(qreal zero READ zero WRITE setZero)
    Q_PROPERTY(Material material READ material WRITE setMaterial)
    Q_PROPERTY(UTSetupSurfaceTracing surfaceTracing READ surfaceTracing WRITE setSurfaceTracing)
    Q_PROPERTY(bool pulserEnable READ pulserEnable WRITE setPulserEnable)
    Q_PROPERTY(int transmitVoltage READ transmitVoltage WRITE setTransmitVoltage)
    Q_PROPERTY(TransmitMode transmitMode READ transmitMode WRITE setTransmitMode)
    Q_PROPERTY(TransmitEnergy transmitEnergy READ transmitEnergy WRITE setTransmitEnergy)
    Q_PROPERTY(int pulseWidth READ pulseWidth WRITE setPulseWidth)
    Q_PROPERTY(Impedance impedance READ impedance WRITE setImpedance)
    Q_PROPERTY(RectifierMode rectifierMode READ rectifierMode WRITE setRectifierMode)
    Q_PROPERTY(int probeMultiplex READ probeMultiplex WRITE setProbeMultiplex)
    Q_PROPERTY(qreal gain READ gain WRITE setGain)
    Q_PROPERTY(qreal gainRef READ gainRef WRITE setGainRef)
    Q_PROPERTY(bool manualGain READ manualGain WRITE setManualGain)
    Q_PROPERTY(int preAmplify READ preAmplify WRITE setPreAmplify)
    Q_PROPERTY(int amplify1 READ amplify1 WRITE setAmplify1)
    Q_PROPERTY(int amplify2 READ amplify2 WRITE setAmplify2)
    Q_PROPERTY(int probeBand READ probeBand WRITE setProbeBand)
    Q_PROPERTY(int sampleAverage READ sampleAverage WRITE setSampleAverage)
    Q_PROPERTY(qreal suppress READ suppress WRITE setSuppress)
    Q_PROPERTY(int prf READ prf WRITE setPrf)
    Q_PROPERTY(QList<UTSetupGate> gates READ gates WRITE setGates)
    Q_PROPERTY(InterpolateTable dac READ dac WRITE setDac)
    Q_PROPERTY(DacMode dacMode READ dacMode WRITE setDacMode)
    Q_PROPERTY(qreal dacTarget READ dacTarget WRITE setDacTarget)
    Q_PROPERTY(int dacTimebase READ dacTimebase WRITE setDacTimebase)
    Q_PROPERTY(bool alterSource READ alterSource WRITE setAlterSource)

public:
    Q_INVOKABLE UTSetup();
    Q_INVOKABLE UTSetup(const UTSetup &other);

    Q_INVOKABLE QJsonValue toJson() const;
    static UTSetup fromJson(const QJsonValue &value);

    /* 访问器 */
    QString name() const;
    UTSetupTimebase::TimebaseUnit timebaseUnit() const;
    qreal delay() const;
    qreal range() const;
    qreal zero() const;
    Material material() const;
    UTSetupSurfaceTracing surfaceTracing() const;
    bool pulserEnable() const;
    int transmitVoltage() const;
    TransmitMode transmitMode() const;
    TransmitEnergy transmitEnergy() const;
    int pulseWidth() const;
    Impedance impedance() const;
    RectifierMode rectifierMode() const;
    int probeMultiplex() const;
    qreal gain() const;
    qreal gainRef() const;
    bool manualGain() const;
    int preAmplify() const;
    int amplify1() const;
    int amplify2() const;
    int probeBand() const;
    int sampleAverage() const;
    qreal suppress() const;
    int prf() const;
    QList<UTSetupGate> gates() const;
    InterpolateTable dac() const;
    DacMode dacMode() const;
    qreal dacTarget() const;
    int dacTimebase() const;

    /* 突变器 */
    void setName(QString name);
    void setTimebaseUnit(UTSetupTimebase::TimebaseUnit timebaseUnit);
    void setDelay(qreal delay);
    void setRange(qreal range);
    void setZero(qreal zero);
    void setMaterial(const Material &material);
    void setSurfaceTracing(const UTSetupSurfaceTracing &surfaceTracing);
    void setPulserEnable(bool pulserEnable);
    void setTransmitVoltage(int transmitVoltage);
    void setTransmitMode(TransmitMode transmitMode);
    void setTransmitEnergy(TransmitEnergy transmitEnergy);
    void setPulseWidth(int pulseWidth);
    void setImpedance(Impedance impedance);
    void setRectifierMode(RectifierMode rectifierMode);
    void setProbeMultiplex(int probeMultiplex);
    void setGain(qreal gain);
    void setGainRef(qreal gainRef);
    void setManualGain(bool manualGain);
    void setPreAmplify(int preAmplify);
    void setAmplify1(int amplify1);
    void setAmplify2(int amplify2);
    void setProbeBand(int probeBand);
    void setSampleAverage(int sampleAverage);
    void setSuppress(qreal suppress);
    void setPrf(int prf);
    void setGates(const QList<UTSetupGate> &gates);
    void setDac(const InterpolateTable &dac);
    void setDacMode(DacMode dacMode);
    void setDacTarget(qreal dacTarget);
    void setDacTimebase(int dacTimebase);

    /* 额外属性 */
    UTSetupTimebase timebase() const;
    void setTimebase(const UTSetupTimebase &timebase);

    qreal end() const;                                                          /*!< 声程后沿, end == delay + range */
    int gateCount() const;                                                      /*!< 闸门数量 */
    UTSetupGate gate(int index) const;                                          /*!< 获取第 index 个闸门 */
    void setGate(int index, const UTSetupGate &gate);                           /*!< 设置第 index 个闸门 */
    void appendGate(const UTSetupGate &gate);                                   /*!< 向闸门列表添加指定的闸门 */
    void removeGate(int index);                                                 /*!< 移除第 index 个闸门 */
    UTSetupGate suggestGate(int index = -1) const;                              /*!< 推荐第 index 个闸门的默认的闸门配置, index越界时返回推荐追加的闸门 */
    QColor suggestGateColor() const;                                            /*!< 推荐下一个闸门颜色 */

    CDataFrame gateFeatures(const ADataFrame &waveform) const;                  /*!< 对指定波形数据计算特征值 */
    void gateFeature(const ADataFrame &waveform, CDataValue *buffer) const;     /*!< 对指定波形数据计算特征值, 存入指定的 buffer 中. buffer 的尺寸必须>=2倍闸门数 */

    QList<QPair<qreal, qreal> > dacList() const;

    bool alterSource() const {return d->alterSource;}
    void setAlterSource(bool alterSource) {d->alterSource = alterSource;}

    QString materialName() const {return d->timebase.material().name;}
    qreal materialSpeed() const {return d->timebase.material().longitudinalVelocity;}
    QString timebaseSuffix() const {return d->timebase.timebaseSuffix();}
    qreal toTimebaseUnit(qreal v) const {return d->timebase.toTimebaseUnit(v);}
    qreal fromTimebaseUnit(qreal v) const {return d->timebase.fromTimebaseUnit(v);}
    QString timebaseText(qreal v) const {return d->timebase.timebaseText(v);}

    void swap(UTSetup &other);
    static bool registerType();
    bool operator ==(const UTSetup &other) const;
    bool operator !=(const UTSetup &other) const;

private:
    void refreshGateIndex();

    class UTSetupData : public QSharedData
    {
    public:
        QString name;
        UTSetupTimebase timebase;

        bool pulserEnable = 0;
        TransmitMode transmitMode = PulseEcho;
        TransmitEnergy transmitEnergy = EnergyLow;
        int pulseWidth = 50;

        Impedance impedance = ImpedanceHigh;
        RectifierMode rectifierMode = RectifierFull;
        int probeMultiplex = 0;
        qreal gain = 0;
        qreal gainRef = 0;
        bool manualGain = false;
        int preAmplify = 0;
        int amplify1 = 0;
        int amplify2 = 0;
        int probeBand = 0;
        int sampleAverage = 0;
        qreal suppress = 0;

        int prf = 200;
        int transmitVoltage = 50;
        bool alterSource = false;

        QList<UTSetupGate> gates;
        InterpolateTable dac;
        DacMode dacMode = DacDisabled;
        qreal dacTarget = 80;
        int dacTimebase = 0;
    };

    QSharedDataPointer<UTSetupData> d;

    UTSetup(UTSetupData *data) : d(data) {}
    static const UTSetup dummy;
};
