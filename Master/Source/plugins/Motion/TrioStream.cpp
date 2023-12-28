#include "TrioDriver.h"
#include "TrioStream.h"
#include <QByteArrayMatcher>
#include <QLoggingCategory>
#include <QTimer>
#include <allrising/Util.h>

/*!
 * \class TrioStream
 *
 * \brief TrioStream 负责管理 MC 的通讯, 处理特殊控制字符, 管理各通道的接收缓存, 以及指令发
 * 送队列.
 *
 * \section 通讯模式
 * MC 的通讯协议中, 有0~4共5种模式, 使用指令 MPE(mode) 进行切换:
 * * 模式0没有通道区分, 没有流量控制, 是 MC 上电后的默认状态.
 * * 模式4分通道, 并且有错误报告, 是 TrioStream 正常通讯选用的状态.
 *
 * 在模式4下, 发送端和接收端均可独立切换通道, 使用"<ESC>通道名"指示其后的所有内容为新的通道号.
 * 由于所有通讯均为异步, 发送端和接收端的通道可以不同.
 * 通道名为ASCII, 例如"<ESC>0"将切换到通道0, "<ESC>7"将切换到通道7.
 * TrioStream 中只允许发送和接收 ASCII 指令和数据.
 * \sa Channel
 * 参见 TrioBASIC 文档, MPE
 *
 * \section 接收
 * 为了正确处理"<ESC>通道号"的控制序列, 接收部分使用状态机记录是否处于"<ESC>"序列中, 并记录各
 * 通道未接收完的数据.
 *
 * \section 发送
 * 数据的发送经过一个指令队列缓冲, 未发送的数据可以从指令缓冲区中删除.
 * 除数据外, 指令队列中可以加入特殊的"wait"命令, 令 TrioStream 在此处暂停发送指令, 等待接收到
 * MC 的指定字符串后, 方可继续发送.
 *
 * \section 回传 (Echo)
 * 除了部分特殊指令外, 向 MC 发送的大部分指令, 都会被 MC 在同一个通道 echo 回来.
 * \code
 * Tx: Move(10)<CR>
 *     Move(-10)<CR>
 * Rx: Move(10)<CR>
 *     Move(-10)<CR>
 * \endcode
 * 由于指令的发送是异步过程, TrioStream 常常一次发送多条指令给 MC, 远超 MC 的执行速度.
 * echo 可以用来判断 MC 接受指令的进度, 在 TrioStream 中, 有 echo 的命令会被移到"sent"队
 * 列中等待 echo, "sent"数量过多时会暂停发送.
 */

enum ControlChar {
    CharEsc = '\033',
    CharNewLine = '\n',
    CharCarrigeReturn = '\r',
    CharXon = '\021',
    CharXoff = '\023',
    CharDC4 = '\024',
    CharNak = '\025',
    CharEnquiry = '\005',
    CharAck = '\006',
    CharEndOfText = '\003',
};

using namespace Motion;

static Q_LOGGING_CATEGORY(debug, "trio.stream", QtWarningMsg);

static const char lineDelimeters[] = {CharCarrigeReturn, CharNewLine, CharDC4, CharNak, CharEnquiry, CharEndOfText};
static const QByteArray channelName(channelNames);
static const char *specialChar[] = {                                            /*!< 特殊字符的显示名, 供调试信息使用 */
    "<NUL>", "<SOH>", "<STX>", "<ETX>", // 00-03
    "<EOT>", "<ENQ>", "<ACK>", "<BEL>", // 04-07
    "<BS>",  "<HT>",  "<LF>",  "<VT>",  // 08-0b
    "<FF>",  "<CR>",  "<SO>",  "<SI>",  // 0c-0f
    "<DLE>", "<DC1>", "<DC2>", "<DC3>", // 10-13
    "<DC4>", "<NAK>", "<SYN>", "<ETB>", // 14-17
    "<CAN>", "<EM>",  "<SUB>", "<ESC>", // 18-1b
    "<FS>",  "<GS>",  "<RS>",  "<US>"   // 1c-1f
};

