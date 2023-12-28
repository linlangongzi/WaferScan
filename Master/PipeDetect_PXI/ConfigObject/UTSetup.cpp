#include "UTSetup.h"
#include "UTChannel.h"
#include "Material.h"
#include "Platform/Platform.h"
#include <QDebug>

UTSetup::UTSetup(QObject *parent) :
    Config(parent),
    m_waveType(0),
    MAX_HEIGHT(1023.0),
    MIN_PRF(100),
    MAX_PRF(16000),
    MIN_GATE_SYNC_THRESHOLD(0.0),    // 应该为所占的比率
    MAX_GATE_SYNC_THRESHOLD(100.0),
    MIN_GATE_AMP(0.0),   // 应该为所占的比率
    MAX_GATE_AMP(100.0),
    MIN_GATE_WIDTH(2.0),
    MIN_GATE_START(0.0),
    MIN_SAMPLE_AVERAGE(0),   // 下位机接收的为1,2,4 ,8,16,32,64
    MAX_SAMPLE_AVERAGE(7),
    MIN_DELAY(0.0),
    MIN_RANGE(1.0),
    MAX_RANGE(999999.0),
    GATE_START_LIMIT(0x7FFFFFFF),
    MIN_TCG_GAIN(0.0),
    MAX_TCG_GAIN(80.0)
{
    SetRaw("Type", "UTSetup");
    Init();
    connect(this, SIGNAL(ValueChanged(QString,QJsonValue)), this, SLOT(ActionValueChanged(QString,QJsonValue)));
}

UTSetup::~UTSetup()
{

}

/**
 * @brief UTSetup::UnbindChannel
 * @param utchannel
 * @return
 * 解除与通道的绑定
 */
bool UTSetup::UnbindChannel(UTChannel *utchannel)
{
    channels.append(utchannel);
    return true;
}

QString UTSetup::GetWaveType() const
{
    QString str;
    switch (m_waveType)
    {
    case 0: str = "Surface";                                            break;
    case 1: str = "Flaw";                                               break;
    default: qDebug() << "UTSetup::GetWaveType(): WaveType error !";    break;
    }

    return str;
}

QJsonObject UTSetup::RequestRefresh()
{
    return ToJson();
}

void UTSetup::RequestUpdate(const QString &key)
{
    if (key == "Delay")
    {
        QString str = GetWaveType();
        QString fullKey(str.append(key));
        QJsonValue value = Get(fullKey);
        qDebug() << "Request " << fullKey << "=" << value;
        emit RequestUpdateAnswer(key, value);
    }
    else
    {
        emit RequestUpdateAnswer(key, Get(key));
    }
}

/**
 * @brief UTSetup::SetParameter
 * @param key
 * @param value
 * 给UI提供的设置超声工艺的槽函数
 */
void UTSetup::SetParameter(const QString &key, const QJsonValue &value)
{
    Set(key, value);
}

void UTSetup::ActionValueChanged(const QString &key, const QJsonValue &value)
{
    if (key == GetWaveType().append("Delay"))
    {
        emit ValueChangedSetup("Delay", value);
    }
    else if (key == GetWaveType().append("Suppress"))
    {
        emit ValueChangedSetup("Suppress", value);
    }
    else if (key == GetWaveType().append("RectifierMode"))
    {
        emit ValueChangedSetup("RectifierMode", value);
    }
    else
    {
        emit ValueChangedSetup(key, value);
    }
}

/**
 * @brief UTSetup::BindChannel
 * @param utchannel
 * @return
 * 与通道进行绑定
 */
bool UTSetup::BindChannel(UTChannel *utchannel)
{
    int pos = channels.indexOf(utchannel);
    if (pos >= 0)
    {
        channels.removeAt(pos);
    }
    return true;
}

ConfigObject* UTSetup::New(QObject *parent) const
{
    UTSetup *utSetup = new UTSetup(parent);
    return dynamic_cast<ConfigObject *>(utSetup);
}

/**
 * @brief UTSetup::Init
 * 设置默认情况下超声工艺的参数
 */
