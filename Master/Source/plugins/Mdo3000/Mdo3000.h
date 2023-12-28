#pragma once

#include <Ultrasonic/Benchmark.h>
#include <Ultrasonic/IInstrumentDriver.h>
#include <QTcpSocket>
#include <QTime>

class Mdo3000 : public IInstrumentDriver
{
    Q_OBJECT

public:
    enum State {
        WaitingLineStart,                                                       /*!< 等待行开头, 首字符决定行内容的性质 */
        WaitingSpace,                                                           /*!< 等待">"之后的空格 */
        WaitingText,                                                            /*!< 等待纯文本行 */
        WaitingCommand,                                                         /*!< 等待命令内容 */
        WaitingValue,                                                           /*!< 等待命令的值 */
        WaitingQuote,                                                           /*!< 等待值中的引号结束 */
        WaitingDigitLength,                                                     /*!< 等待二进制数据长度的位数 */
        WaitingLength,                                                          /*!< 等待二进制数据长度 */
        WaitingData,                                                            /*!< 等待二进制数据 */
    };
    Q_ENUM(State)

public:
    Mdo3000(QObject *parent = nullptr);
    ~Mdo3000() override;
    static UTDataHeader InitDataHeader();

public slots:

    virtual Benchmark *GetBenchmark();
    void setHostAddr(const QString &ip) override;
    void sendCommand(const QString &command) override;
    void setDataLevel(int level, int channel) override;
    void setActiveChannel(quint32 channelIndex) override;
    void Connect() override;
    void Disconnect() override;
    void setChannelSetup(int channel, const UTSetup &setup) override;
    InstrumentStatus instrumentStatus() const;

signals:
    void instrumentStatusChanged(InstrumentStatus &status);

private slots:
    void OnConnected();
    void OnDataArrived();
    void updateInstrumentStatus();
    void setInstrumentStatus(const InstrumentStatus &status);


private:
    void writeCommand(const QString &command);
    bool eatData();
    void setState(State state);
    void setIdle(bool idle);
    void eatCommand();
    void eatText();
    void parseCurve(const QByteArray &data);

private:
    /*! socket通讯上下文 */
    struct Context
    {
        QByteArray command;                                                     /*!< 已接收到的命令 */
        QByteArray value;                                                       /*!< 已接收到的值, 文本或二进制 */
        QByteArray commandRoot;                                                 /*!< 当前接受命令的路径 */
        State state = WaitingText;                                              /*!< 接收状态机的状态 */
        qint64 expectLength = 0;                                                /*!< 期望的数据长度, 含义与state有关 */
        qreal xzero = 0;                                                        /*!< 波形的触发原点 */
        uint acquireCount = -1;                                                 /*!< 采集波形的序号 */
        bool acquireNew = true;                                                 /*!< 采集波形 */

        bool idle = false;                                                      /*!< 示波器空闲, 可发送指令 */
        QStringList commandQueue;                                               /*!< 未发送的命令队列 */
    };

    QString hostIp;
    QList<UTSetup> localParameter;
    QTcpSocket socket;
    Benchmark benchmark;
    InstrumentInfo m_info;
    InstrumentStatus m_status;
    Context context;

};

class Mdo3000Factory : public IInstrumentDriverFactory
{
    Q_OBJECT

public:
    Mdo3000Factory(QObject *parent) : IInstrumentDriverFactory(parent) {}

    QString name() const override {return "Mdo3000";}
    IInstrumentDriver *create() const override {return new Mdo3000;}
};