QByteArray TrioStream::escape(const QByteArray &data)
{
    QByteArray ret;
    for (uchar c: data) {
        if (uchar(c) < 0x20) {
            ret.append(specialChar[c]);
        } else {
            ret.append(c);
        }
    }
    return ret;
}

TrioStream::TrioStream(TrioDriver *parent) :
    QObject(parent)
{
    Q_ASSERT_X(channelName.count() == ChannelCount, "Trio:Stream", "channelName not matching enum Channel");
    p = parent;
    connect(parent, &QAbstractSocket::connected, this, &TrioStream::onSocketConnected);
    connect(parent, &QIODevice::readyRead, this, &TrioStream::onReadyRead);

    while (channels.count() < ChannelCount) {
        channels << QByteArray();
    }

    startTimer(100);
}

int TrioStream::tailCommandId() const
{
    return lastCommandId;
}

/*!
 * \brief 在指定通道向 MC 发送指令
 * \param command 指令
 * \param channel 发送指令的通道. MC 只执行 Channel0 和 ChannelProject 上接收到的指令,
 *          其它通道的指令只能被用户程序接收.
 * \param hasEcho 该指令是否有回传(echo). 没有 echo 的指令无法检测是否被MC收到,
 *          所以 echo = false 的指令没有对应的 commandSent 信号.
 * \param isRaw 是否使用换行符. raw模式不添加任何换行符. 注意, MC没有接收到换行符时, 不会执行
 *          收到的指令.
 * \return 此命令的id
 */
int TrioStream::execute(const QString &command, Channel channel, bool hasEcho, bool isRaw)
{
    lastCommandId++;

    Command c;
    c.command = command.toLatin1();
    c.channel = channel;
    c.isRaw = isRaw;
    c.hasEcho = hasEcho;
    c.commandId = lastCommandId;
    commands << c;
    qCDebug(debug) << "Tx: Adding to q#" << channel << ":" << escape(command.toLatin1()) << "echo" << hasEcho;
    flush();
    return c.commandId;
}

/*!
 * \brief 在通道 channel 等待 MC 发送一个特定的字符串 sequence, 收到此字符串后方可发送指令
 *          队列后面的指令. 当 sequence 全部收到后, 会抛出 commandSent 信号, 并调用
 *          callback, 之后继续发送指令队列中的下一条指令.
 * \param sequence 等待的字符串序列, 不支持正则表达式.
 * \param channel MC 须在此通道上发送数据. ChannelUnknown 为任意通道.
 * \param callback 收到字符串序列后的回调函数. 可选.
 * \return 此命令的 id, 与收到 sequence 后 commandSent(id) 的参数相同.
 */
int TrioStream::wait(const QString &sequence, Channel channel, std::function<void ()> callback)
{
    lastCommandId++;
    if (!sequence.isEmpty()) {
        qCDebug(debug, "Adding wait sequence %s", qPrintable(escape(sequence.toLatin1())));
        Command c;
        c.commandId = lastCommandId;
        c.waitSequence = sequence.toLatin1();
        c.channel = channel;
        c.callback = callback;
        commands << c;
    }
    return lastCommandId;
}

void TrioStream::cancel(int fromCommandId, int toCommandId)
{
    std::remove_if(commands.begin(), commands.end(), [=](const Command &c) {
        return c.commandId >= fromCommandId && c.commandId <= toCommandId;
    });
    flush();
}