void UTSetup::Init()
{
    Set("NumGates", QJsonValue(4));             // 闸门个数
/*    Set("Reset", QJsonValue(0));                // 通道软复位
    Set("PRF", QJsonValue(390));                // 重复频率计数器
    Set("PulseWidth", QJsonValue(5));           // 脉宽
    Set("ZeroOffset", QJsonValue(0));           // 零点修正监视器
    Set("DACEnable", QJsonValue(false));        // DAC使能开关
    Set("GainEnable", QJsonValue(1));           // 增益转换开关
    Set("GainRefEnable", QJsonValue(1));        // 增益控制基准
    Set("Range", QJsonValue(100.0));            // 声程
    Set("SampleAverage", QJsonValue(0));        // 通道采样平均数
    Set("Velocity", QJsonValue(1000));           // 声速
    Set("ProbeBand", QJsonValue(3));            // 探头频率
    Set("TransmitVoltage", QJsonValue(400));    // 电压
    Set("Impedance", QJsonValue(0));            // 阻抗
    Set("Gain", QJsonValue(0));                 // 增益
    Set("GainRef", QJsonValue(0));              // 参考增益
    Set("TransmitEnergy", QJsonValue(1));       // 高低能量切换，0为低能量，1为高能量,分通道
    Set("TransmitMode", QJsonValue(1));         // 收发模式切换，0为一发一收模式，1为自发自收模式

    Set("FlawDelay", QJsonValue(3));                    // 通道延迟时间
    Set("FlawRectifierMode", QJsonValue(2));            // 检波模式-底波
    Set("FlawSuppress", QJsonValue(0));                 // 抑制-底波
    Set("FlawGate1Amp", QJsonValue(30));                // 闸门1幅值
    Set("FlawGate2Amp", QJsonValue(30));                // 闸门2幅值
    Set("FlawGate3Amp", QJsonValue(30));                // 闸门3幅值
    Set("FlawGate4Amp", QJsonValue(30));                // 闸门4幅值
    Set("FlawGate1Start", QJsonValue(750));             // 闸门1起点
    Set("FlawGate2Start", QJsonValue(750));             // 闸门2起点
    Set("FlawGate3Start", QJsonValue(750));             // 闸门3起点
    Set("FlawGate4Start", QJsonValue(750));             // 闸门4起点
    Set("FlawGate1Width", QJsonValue(1050));            // 闸门1宽度
    Set("FlawGate2Width", QJsonValue(1050));            // 闸门2宽度
    Set("FlawGate3Width", QJsonValue(1050));            // 闸门3宽度
    Set("FlawGate4Width", QJsonValue(1050));            // 闸门4宽度
    Set("FlawGate1Enable", QJsonValue(1));              // 闸门1使能
    Set("FlawGate2Enable", QJsonValue(0));              // 闸门2使能
    Set("FlawGate3Enable", QJsonValue(0));              // 闸门3使能
    Set("FlawGate4Enable", QJsonValue(0));              // 闸门4使能
    Set("FlawGate1SyncMode", QJsonValue(0));            // 闸门1同步模式
    Set("FlawGate2SyncMode", QJsonValue(1));            // 闸门2同步模式
    Set("FlawGate3SyncMode", QJsonValue(1));            // 闸门3同步模式
    Set("FlawGate4SyncMode", QJsonValue(1));            // 闸门4同步模式
    Set("FlawGate1SyncThreshold", QJsonValue(400));     // 闸门1同步阈值
    Set("FlawGate2SyncThreshold", QJsonValue(400));     // 闸门2同步阈值
    Set("FlawGate3SyncThreshold", QJsonValue(400));     // 闸门3同步阈值
    Set("FlawGate4SyncThreshold", QJsonValue(400));     // 闸门4同步阈值

    Set("SurfaceDelay", QJsonValue(3));                 // 通道延迟时间
    Set("SurfaceSuppress", QJsonValue(0));              // 抑制-界面波
    Set("SurfaceRectifierMode", QJsonValue(2));         // 检波模式-界面波
    Set("SurfaceGate1Amp", QJsonValue(30));             // 闸门1幅值
    Set("SurfaceGate2Amp", QJsonValue(30));             // 闸门2幅值
    Set("SurfaceGate3Amp", QJsonValue(30));             // 闸门3幅值
    Set("SurfaceGate4Amp", QJsonValue(30));             // 闸门4幅值
    Set("SurfaceGate1Start", QJsonValue(750));          // 闸门1起点
    Set("SurfaceGate2Start", QJsonValue(750));          // 闸门2起点
    Set("SurfaceGate3Start", QJsonValue(750));          // 闸门3起点
    Set("SurfaceGate4Start", QJsonValue(750));          // 闸门4起点
    Set("SurfaceGate1Width", QJsonValue(1050));         // 闸门1宽度
    Set("SurfaceGate2Width", QJsonValue(1050));         // 闸门2宽度
    Set("SurfaceGate3Width", QJsonValue(1050));         // 闸门3宽度
    Set("SurfaceGate4Width", QJsonValue(1050));         // 闸门4宽度
    Set("SurfaceGate1Enable", QJsonValue(1));           // 闸门1使能
    Set("SurfaceGate2Enable", QJsonValue(0));           // 闸门2使能
    Set("SurfaceGate3Enable", QJsonValue(0));           // 闸门3使能
    Set("SurfaceGate4Enable", QJsonValue(0));           // 闸门4使能
    Set("SurfaceGate1SyncMode", QJsonValue(0));         // 闸门1同步模式
    Set("SurfaceGate2SyncMode", QJsonValue(1));         // 闸门2同步模式
    Set("SurfaceGate3SyncMode", QJsonValue(1));         // 闸门3同步模式
    Set("SurfaceGate4SyncMode", QJsonValue(1));         // 闸门4同步模式
    Set("SurfaceGate1SyncThreshold", QJsonValue(400));  // 闸门1同步阈值
    Set("SurfaceGate2SyncThreshold", QJsonValue(400));  // 闸门2同步阈值
    Set("SurfaceGate3SyncThreshold", QJsonValue(400));  // 闸门3同步阈值
    Set("SurfaceGate4SyncThreshold", QJsonValue(400));  // 闸门4同步阈值
    */
}

