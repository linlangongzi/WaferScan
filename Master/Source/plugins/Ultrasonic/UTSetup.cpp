#include "UTSetup.h"
#include <allrising/Util.h>

static const bool initType = UTSetup::registerType();

const UTSetup UTSetup::dummy(new UTSetup::UTSetupData);

UTSetup::UTSetup() :
    d(dummy.d)
{
}

UTSetup::UTSetup(const UTSetup &other) = default;

QJsonValue UTSetup::toJson() const
{
    if (d == dummy.d) {
        return {};
    }
    return GadgetToJson(UTSetup::staticMetaObject, this);
}

UTSetup UTSetup::fromJson(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    UTSetup setup;
    if (obj.isEmpty()) {
        return setup;
    }
    if (value.toObject().contains("Type")) {                // 兼容旧版本格式
        setup.setName(obj["Name"].toString());
        setup.setGain(obj["Gain"].toDouble());
        setup.setDelay(obj["Delay"].toDouble());
        setup.setGainRef(obj["GainRef"].toDouble());
        setup.setZero(obj["Zero"].toDouble());
        setup.setImpedance(UTSetup::Impedance(obj["Impedance"].toVariant().toInt()));
        setup.setPrf(obj["PRF"].toInt());
        setup.setProbeBand(obj["ProbeBand"].toVariant().toInt());
        setup.setPulseWidth(obj["PulseWidth"].toInt());
        setup.setRange(obj["Range"].toDouble());
        setup.setRectifierMode(UTSetup::RectifierMode(obj["RectifierMode"].toVariant().toInt()));
        setup.setSampleAverage(obj["SampleAverage"].toVariant().toInt());
        for (int index = 1; index <= 4; ++index) {
            UTSetupGate gate;
            if (index > 1) {
                gate.setSyncIndex(0);
            }
            gate.setName(QString("Gate%1").arg(index));
            gate.setColor(QColor(obj[QString("Gate%1Color").arg(index)].toString()));
            gate.setEnabled(obj[QString("Gate%1Enable").arg(index)].toBool());
            gate.setStart(obj[QString("Gate%1Start").arg(index)].toDouble());
            gate.setWidth(obj[QString("Gate%1Width").arg(index)].toDouble());
            gate.setThreshold(obj[QString("Gate%1Amp").arg(index)].toDouble());
            setup.appendGate(gate);
            setup.setGate(index, gate);
        }
        return setup;
    }
    JsonToGadget(value, UTSetup::staticMetaObject, &setup);
    setup.refreshGateIndex();
    return setup;
}

QString UTSetup::name() const
{
    return d->name;
}

UTSetupTimebase::TimebaseUnit UTSetup::timebaseUnit() const
{
    return d->timebase.timebaseUnit();
}

qreal UTSetup::delay() const
{
    return d->timebase.delay();
}

qreal UTSetup::range() const
{
    return d->timebase.range();
}

qreal UTSetup::zero() const
{
    return d->timebase.zero();
}

Material UTSetup::material() const
{
    return d->timebase.material();
}

UTSetupSurfaceTracing UTSetup::surfaceTracing() const
{
    return d->timebase.surfaceTracing();
}

bool UTSetup::pulserEnable() const
{
    return d->pulserEnable;
}

int UTSetup::transmitVoltage() const
{
    return d->transmitVoltage;
}

UTSetup::TransmitMode UTSetup::transmitMode() const
{
    return d->transmitMode;
}

UTSetup::TransmitEnergy UTSetup::transmitEnergy() const
{
    return d->transmitEnergy;
}

int UTSetup::pulseWidth() const
{
    return d->pulseWidth;
}

UTSetup::Impedance UTSetup::impedance() const
{
    return d->impedance;
}

UTSetup::RectifierMode UTSetup::rectifierMode() const
{
    return d->rectifierMode;
}

int UTSetup::probeMultiplex() const
{
    return d->probeMultiplex;
}

qreal UTSetup::gain() const
{
    return d->gain;
}

qreal UTSetup::gainRef() const
{
    return d->gainRef;
}

bool UTSetup::manualGain() const
{
    return d->manualGain;
}

int UTSetup::preAmplify() const
{
    return d->preAmplify;
}

int UTSetup::amplify1() const
{
    return d->amplify1;
}

int UTSetup::amplify2() const
{
    return d->amplify2;
}

int UTSetup::probeBand() const
{
    return d->probeBand;
}

int UTSetup::sampleAverage() const
{
    return d->sampleAverage;
}

qreal UTSetup::suppress() const
{
    return d->suppress;
}

int UTSetup::prf() const
{
    return d->prf;
}

QList<UTSetupGate> UTSetup::gates() const
{
    return d->gates;
}