void TrioStream::dumpDebug()
{
    qCDebug(debug) << "Dumping" << commands.count() << "commands";
    for (const auto &c: commands) {
        qCDebug(debug) << c.commandId << "@" << c.channel << ":" << c.command << c.hasEcho << c.isRaw;
        if (!c.waitSequence.isEmpty()) {
            qCDebug(debug) << "  Wait sequence" << escape(c.waitSequence) << "got" << escape(c.receivedSequence);
        }
    }
    qCDebug(debug) << "Dumping" << sent.count() << "sent commands";
    for (const auto &c: sent) {
        qCDebug(debug) << c.commandId << "@" << c.channel << ":" << c.command;
    }
}

void TrioStream::flush()
{
//    qCDebug(debug, "Flusing queue with %d commands", commands.count());
    while (!commands.isEmpty() && sent.count() < 16) {
        Command &command = commands.first();
        if (!command.waitSequence.isEmpty()) {
            return;
        }
        if (!command.command.isEmpty()) {
            writeChannel(p, command.channel);
            qCDebug(debug, "Writing #%d: %s", command.channel, qPrintable(command.command));
            p->write(command.command);
            if (!command.isRaw) {
                if (command.channel == ChannelProject) {
                    p->write("\n\024");
                } else {
                    p->write("\r");
                }
            }
            if (command.hasEcho) {
                sent << command;
            }
        }
        commands.removeFirst();
    }
}

void TrioStream::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    if (p->state() == TrioDriver::ConnectedState) {
        p->write("\024");
    }
}

void TrioStream::writeMode(TrioDriver *p, int mode)
{
    if (mode >= 0) {
        qCDebug(debug, "Writing mode %d", mode);
        p->write(QString("\0330\003MPE(%1)\r\024").arg(mode).toLatin1());
    }
}

void TrioStream::writeChannel(TrioDriver *p, Channel channel)
{
    if (uint(channel) < uint(channelName.count())) {
        const char data[] = {CharEsc, channelName[channel]};
        p->write(data, 2);
    }
}

void TrioStream::onSocketConnected()
{
    commands.clear();
    sent.clear();

    inputChannel = 0;
    p->write("\0330\003\r\r");                                                  /* 输出通道切换到0, 发送一个空指令 */
    wait(">>", ChannelUnknown);                                                 /* 等待空指令响应 */
    execute("MPE(0)\r", ChannelUnknown);                                        /* 切换到MPE(0) */
    wait("MPE(0)", Channel0);                                                   /* 等待MPE切换成功 */
    execute("MPE(4)\r", ChannelUnknown);                                        /* 切换到MPE(4) */
    wait("%[MPE] On(4)", ChannelUnknown);                                       /* 等待MPE切换成功 */
    context = InputContext();
    context.current = &channels.first();

    p->onConnected();
}