QJsonArray UTSetup::GetTCGJsonArray()
{
    // 需修改
    m_TCGJsonArray = Get("TCG").toArray();
    return m_TCGJsonArray;
}

void UTSetup::AddTCGItem(const QJsonValue &time, const QJsonValue &gain, QJsonValue array)
{
    if (array.toDouble() == 0)
    {
        m_TCGJsonArray = GetTCGJsonArray();
    }
    m_TCGJsonArray = array.toArray();
    int count = m_TCGJsonArray.size();
    int index = 0;
    for (index = 0; index < count; ++index)
    {
        if (m_TCGJsonArray[index].toObject()["Time"].toDouble() > time.toDouble())
        {
            break;
        }
    }

    QJsonObject obj;
    obj.insert("Time", time);
    double validTCGGain = qBound(MIN_TCG_GAIN, gain.toDouble(), MAX_TCG_GAIN);
    obj.insert("Gain", validTCGGain);
    QJsonValue TCGValue(obj);

    m_TCGJsonArray.insert(index, TCGValue);

    Set("TCG", m_TCGJsonArray);
}

void UTSetup::ModifyTCGJsonArrayItem(const QJsonValue &time, const QJsonValue &gain, int index)
{
    m_TCGJsonArray = GetTCGJsonArray();
    m_TCGJsonArray.removeAt(index);

    AddTCGItem(time, gain, m_TCGJsonArray);
}

void UTSetup::RemoveTCGItemAt(int index)
{
     m_TCGJsonArray = GetTCGJsonArray();
     m_TCGJsonArray.removeAt(index);
     Set("TCG", m_TCGJsonArray);
}

/**
 * @brief UTSetup::Set 重载父类的Set方法
 * @param key  键名称
 * @param value  键值
 */