InterpolateTable UTSetup::dac() const
{
    return d->dac;
}

UTSetup::DacMode UTSetup::dacMode() const
{
    return d->dacMode;
}

qreal UTSetup::dacTarget() const
{
    return d->dacTarget;
}

int UTSetup::dacTimebase() const
{
    return d->dacTimebase;
}

void UTSetup::setName(QString name)
{
    d->name = name;
}

void UTSetup::setTimebaseUnit(UTSetupTimebase::TimebaseUnit timebaseUnit)
{
    d->timebase.setTimebaseUnit(timebaseUnit);
}

void UTSetup::setDelay(qreal delay)
{
    d->timebase.setDelay(delay);
}

void UTSetup::setRange(qreal range)
{
    d->timebase.setRange(range);
}

void UTSetup::setZero(qreal zero)
{
    d->timebase.setZero(zero);
}

void UTSetup::setMaterial(const Material &material)
{
    d->timebase.setMaterial(material);
}

void UTSetup::setSurfaceTracing(const UTSetupSurfaceTracing &surfaceTracing)
{
    d->timebase.setSurfaceTracing(surfaceTracing);
}

void UTSetup::setPulserEnable(bool pulserEnable)
{
    d->pulserEnable = pulserEnable;
}

void UTSetup::setTransmitVoltage(int transmitVoltage)
{
    d->transmitVoltage = transmitVoltage;
}

void UTSetup::setTransmitMode(UTSetup::TransmitMode transmitMode)
{
    d->transmitMode = transmitMode;
}

void UTSetup::setTransmitEnergy(UTSetup::TransmitEnergy transmitEnergy)
{
    d->transmitEnergy = transmitEnergy;
}

void UTSetup::setPulseWidth(int pulseWidth)
{
    d->pulseWidth = pulseWidth;
}

void UTSetup::setImpedance(UTSetup::Impedance impedance)
{
    d->impedance = impedance;
}

void UTSetup::setRectifierMode(UTSetup::RectifierMode rectifierMode)
{
    d->rectifierMode = rectifierMode;
}

void UTSetup::setProbeMultiplex(int probeMultiplex)
{
    d->probeMultiplex = probeMultiplex;
}

void UTSetup::setGain(qreal gain)
{
    d->gain = gain;
}

void UTSetup::setGainRef(qreal gainRef)
{
    d->gainRef = gainRef;
}

void UTSetup::setManualGain(bool manualGain)
{
    d->manualGain = manualGain;
}

void UTSetup::setPreAmplify(int preAmplify)
{
    d->preAmplify = preAmplify;
}

void UTSetup::setAmplify1(int amplify1)
{
    d->amplify1 = amplify1;
}

void UTSetup::setAmplify2(int amplify2)
{
    d->amplify2 = amplify2;
}

void UTSetup::setProbeBand(int probeBand)
{
    d->probeBand = probeBand;
}

void UTSetup::setSampleAverage(int sampleAverage)
{
    d->sampleAverage = sampleAverage;
}

void UTSetup::setSuppress(qreal suppress)
{
    d->suppress = suppress;
}

void UTSetup::setPrf(int prf)
{
    d->prf = prf;
}

void UTSetup::setGates(const QList<UTSetupGate> &gates)
{
    d->gates = gates;
    refreshGateIndex();
}

void UTSetup::setDac(const InterpolateTable &dac)
{
    d->dac = dac;
}

void UTSetup::setDacMode(UTSetup::DacMode dacMode)
{
    d->dacMode = dacMode;
}

void UTSetup::setDacTarget(qreal dacTarget)
{
    d->dacTarget = dacTarget;
}

void UTSetup::setDacTimebase(int dacTimebase)
{
    d->dacTimebase = dacTimebase;
    refreshGateIndex();
}

UTSetupTimebase UTSetup::timebase() const
{
    return d->timebase;
}

void UTSetup::setTimebase(const UTSetupTimebase &timebase)
{
    d->timebase = timebase;
}

qreal UTSetup::end() const
{
    return d->timebase.end();
}

int UTSetup::gateCount() const
{
    return d->gates.count();
}

UTSetupGate UTSetup::gate(int index) const
{
    return d->gates.value(index);
}

void UTSetup::setGate(int index, const UTSetupGate &gate)
{
    if (uint(index) < uint(d->gates.count())) {
        d->gates[index] = gate;
        refreshGateIndex();
    }
}

void UTSetup::appendGate(const UTSetupGate &gate)
{
    d->gates.append(gate);
    refreshGateIndex();
}

