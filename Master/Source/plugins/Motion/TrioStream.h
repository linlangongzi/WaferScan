#pragma once

#include <functional>
#include <QObject>
#include <QQueue>
#include <QVector>

namespace Motion {

class TrioDriver;

/*! 以太网通讯模式下允许的通道号, 这个枚举必须与channelNames一致 */
enum Channel {
    ChannelUnknown = -1,                                                        /*!< 未知通道, Trio不支持负数通道号, 仅在此组件内部使用 */
    Channel0,                                                                   /*!< 默认通道'0' */
    Channel5,                                                                   /*!< 用户通道'5' */
    Channel6,                                                                   /*!< 用户通道'6' */
    Channel7,                                                                   /*!< 用户通道'7' */
    ChannelProject,                                                             /*!< MotionPerfect控制通道'8', LOAD_PROJECT等指令必须通过此通道发送 */
    ChannelError,                                                               /*!< 错误通道'9', 控制器通过此通道发送错误信息 */
    ChannelText,                                                                /*!< 文字通道'/', 大块文本通过此通道发送和接收 */
    ChannelCount,
};

enum Mode {
    ModeUnknown = -1,
    Mode0 = 0,
    Mode1 = 1,
    Mode2 = 2,
    Mode3 = 3,
    Mode4 = 4,
};

enum State {
    StateNormal,    /*!< 正常 */
    StateChannel,   /*!< 已收到<ESC>(\033), 正在等待通道号 */
};

const char channelNames[] = "056789/";

/*! 待发送的指令 */
struct Command {
    QByteArray command;                                                         /*!< 待发送的指令 */
    QByteArray waitSequence;                                                    /*!< 要求等待的字符串 */
    QByteArray receivedSequence;                                                /*!< 已收到的字符串 */
    Channel channel = Channel0;                                                 /*!< 目标通道 */
    int commandId = 0;                                                          /*!< 指令号 */
    bool isRaw = false;                                                         /*!< raw命令不发送换行符 */
    bool hasEcho = false;                                                       /*!< MC 是否回传该指令 */
    std::function<void ()> callback;
};

/*! TrioStream是Trio运动控制驱动的通讯接口组件, 负责初始化, 追踪和切换输入输出通道.
 * Trio运动控制器的通讯协议类似于Telnet, 编码字符集为ASCII, 并使用部分控制字符进行通道切换
 * 和流控. 所有与Trio的通讯都必须经过TrioStream的处理, 其他组件不应当直接读取或写入socket
 */
class TrioStream : public QObject
{
    Q_OBJECT

public:
    class Crc16;

    explicit TrioStream(TrioDriver *parent);

    int tailCommandId() const;
    static QString encodeInt(uint value);

    static QByteArray escape(const QByteArray &data);
signals:
    void commandSent(int id);

public slots:
    int execute(const QString &command, Channel channel = Channel0, bool hasEcho = false, bool isRaw = false);
    int wait(const QString &sequence, Channel channel, std::function<void ()> callback = nullptr);
    void cancel(int fromCommandId, int toCommandId);
    void dumpDebug();

private:
    void flush();
    void timerEvent(QTimerEvent *event) override;
    static void writeMode(TrioDriver *p, int mode);
    static void writeChannel(TrioDriver *p, Channel channel);

private slots:
    void onSocketConnected();
    void onReadyRead();

private:
    TrioDriver *p = nullptr;
    int inputChannel = 0;                                                       /*!< 当前输入通道(接收) */
    int lastCommandId = 0xBAD;

    QQueue<Command> commands;                                                   /*!< 待发送的指令队列 */
    QQueue<Command> sent;                                                       /*!< 已发送, 等待echo的指令队列 */
    QByteArrayList channels;                                                    /*!< 各通道已收到的数据缓冲区 */
    struct InputContext {
        QByteArray *current = nullptr;                                          /*!< 指向channels[inputChannel] */
        State state = StateNormal;
    };
    InputContext context;
};

class TrioStream::Crc16
{
public:
    static const quint16 defaultPolynomial = 0x8005;

    explicit Crc16(quint16 polynomial = defaultPolynomial);

    quint16 hash(const QByteArray &buffer);

    QVector<quint16> crc16Table;
};

}   /* namespace Motion */