void UTSetup::Set(const QString &key, const QJsonValue &value)
{
    if (key == "NumGates")      // 闸门总数
    {
        int numGates = value.toInt();
        return CheckNumGates(numGates);
    }
    else if (key == "Delay")	     //通道延迟时间  >3
    {
        return CheckDelay(key, value.toDouble());
    }
    else if (key == "Range")   //声程 1-1000  因为ADFREQUENCY变成400，所以最大值翻4倍,原ADFREQUENCY为100
    {
        double range = value.toDouble();
        double minRange = GetMaxGateEnd() - Get(GetWaveType().append("Delay")).toDouble();
        range = qMax(MIN_RANGE, range);      /* 不能小于声程的设定的最小值（硬件的设定范围）*/
        range = qMax(minRange, range);      /* 图形显示时不能小于其允许的最小值 */
        range = qMin(MAX_RANGE, range);
        emit ParameterLimit(key, range);
        return Config::Set(key, range);
    }
    else if (key.startsWith("Gate"))
    {
        return CheckGate(key, value);
    }
    else if (key == "PRF")  // 重复频率计数器 老代码中设置( 500-4k) 在当前代码中设置为100-16k
    {
        if (value.toInt() < MIN_PRF)
        {
            emit ParameterLimit(key, MIN_PRF);
            return Config::Set(key, MIN_PRF);
        }
        if (value.toInt() > MAX_PRF)
        {
            emit ParameterLimit(key, MAX_PRF);
            return Config::Set(key, MAX_PRF);
        }
    }
    else if (key == "SampleAverage")   //通道采样平均数 1-64	因为ADFREQUENCY变成400，所以最大值翻4倍，原ADFREQUENCY为100
    {
        if (value.toInt() < MIN_SAMPLE_AVERAGE)
        {
            emit ParameterLimit(key, MIN_SAMPLE_AVERAGE);
            return Config::Set(key, MIN_SAMPLE_AVERAGE);
        }
        if (value.toInt() > MAX_SAMPLE_AVERAGE)
        {
            emit ParameterLimit(key, MAX_SAMPLE_AVERAGE);
            return Config::Set(key ,MAX_SAMPLE_AVERAGE);
        }
    }
    else if (key == "Gain")     // 增益 Gain + GainRef <= 120
    {
        int GainNum = value.toDouble();
        int GainRefNum = Get("GainRef").toDouble();
        if (GainNum + GainRefNum > 120.0)
        {
            GainNum = 120.0 - GainRefNum;
            emit ParameterLimit(key, GainNum);
            return Config::Set(key, GainNum);
        }
    }
    else if (key == "GainRef")  // 参考增益
    {
        double GainRefNum = value.toDouble();
        double GainNum = Get("Gain").toDouble();
        if (GainNum + GainRefNum > 120.0)
        {
            GainRefNum = 120.0 - GainNum;
            emit ParameterLimit(key, GainRefNum);
            return Config::Set(key, GainRefNum);
        }
    }
    else if (key == "TransmitEnergy" || key == "TransmitMode")  // 能量转换和模式转换
    {
        switch (value.toInt())   // 只有0,1两种模式的设置
        {
        case 0:     return Config::Set(key, value);
        case 1:     return Config::Set(key, value);
        default:    return;
        }
    }

    Config::Set(key, value);
}

/**
 * @brief UTSetup::GetNumGates
 * @return 返回闸门总数量
 */
int UTSetup::GetNumGates()
{
    return Get("NumGates").toInt();
}

Material *UTSetup::GetMaterial()
{
    return platform()->Materials()->GetByID<Material>(GetAsID("MaterialID"));
}

double UTSetup::GetMaterialSpeed()
{
    Material *material = GetMaterial();
    return material ? material->Get("MaterialVelocity").toDouble() : 1000;
}

QString UTSetup::GetMaterialName()
{
    Material *material = GetMaterial();
    return material ? material->Get("MaterialName").toString() : QString();
}

double UTSetup::GetDelay()
{
    return GetParameter("Delay");
}

double UTSetup::GetRange()
{
    return GetParameter("Range");
}

double UTSetup::GetParameter(const QString &key)
{
    QString tkey = key;
    QString mode = Get(tkey.append("Unite")).toString("DISTANCE");
    double value = 0;

    if (key == "Delay")
    {
        value = Get(GetWaveType().append(key)).toDouble();
    }
    else
    {
        value = Get(key).toDouble();
    }

    if (mode == "TIME")
    {
        return value;
    }
    ConfigID materialID = GetAsID("MaterialID");
    Material *material = platform()->Materials()->GetByID<Material>(materialID);
    double velocity = material->Get("MaterialVelocity").toDouble();
    value = value * 1000 / velocity;
    return value;
}

/**
 * @brief UTSetup::GetGate
 * @param gateIndex  对应的闸门索引
 * @return
 * 返回对应闸门指针
 */
UTGate *UTSetup::GetGate(int gateIndex)
{
    return gates.takeAt(gateIndex).data();
}

UTSetup::GateList UTSetup::GetGates()
{
    return GateList(gates);
}

/**
 * @brief UTSetup::GetMaxGateEnd
 * @return 闸门中后沿最大值
 */
double UTSetup::GetMaxGateEnd()
{
    double MaxGateEnd = 0;
//    foreach (GatePointer gate, gates)
//    {
    GatePointer gate = gates[0];
        if (!gate.isNull())
        {
            double start = gate->Get(QString("Start")).toDouble();  //
            double width = gate->Get(QString("Width")).toDouble();
            MaxGateEnd = qMax(start + width, MaxGateEnd);
        }
//    }
    return MaxGateEnd;
}

/**
 * @brief UTSetup::GetMinGateStart
 * @return
 * 获取闸门起点最小的值
 */
double UTSetup::GetMinGateStart()
{
    double minGateStart = GATE_START_LIMIT;
//    foreach (GatePointer gate, gates)
//    {
        GatePointer gate = gates[0];
        if (!gate.isNull())
        {
            double start = gate->Get(QString("Start")).toDouble();
            minGateStart = qMin(start, minGateStart);
        }
//    }
    return minGateStart;
}