void UTSetup::removeGate(int index)
{
    if (uint(index) >= uint(d->gates.count())) {
        return;
    }

    d->gates.removeAt(index);
    for (int i = index; i < d->gates.count(); ++i) {
        UTSetupGate &gate = d->gates[i];
        if (gate.syncIndex() == index) {            /* 指向的闸门被删除 */
            gate.setSyncIndex(-1);
        } else if (gate.syncIndex() > index) {      /* 指向的闸门被前移 */
            gate.m_syncIndex -= 1;
        }
    }
    refreshGateIndex();
}

UTSetupGate UTSetup::suggestGate(int index) const
{
    UTSetupGate gate = d->gates.value(index);
    if (index < 0 || index >= d->gates.count()) {
        index = d->gates.count();
        gate.setColor(suggestGateColor());
        gate.setSyncIndex(d->gates.count() == 0 ? -1 : 0);
        gate.setEnabled(true);
    }

    int col = index % 5;
    int row = (index / 5) + col;
    qreal offset = range() / 20 * row;
    gate.setThreshold(70 - 10 * col);
    gate.setWidth(range() / 3);
    if (gate.hasSync()) {
        gate.setStart(range() / 3 + offset);
    } else {
        gate.setStart(delay() + range() / 3 + offset);
    }

    return gate;
}

QColor UTSetup::suggestGateColor() const
{
    static QList<QColor> defaultColors = {
        Qt::red,
        Qt::green,
        Qt::blue,
        Qt::cyan,
        Qt::magenta,
        Qt::yellow,
        Qt::darkRed,
        Qt::darkGreen,
        Qt::darkBlue,
        Qt::darkCyan,
        Qt::darkMagenta,
        Qt::darkYellow,
    };

    QList<QColor> colors = defaultColors;
    for (const auto &gate: d->gates) {
        colors.removeAll(gate.color());
    }
    if (colors.isEmpty()) {
        return defaultColors[d->gates.count() % defaultColors.count()];
    }
    return colors.first();
}

CDataFrame UTSetup::gateFeatures(const ADataFrame &waveform) const
{
//    CDataFrame frame(d->gates.count() * UTDataHeader::GateFeatureCount);
//    gateFeature(waveform, frame.data());
//    return frame;
    Q_UNUSED(waveform)
    return {};
}

void UTSetup::gateFeature(const ADataFrame &waveform, CDataValue *buffer) const
{
    Q_UNUSED(waveform)
    Q_UNUSED(buffer)
//    const qreal sampleInterval = range() / waveform.size();
//    const qreal timeOffset = delay();
//    const qreal ampScale = 100.0 / UTDataFrame::waveformMax();
//    auto toIndex = [&](qreal time) {
//        return (time - timeOffset) / sampleInterval;
//    };
//    auto fromIndex = [&](int index) {
//        return (index * sampleInterval) + timeOffset;
//    };

//    struct Acquire {
//        int time = 0;
//        int amp = 0;
//        bool isValid = false;
//        inline void set(int t, int a)
//        {
//            time = t;
//            amp = a;
//            isValid = true;
//        }
//    };

//    for (int gateIndex = 0; gateIndex < this->d->gates.count(); ++gateIndex) {  /* VS2013无法编译d->gates.count() */
//        const UTSetupGate &gate = this->d->gates[gateIndex];
//        qreal syncOffset = 0;
//        if (gate.hasSync() && gate.syncIndex() < gateIndex) {
//            syncOffset = buffer[gate.syncIndex() * UTDataHeader::GateFeatureCount + UTDataHeader::GateTime];
//        }
//        const qreal start = gate.start() + syncOffset;
//        int startIndex = qBound<int>(0, toIndex(start), waveform.size() - 1);
//        int endIndex = qBound<int>(0, toIndex(start + gate.width()), waveform.size() - 1);
//        ADataValue threshold = gate.threshold() / ampScale;

//        using M = UTSetupGate;
//        Acquire acquires[M::ModeCount];
//        Acquire &trigger = acquires[M::Trigger];
//        Acquire &max = acquires[M::MaxPeak];
//        Acquire &first = acquires[M::FirstPeak];
//        bool firstCaptured = false;
//        for (int index = startIndex; index <= endIndex; ++index) {
//            ADataValue v = waveform[index];
//            /* 捕获触发 */
//            if (!trigger.isValid && v >= threshold) {
//                trigger.set(index, v);
//            }
//            /* 捕获最大值 */
//            if (!max.isValid || v > max.amp) {
//                max.set(index, v);
//            }
//            /* 捕获首次峰 */
//            if (!firstCaptured && trigger.isValid && (!first.isValid || v > first.amp)) {
//                first.set(index, v);
//            }
//            if (!firstCaptured && v < first.amp) {
//                firstCaptured = true;
//            }
//        }

//        int featureBase = gateIndex * UTDataHeader::GateFeatureCount;
//        const Acquire &timeAcquire = acquires[gate.timeMode()];
//        buffer[featureBase + UTDataHeader::GateTime] = timeAcquire.isValid ? fromIndex(timeAcquire.time) - syncOffset : 0;

//        const Acquire &ampAcquire = acquires[gate.ampMode()];
//        buffer[featureBase + UTDataHeader::GateAmp] = ampAcquire.isValid ? (ampAcquire.amp * ampScale) : 0;
//    }
}