void TrioStream::onReadyRead()
{
    qCDebug(debug) << "Rx: Reading" << p->bytesAvailable() << "bytes:" << escape(p->peek(p->bytesAvailable()));
    static const QByteArray lineDelimeter(lineDelimeters, sizeof(lineDelimeters));
    char c;

    while (!p->atEnd() && p->getChar(&c)) {
        if (c == CharEsc) {                                                     /* 通道切换指令<ESC>1 */
//            qCDebug(debug, "Got esc");
            context.state = StateChannel;
            continue;
        }
        if (c == CharXoff) {                                                    /* 流控暂停指令 */
            qCDebug(debug, "Rx: Paused by Xoff");
//            context.paused = true;
            continue;
        }
        if (c == CharXon) {                                              /* 流控暂停恢复指令 */
//            context.paused = false;
//            FlushQueue();
            qCDebug(debug, "Rx: Continue by Xon");
            continue;
        }
        if (context.state == StateChannel) {                                    /* 已接收到<ESC>, 处于通道切换状态, 当前c为通道号 */
            context.state = StateNormal;
            const int channel = channelName.indexOf(c);                         /* 查询通道名表, 获取通道号 */

            if (uint(channel) < uint(channels.count())) {
                qCDebug(debug, "Rx: Switching to channel %c (index %d)", channelName[channel], channel);
                context.current = &channels[channel];
                inputChannel = channel;
            } else {
                qCWarning(debug, "Rx: Invalid channel name %c", c);
            }
            continue;
        }
        if (commands.count() && commands.first().waitSequence.count()) {        /* 当前有指令正在等待特定序列 */
//            qCDebug(debug, "Rx: Wait sequence got char %d", c);
            Command &command = commands.first();
            if (command.channel == ChannelUnknown || command.channel == inputChannel) {
                command.receivedSequence.append(c);
//                qCDebug(debug) << "Rx: Expecting" << Escape(command.waitSequence) << ", got" << Escape(command.receivedSequence);
                command.receivedSequence = command.receivedSequence.right(command.waitSequence.size());/* 为避免receivedSequence溢出, 移除已收到的过多部分 */

                if (command.receivedSequence == command.waitSequence) {
                    qCDebug(debug) << "Rx: Received expected token" << escape(command.receivedSequence);
                    emit commandSent(command.commandId);
                    if (command.callback) {
                        command.callback();
                    }
                    commands.removeFirst();                                     /* remove之后, Command &变为无效的引用 */
                    flush();
                }
            } else {
                qCDebug(debug, "Rx: Waiting at wrong channel: %d expected, got %d", command.channel, inputChannel);
            }
        }
        if (lineDelimeter.contains(c)) {                                        /* 换行 */
            if (!context.current->isEmpty() || inputChannel == ChannelProject) {/* ChannelProject 允许空行 */
                qCDebug(debug) << "Rx: Message #" << inputChannel << escape(*context.current) << "line-ending" << specialChar[c];
                p->onChannelDataArrived(inputChannel, *context.current, c);        /* 抛出通知 */
//                if (!context.current->isEmpty()) {
//                    p->channelDataArrived(inputChannel, *context.current);
//                }

                if (!sent.isEmpty()) {                                          /* 若当前正在等待echo, 对其进行处理 */
                    const Command &command = sent.first();
                    if (command.channel == inputChannel && context.current->endsWith(command.command)) {
                        qCDebug(debug) << "Rx: Got echo #" << command.channel << command.command;
                        emit commandSent(command.commandId);
                        sent.removeFirst();
                        flush();
                    }
                }

                context.current->clear();                                       /* 清空当前通道缓冲区 */
            }
        } else {                                                                /* 正常文本 */
            if (c < 0x20) {
                qCDebug(debug, "Rx: Got unknown char %02x (%s)", c, specialChar[uchar(c)]);
            }
            context.current->append(c);
        }
    }
}

QString TrioStream::encodeInt(uint value)
{
    QByteArray array(3, 0);
    array[2] = (value & 0x3f) + 0x20;
    value = value >> 6;
    array[1] = (value & 0x3f) + 0x20;
    value = value >> 6;
    array[0] = (value & 15) + 0x20;
    return array;
}

static QVector<quint16> buildCrc16Table(quint16 polynomial)
{
    QVector<quint16> array(0x100);
    for (quint16 index = 0; index < array.size(); ++index) {
        quint16 num2 = index << 8;
        for (uchar j = 0; j < 8; ++j) {
            num2 = (num2 << 1) ^ (((num2 & 0x8000) != 0) ? polynomial : 0);
        }
        array[index] = num2;
    }
    return array;
}

TrioStream::Crc16::Crc16(quint16 polynomial)
{
    crc16Table = buildCrc16Table(polynomial);
}

quint16 TrioStream::Crc16::hash(const QByteArray &buffer)
{
    quint16 crc = 0;
    for (uchar i: buffer) {
        quint16 num2 = crc16Table[(crc >> 8) & 0xff];
        crc = ((crc << 8) | i) ^ num2;
    }
    crc = (crc << 8) ^ crc16Table[(crc >> 8) & 0xff];
    crc = (crc << 8) ^ crc16Table[(crc >> 8) & 0xff];
    return crc;
}