void UTSetup::CheckGate(const QString &key, const QJsonValue &value)
{
    QRegExp RxGateAmp("Gate+(\\d)+Amp");            // 闸门幅值 0-100
    QRegExp RxGateStart("Gate+(\\d)+Start");        // 起点相对于Delay的设置，应该是不需要考虑偏左的情况
    QRegExp RxGateWidth("Gate+(\\d)+Width");        // 闸门宽度

    QJsonValue validValue = value;
    if (RxGateAmp.indexIn(key) != -1)    // 闸门幅值
    {
        validValue = QJsonValue(qMax(value.toDouble(), MIN_GATE_AMP));
        validValue = QJsonValue(qMin(validValue.toDouble(), MAX_GATE_AMP));
    }
    else if (RxGateStart.indexIn(key) != -1)         //闸门起点	   >3
    {
        QString GateNum = RxGateStart.cap(1);
        QString gateWidthJsonName = QString("Gate%1Width").arg(GateNum);
        if (GateNum.toInt() != 1)
        {
            double maxStart = Get(GetWaveType().append("Delay")).toDouble() + Get("Range").toDouble() - Get(gateWidthJsonName).toDouble() - Get(GetWaveType().append("Gate1Start")).toDouble();
            validValue = QJsonValue(qMin(maxStart, value.toDouble()));
        }
        else
        {
            double maxStart = Get(GetWaveType().append("Delay")).toDouble() + Get("Range").toDouble() - Get(gateWidthJsonName).toDouble();
            validValue = QJsonValue(qMin(maxStart, value.toDouble()));
        }
        validValue = QJsonValue(qMax(validValue.toDouble(), MIN_GATE_START));

    }
    else if (RxGateWidth.indexIn(key) != -1)    // 闸门宽度
    {
       QString GateNum = RxGateWidth.cap(1);
       QString gateStartJsonName = QString("Gate%1Start").arg(GateNum);
       if (GateNum.toInt() != 1)
       {
           double maxWidth = Get(GetWaveType().append("Delay")).toDouble() + Get("Range").toDouble() - Get(gateStartJsonName).toDouble() - Get(GetWaveType().append("Gate1Start")).toDouble();
           validValue = QJsonValue(qMin(maxWidth, value.toDouble()));
       }
       else
       {
           double maxWidth = Get(GetWaveType().append("Delay")).toDouble() + Get("Range").toDouble() - Get(gateStartJsonName).toDouble();
           validValue = QJsonValue(qMin(maxWidth, value.toDouble()));
       }
       validValue = QJsonValue(qMax(MIN_GATE_WIDTH, validValue.toDouble()));
    }

    if (validValue != value)
    {
        emit ParameterLimit(key, validValue);
    }

    return Config::Set(GetWaveType().append(key), validValue);
}

/**
 * @brief UTSetup::CheckDelay
 * @param key
 * @param OldDelayNum
 * @param NewDelayNum
 * 检查延时是否符合要求
 */
void UTSetup::CheckDelay(const QString &key, double delay)
{
    delay = qMax(delay, MIN_DELAY); /* 如果小于最小值，令其等于最小值 */

    double minGateStart = GetMinGateStart();
    delay = qMin(delay, minGateStart);      /* Delay必须早于任何闸门的起始 */

//    double minDelay = GetMaxGateEnd() - Get("Range").toDouble();  /* 要让所有闸门都容纳在声程中，由最后的闸门后沿反推Delay的最小值 */
//    delay = qMax(delay, minDelay);
    emit ParameterLimit(key, delay);
    return Config::Set(GetWaveType().append(key), QJsonValue(delay));
}

/**
 * @brief UTSetup::CheckNumGates
 * @param numGate
 * 检查闸门数，并进行赋值
 */
void UTSetup::CheckNumGates(int numGate)
{
    // 重新进行编排
    int numGates = numGate;
    if (numGates < 0)
    {
        qCritical() << "UTSetup::Set NumGates negative number of gates";
        return;
    }
    gates.resize(numGates);
    for (int index = 0; index < numGates; index++)
    {
        if (gates[index].isNull())
        {
            gates[index].reset(new UTGate(index, this));
            connect(this,  SIGNAL(ValueChanged(const QString&, const QJsonValue&)), gates[index].data(), SLOT(UTSetupParameterChanged(const QString&, const QJsonValue&)));
        }
    }
}