QList<QPair<qreal, qreal> > UTSetup::dacList() const
{
    QList<QPair<qreal, qreal> > list;
    list.reserve(d->dac.size());
    for (auto it = d->dac.begin(); it != d->dac.end(); ++it) {
        list << qMakePair(it.key(), it.value());
    }
    return list;
}

void UTSetup::swap(UTSetup &other)
{
    d.swap(other.d);
}

bool UTSetup::registerType()
{
    registerJsonConverter<UTSetup>();
    registerJsonConverterForType<QHash<int, UTSetup>>();
    MetaType meta(qMetaTypeId<UTSetup>());
    meta.setTr("name", tr("名称"));
    meta.setTr("timebaseUnit", tr("时基单位"));
    meta.setTr("delay", tr("延迟"));
    meta.setTr("range", tr("声程"));
    meta.setTr("zero", tr("零点"));
    meta.setTr("material", tr("材料"));
    meta.setTr("pulserEnable", tr("启用发射"));
    meta.setTr("transmitVoltage", tr("发射电压"));
    meta.setTr("transmitMode", tr("收发模式"));
    meta.setTr("transmitEnergy", tr("发射能量"));
    meta.setTr("pulseWidth", tr("脉冲宽度"));
    meta.setTr("impedance", tr("输入阻抗"));
    meta.setTr("rectifierMode", tr("检波模式"));
    meta.setTr("probeMultiplex", tr("前置探头选择"));
    meta.setTr("gain", tr("增益"));
    meta.setTr("gainRef", tr("参考增益"));
    meta.setTr("manualGain", tr("手工增益控制"));
    meta.setTr("preAmplify", tr("前置放大"));
    meta.setTr("amplify1", tr("1级放大"));
    meta.setTr("amplify2", tr("2级放大"));
    meta.setTr("probeBand", tr("频带"));
    meta.setTr("sampleAverage", tr("采样平均"));
    meta.setTr("suppress", tr("抑制"));
    meta.setTr("prf", tr("重复频率"));
    meta.setTr("gates", tr("闸门"));
    meta.setTr("dac", tr("DAC"));
    meta.setTr("dacMode", tr("DAC模式"));
    meta.setTr("dacTarget", tr("DAC目标波高"));
    meta.setTr("dacTimebase", tr("DAC同步基准"));
    meta.setTr("alterSource", tr("关联检测"));

    MetaType::setTr(ImpedanceLow, tr("低阻抗"));
    MetaType::setTr(ImpedanceHigh, tr("高阻抗"));

    MetaType::setTr(RectifierFull, tr("全波"));
    MetaType::setTr(RectifierPositive, tr("正半波"));
    MetaType::setTr(RectifierNegative, tr("负半波"));
    MetaType::setTr(RectifierRf, tr("射频"));

    MetaType::setTr(TransmitThrough, tr("一发一收"));
    MetaType::setTr(PulseEcho, tr("自发自收"));

    MetaType::setTr(EnergyLow, tr("低能量"));
    MetaType::setTr(EnergyHigh, tr("高能量"));

    MetaType::setTr(DacDisabled, tr("禁用"));
    MetaType::setTr(DacEnabled, tr("DAC"));
    MetaType::setTr(DacTcg, tr("TCG"));
    MetaType::setTr(DacSoftwareTcg, tr("软件TCG"));

    MetaType::registerMeta(meta);

    return true;
}

bool UTSetup::operator ==(const UTSetup &other) const
{
    return d == other.d || MetaType::isGadgetsEqual(staticMetaObject, this, &other);
}

bool UTSetup::operator !=(const UTSetup &other) const
{
    return d != other.d && MetaType::isGadgetsDifferent(staticMetaObject, this, &other);
}

void UTSetup::refreshGateIndex()
{
    for (int index = 0; index < d->gates.count(); ++index) {
        UTSetupGate &gate = d->gates[index];
        gate.m_index = index;
        if (gate.m_syncIndex >= index) {                                        /* 不得跟随之后的闸门 */
            gate.m_syncIndex = -1;
        }
    }
    if (d->dacTimebase >= d->gates.count()) {
        d->dacTimebase = d->gates.isEmpty() ? -1 : 0;
    }
}
