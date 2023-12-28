#ifndef UTSETUP_H
#define UTSETUP_H

#include <QObject>
#include <QVector>
#include "ConfigObject/Config.h"
#include "UTGate.h"
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

class UTGate;
class UTChannel;
class Material;

class UTSetup : public Config
{
    Q_OBJECT
    CONFIG_OBJECT

    typedef QPointer<UTChannel> ChannelPointer;
    typedef QVector<ChannelPointer> BindChannelList;

public:
    typedef QSharedPointer<UTGate> GatePointer;
    typedef QVector<GatePointer> GateList;

public:
    bool BindChannel(UTChannel *utchannel);
    bool UnbindChannel(UTChannel *utchannel);
    QString GetWaveType() const;

signals:
    void ParameterLimit(const QString &key, const QJsonValue &value);
    void RequestUpdateAnswer(const QString &key, const QJsonValue &value);
    void ValueChangedSetup(const QString &key, const QJsonValue &value);

public slots:
    QJsonObject RequestRefresh();
    void RequestUpdate(const QString &key);
    void SetParameter(const QString &key, const QJsonValue &value);
    void ActionValueChanged(const QString &key, const QJsonValue &value);

public:
    virtual void Set(const QString &key, const QJsonValue &value);  // 重载父类的函数

public:
    int GetNumGates();  // 获取闸门总数
    Material *GetMaterial();
    double GetMaterialSpeed();
    QString GetMaterialName();

    double GetDelay();
    double GetRange();
    double GetParameter(const QString &key);
    void Init();   // 初始化各默认参数
    QJsonArray GetTCGJsonArray();
    void  AddTCGItem(const QJsonValue &time, const QJsonValue &gain, QJsonValue array = 0); // 增加的一项TCG的点
    void  ModifyTCGJsonArrayItem(const QJsonValue &time, const QJsonValue &gain, int index);  // 修改的值和索引号
    void  RemoveTCGItemAt(int index);     // 删除掉的数组的序号
    UTGate *GetGate(int gateIndex);  // 获取子类的指针
    GateList GetGates();

public:

protected:
    explicit UTSetup(QObject *parent = 0);
    ~UTSetup();
    virtual ConfigObject *New(QObject *parent) const;

private:
    double GetMaxGateEnd();

    double GetMinGateStart();

    void CheckGate(const QString &key, const QJsonValue &value);

    void CheckDelay(const QString &key, double delay);

    void CheckNumGates(int numGate);

    GateList gates;

    BindChannelList channels;  // 与此工艺绑定的chennel 列表
    qint32 m_waveType;
    QJsonArray m_TCGJsonArray;
    const double MAX_HEIGHT;
    const int MIN_PRF;                                                          // 重复频率最小值
    const int MAX_PRF;                                                          // 重复频率最大值 ADFREQUENCY成400，最大值为16000 ；ADFREQUENCY成200，最大值为8000 ； ADFREQUENCY成100，最大值为4000
    const double MIN_GATE_SYNC_THRESHOLD;
    const double MAX_GATE_SYNC_THRESHOLD;
    const double MIN_GATE_AMP;
    const double MAX_GATE_AMP;
    const double MIN_GATE_WIDTH;
    const double MIN_GATE_START;
    const int MIN_SAMPLE_AVERAGE;
    const int MAX_SAMPLE_AVERAGE;
    const double MIN_DELAY;
    const double MIN_RANGE;
    const double MAX_RANGE;
    const int GATE_START_LIMIT;
    const double MIN_TCG_GAIN;
    const double MAX_TCG_GAIN;
};

#endif // UTSETUP_H
