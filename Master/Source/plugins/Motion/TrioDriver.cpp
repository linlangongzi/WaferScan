#include "TrioDriver.h"
#include "MotionManager.h"
#include "TrioAllocator.h"
#include "TrioQueue.h"
#include "TrioStream.h"
#include "TrioWarden.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QRegularExpression>
#include <QUrl>
#include <allrising/Util.h>
#include <utils/algorithm.h>
#include <functional>

/*!
 * \class TrioDriver
 * TrioDriver 是连接 TrioMotion 运动控制器(Motion Controller, MC)的驱动.
 * 技术文档参见官网 www.triomotion.uk
 *
 * \section1 通讯
 * TrioDriver 本身是一个 QTcpSocket, 但所有通讯都经过 TrioStream 进行处理.
 * MC 使用的是自定义的 telnet 协议, 端口号 23, 协议有"通道"的区分, 并且使用了特殊控制字符.
 * TrioStream 对其进行了封装, 并提供了指令队列的功能.
 * \sa TrioStream
 *
 * \section1 状态监视
 * TrioDriver 使用 MC 中的 PRMBLK (Parameter Block) 命令获取 MC 中的全局状态和轴状态.
 * PRMBLK 指令没有文档收录, 相关信息通过反向工程, 及反编译 MotionPerfect 获得.
 *
 * 目前已知的 PRMBLK 功能有:
 *
 * PRMBLK(0, block, function, parameter...)
 *  定义块block的功能, block编号从0开始.
 *  function = 0 为轴参数, function = 1 为全局参数;
 *  parameter列出需要上传的变量名. 变量名过多会超出行宽限制.
 *  指令不会返回任何值
 *
 * PRMBLK(1, block, function, parameter...)
 *  追加块block的参数, 类似于PRMBLK(0, ...)
 *
 * PRMBLK(3, block[, axis...])
 *  获取块block的当前数值, 数值以文本形式通过PRMBLK所在通道发送.
 *  如果该块已定义为全局参数, 无需指定axis;
 *  如果该块已定义为轴参数, 需要指定axis, 例如:
 *      PRMBLK(3, 1, 0, 1, 5)会返回0, 1, 5三个轴的数据:
 *          45.00000,16000.00000,1.00000,0.00000<CR><DC4>
 *          45.00000,16000.00000,1.00000,0.00000<CR><DC4>
 *          0.00000,888.88900,1.00000,0.00000<CR><DC4><LF><DC4>
 *      PRMBLK(3, 1, 0..12)会返回0~12共13个轴的数据, 共13行.
 *
 * PRMBLK(4, block[, axis...])
 *  类似PRMBLK(3, ...), 但只会返回自上一次PRMBLK请求之后改变的数据, 未改变的数据不发送.
 *  如果该块是轴参数, 其中未发生任何改变的轴将为空行.
 *  例如:
 *      PRMBLK(4,2,0..3) 请求block 1中0~3共4个轴改变的数据:
 *          1:0.00000,2:0.00000<CR><DC4>
 *          0:0.00000,<CR><DC4>
 *          <CR><DC4>
 *          0:0.00000,1:0.00000,2:0.00000,3:0.00000<CR><DC4><LF><DC4>
 *
 * PRMBLK(5)
 *  返回控制器中的最大 block 数量. 例如 PRINT PRMBLK(5)
 *
 * PRMBLK(6)
 *  返回控制器中每个 block 能够存储的最大参数数量. 例如 PRINT PRMBLK(6)
 *
 * PRMBLK(7, blockIndex)
 *  获取指定 block 的参数列表
 *
 * PRMBLK(8, blockIndex)
 *  删除指定 block 的配置
 *
 * PRMBLK(9)
 *  删除所有 block 的配置
 *
 * PRMBLK 的指令功能可以参考 Trio PCMotion 文档中, "PRMBLK_*" 部分的说明.
 *
 * PRMBLK 的配置, 请求, 处理均在 ChannelProject (8) 通道中进行, 上传的数据会被解析为
 * AxisStatus.
 */

using namespace Motion;

static Q_LOGGING_CATEGORY(debug, "trio.driver", QtWarningMsg);

using s = QString;

static const QRegularExpression tokenPattern("@(.+?)%");

static const QString statusServerName("STATUS_SERVER");
static const QString axisProgramName("INIT_AXIS");
static const QStringList defaultGlobalKeys = {"WDOG", "MOTION_ERROR", "INPUTS0", "INPUTS1", "SYSTEM_LOAD", "SYSTEM_ERROR"};
static const QStringList axisKeys          = {"DPOS", "AXISSTATUS", "MTYPE", "CORNER_MODE", "ATYPE"};

/*! 轴状态 (AXISSTATUS) 的掩模, 来自 TrioBASIC 官方文档 */
enum AxisStatusMask {
    SPEED_LIMIT_ACTIVE                      = 1 << 0,
    FOLLOWING_ERROR_WARNING_RANGE           = 1 << 1,
    COMMUNICATIONS_ERROR_TO_REMOTE_DRIVE    = 1 << 2,
    REMOTE_DRIVE_ERROR                      = 1 << 3,
    FORWARD_HARDWARE_LIMIT                  = 1 << 4,
    REVERSE_HARDWARE_LIMIT                  = 1 << 5,
    DATUMING_IN_PROGRESS                    = 1 << 6,
    FEEDHOLD_ACTIVE                         = 1 << 7,
    FOLLOWING_ERROR_EXCEEDS_LIMIT           = 1 << 8,
    FS_LIMIT_ACTIVE                         = 1 << 9,
    RS_LIMIT_ACTIVE                         = 1 << 10,
    CANCELING_MOVE                          = 1 << 11,
    PULSE_OUTPUT_AXIS_OVERSPEED             = 1 << 12,
    MOVETANG_DECELERATING                   = 1 << 13,
    VOLUME_LIMIT_ACTIVE                     = 1 << 14,
    AXIS_FS_LIMIT_ACTIVE                    = 1 << 15,
    AXIS_RS_LIMIT_ACTIVE                    = 1 << 16,
    ENCODER_POWER_SUPPLY_OVERLOAD           = 1 << 17,
    HW_PSWITCH_FIFO_NOT_EMPTY               = 1 << 18,
    HW_PSWITCH_FIFO_FULL                    = 1 << 19,
    FRAME_STOPPED                           = 1 << 20,
};

TrioDriver::TrioDriver(QObject *parent) :
    QTcpSocket(parent),
    m_globalKeys(defaultGlobalKeys)
{
    m_vrAlloc = new TrioAllocator(65536, this);
    m_tableAlloc = new TrioAllocator(512000, this);
    m_stream = new TrioStream(this);
    m_queue = new TrioQueue(this);
    m_warden = new TrioWarden(this);
    m_sineTable = m_tableAlloc->Alloc(m_sineTableSize);
    qCInfo(debug) << "Allocated circle table buffer at" << m_sineTable << "length" << m_sineTableSize;

    connect(this, &TrioDriver::stateChanged, this, &TrioDriver::onStateChanged);
    connect(m_stream, &TrioStream::commandSent, this, &TrioDriver::onCommandSent);
}

TrioDriver::~TrioDriver()
{
    disconnectFromController();
    if (state() != UnconnectedState) {
        waitForDisconnected();
    }
}

ControllerConfig TrioDriver::config() const
{
    return m_config;
}

ControllerStatus TrioDriver::status() const
{
    return m_status;
}

void TrioDriver::connectToController()
{
    QUrl url = QUrl::fromUserInput(m_config.host);
    quint16 port = url.port(23);
    connectToHost(url.host(), port);
    qCInfo(debug, "Connecting to host %s at port %d", qPrintable(m_config.host), port);
}

void TrioDriver::disconnectFromController()
{
    if (state() == ConnectedState) {
        QTcpSocket::disconnectFromHost();
        qCInfo(debug, "Disconnecting from host");
    } else {
        abort();
        qCInfo(debug, "Resetting socket");
    }
}

void TrioDriver::setConfig(const ControllerConfig &config)
{
    if (state() == ConnectedState && config.ioConfig != m_config.ioConfig) {
        const static QString pattern("INVERT_IN(%1, %2)");
        for (int index = 0; index < m_config.ioConfig.count(); ++index) {
            const auto &io = m_config.ioConfig[index];
            execute(pattern.arg(index).arg(io.inverted ? "ON" : "OFF"));
        }
    }

    m_config = config;
}

void TrioDriver::setParameter(const QString &key, double value)
{
    const static QString pattern("%1 = %2");
    execute(pattern.arg(key).arg(value));
}

void TrioDriver::setAxisParameter(int axis, const QString &key, double value)
{
    const static QString pattern("%2 Axis(%1) = %3");
    execute(pattern.arg(axis).arg(key).arg(value));
}

void TrioDriver::setAxisConfig(int axis, const AxisConfig &config)
{
    if (m_status.state != Normal) {
        return;
    }
    setAxisParameter(axis, "UNITS", config.units * 16);
    setAxisParameter(axis, "CREEP", config.datumCreep);
    setAxisParameter(axis, "DATUM_IN", config.datumInput);
    setAxisParameter(axis, "CREEP", config.datumCreep);
    setAxisParameter(axis, "DATUM_IN", config.datumInput);
    setAxisParameter(axis, "FWD_IN", config.limitForwardInput);
    setAxisParameter(axis, "REV_IN", config.limitReverseInput);
    setAxisParameter(axis, "FS_LIMIT", config.limitForward);
    setAxisParameter(axis, "RS_LIMIT", config.limitReverse);
    setAxisParameter(axis, "SRAMP", config.sRamp);
    if (config.repeatDistance > 0) {
        setAxisParameter(axis, "REP_DIST", config.repeatDistance);
        setAxisParameter(axis, "REP_OPTION", 0x1);
    } else {
        setAxisParameter(axis, "REP_DIST", 2e11);
        setAxisParameter(axis, "REP_OPTION", 0);
    }
}

void TrioDriver::setAxisSetup(int axisId, const AxisSetup &setup)
{
    setAxisParameter(axisId, "JOGSPEED", setup.speedJog);
    setAxisParameter(axisId, "SPEED", setup.speedJog);
    setAxisParameter(axisId, "ACCEL", setup.accel);
    setAxisParameter(axisId, "DECEL", setup.decel);
}

void TrioDriver::setMode(TrioDriver::MotionMode mode)
{
    if (mode == m_destMode) {
        return;
    }
    m_destMode = mode;
    m_currentMode = ModeUninitialized;
    emit modeChanged(m_currentMode);
    switchMode(mode);
}

void TrioDriver::forward(int axis)
{
    execute(QString("Forward Axis(%1)").arg(axis));
}

void TrioDriver::reverse(int axis)
{
    execute(QString("Reverse Axis(%1)").arg(axis));
}

void TrioDriver::cancel(int axisIndex)
{
    const static QString pattern1("Cancel(0) Axis(%1)");
    const static QString pattern2("Cancel(2) Axis(%1)");
    if (axisIndex >= 0) {
        execute(pattern1.arg(axisIndex));
        execute(pattern2.arg(axisIndex));
    } else {
        for (auto *axis: m_axis) {
            execute(pattern1.arg(axis->axisId()));
            execute(pattern2.arg(axis->axisId()));
        }
    }
}

void TrioDriver::halt()
{
    execute("Halt");
    cancel();
}

void TrioDriver::moveAbs(int axis, double position)
{
    execute(QString("MoveAbs(%1) Axis(%2)").arg(position).arg(axis));
}

void TrioDriver::moveRel(int axis, double position)
{
    execute(QString("Move(%1) Axis(%2)").arg(position).arg(axis));
}

static QMap<int, qreal> resolveMotion(const QHash<QUuid, qreal> &motion)
{
    QMap<int, qreal> m;
    for (auto it = motion.cbegin(); it != motion.cend(); ++it) {
        Axis *axis = MotionManager::axis(it.key());
        if (axis && axis->axisId() >= 0) {
            m[axis->axisId()] = it.value();
        }
    }
    return m;
}

void TrioDriver::moveAbs(const QHash<QUuid, qreal> &axisMotion)
{
    auto motion = resolveMotion(axisMotion);
    if (!motion.isEmpty()) {
        using namespace std;
        QStringList axisList = Utils::transform(motion.keys(), [] (int axis) {
            return QString::number(axis);
        });
        QStringList posList = Utils::transform(motion.values(), [] (qreal v) {
            return QString::number(v, 'f');
        });
        execute(QString("Base(%1)").arg(axisList.join(", ")));
        execute(QString("MoveAbs(%1)").arg(posList.join(", ")));
    }
}

void TrioDriver::dumpDebug()
{
    m_stream->dumpDebug();
}

QBitArray TrioDriver::ioStatus() const
{
    return m_ioStatus;
}

int TrioDriver::maxAxisId() const
{
    return m_axisIndex.isEmpty() ? 0 : m_axisIndex.lastKey();
}

int TrioDriver::virtualAxisId() const
{
    return maxAxisId() + 1;
}

void TrioDriver::addGlobalStatus(const QString &name)
{
    m_globalKeys << name;
}

void TrioDriver::execute(const QString &command, int channel)
{
    if (!isConnected()) {
        return;
    }
    if (channel < 0) {
        channel = ChannelProject;
    }
    if (command.contains('\n')) {
        foreach (QString cmd, command.split("\n")) {
            execute(cmd, channel);
        }
    } else if (!command.isEmpty()) {
        m_stream->execute(command, Channel(channel), true);
    }
}

void TrioDriver::sendRaw(const QString &command)
{
    m_stream->execute(command, ChannelUnknown, false, true);
}

void TrioDriver::initAxisProgram()
{
    QStringList lines;

    for (Axis *axis: m_axis) {
        const auto &config = axis->config();
        const auto &setup = axis->setup();
        lines << s("BASE(%1)").arg(config.axisId);
        lines << s("UNITS = %1").arg(config.units * 16);
        lines << s("CREEP = %1").arg(config.datumCreep);
        lines << s("DATUM_IN = %1").arg(config.datumInput);
        lines << s("FWD_IN = %1").arg(config.limitForwardInput);
        lines << s("REV_IN = %1").arg(config.limitReverseInput);
        lines << s("FS_LIMIT = %1").arg(config.limitForward);
        lines << s("RS_LIMIT = %1").arg(config.limitReverse);
        lines << s("SRAMP = %1").arg(config.sRamp);
        if (config.repeatDistance > 0) {
            lines << s("REP_DIST = %1").arg(config.repeatDistance);
            lines << s("REP_OPTION = %1").arg(0x1);
        } else {
            lines << s("REP_DIST = %1").arg(2e11);
            lines << s("REP_OPTION = %1").arg(0);
        }

        lines << s("JOGSPEED = %1").arg(setup.speedJog);
        lines << s("SPEED = %1").arg(setup.speedJog);
        lines << s("ACCEL = %1").arg(setup.accel);
        lines << s("DECEL = %1").arg(setup.decel);

        lines << QString();
    }

    for (int index = 0; index < qMax(32, m_config.ioConfig.size()); ++index) {
        const auto config = m_config.ioConfig.value(index);
        lines << s("INVERT_IN(%1, %2)").arg(index).arg(config.inverted ? "ON" : "OFF");
    }
    lines << s("FOR i = 0 TO %1 - 1").arg(m_sineTableSize);
    lines << s("    TABLE(%1 + i, sin(PI * i / %2 * 2))").arg(m_sineTable).arg(m_sineTableSize - 1);
    lines << s("NEXT i");

    setProgram(axisProgramName, lines);
}

void TrioDriver::initProgram(const QString &path, const QString &name)
{
    QStringList lines = ReadFileAsStringList(path);
    if (lines.isEmpty()) {
        qCCritical(debug) << "Failed initializing program from" << path;
    }
    setProgram(name, lines);
}

int TrioDriver::runProgram(const QString &name, int processNumber)
{
    static const QString pattern("Run %1");
    static const QString patternWithNumber("Run %1, %2");
    if (processNumber > 0) {
        return m_stream->execute(patternWithNumber.arg(name).arg(processNumber), ChannelProject, true);
    }
    return m_stream->execute(pattern.arg(name), ChannelProject, true);
}

void TrioDriver::stopProgram(const QString &name, int processNumber)
{
    static const QString pattern("Stop %1");
    static const QString patternWithNumber("Stop %1, %2");
    if (processNumber > 0) {
        execute(patternWithNumber.arg(name).arg(processNumber), ChannelProject);
    } else {
        execute(pattern.arg(name), ChannelProject);
    }
}

int TrioDriver::setProgram(const QString &name, const QStringList &lines)
{
    qCInfo(debug) << "SetProgram" << name << ":" << lines.count() << "lines";

#ifndef ARS_FINAL_RELEASE
    WriteFileAsString(name + ".BAS", lines.join("\n"));
#endif
    QStringList program;
    program << s("' %1").arg(qApp->property("Copyright").toString());
    program << s("' This program is dynamically configured by %1").arg(qApp->applicationName());
    program << s("' Do NOT edit this program, since it will be overriden soon.");
    program << s("");
    foreach (QString line, lines) {                                             /* 将多行的程序分行 */
        program << line.remove('\r').split("\n");                               /* 移除代码中可能的\r，因为\r会导致下位机执行 */
    }

    if (m_programCache.contains(name) && (m_programCache[name] == program)) {
        return m_stream->tailCommandId();
    }
    m_programCache[name] = program;

    int lineNumber = 0;
    if (m_config.fastProgram) {                                                 /* 快速模式 */
        QByteArray data = name.toLatin1() + '\x001c' + "0" + '\x001d' + "00";   /* 程序描述头: <程序名><类型><GS><下载><存储> */
        TrioStream::Crc16 crc;
        auto checksum = crc.hash(data);
        qCDebug(debug) << "Title" << TrioStream::escape(data) << "CRC" << checksum << TrioStream::encodeInt(checksum);
        m_stream->execute("LOAD_PROJECT(1)", ChannelProject);
        m_stream->wait("\x0005", ChannelText);
        m_stream->execute("\x0001" + data + "\x0003" + TrioStream::encodeInt(checksum) + "\x0002", ChannelText, false, true);
        m_stream->wait("\006", ChannelText);
        QByteArray programData = program.join('\r').toLatin1() + "\r\r";

        checksum = crc.hash(programData);
        qCDebug(debug) << "CRC of program" << checksum << TrioStream::encodeInt(checksum);
        m_stream->execute(programData, ChannelText, false, true);
        m_stream->execute("\x0003" + TrioStream::encodeInt(checksum), ChannelText, false, true);
        m_stream->wait("\006", ChannelText);
        m_stream->execute("COMPILE");
    } else {
        static const QString pattern("Edprog \"%2\", 1, %1");

        m_stream->execute(QString("New %1").arg(name), ChannelProject, true);     /* 删除 */
        m_stream->execute(QString("Select %1").arg(name), ChannelProject, true);  /* 新建 */
        m_stream->wait(QString("%1 selected").arg(name), ChannelProject);
        foreach (QString line, program) {
            m_stream->execute(pattern.arg(lineNumber).arg(line.replace('\"', "\"\"")), ChannelProject, true);   /* 将代码中的引号替换为BASIC的引号 */
            lineNumber++;
        }
        m_stream->execute("Compile", ChannelProject, true);                       /* 编译 */
    }
    return m_stream->wait("%[COMPILE", ChannelUnknown);                           /* %[COMPILE name SUCCESS number] %[COMPILE Line number] (number) - error text */
}

void TrioDriver::setVr(int index, double value)
{
    static const QString pattern("VR(%1) = %2");
    execute(pattern.arg(index).arg(value, 0, 'f'), ChannelProject);
}

void TrioDriver::setTable(int index, double value)
{
    static const QString pattern("Table(%1, %2)");
    execute(pattern.arg(index).arg(value, 0, 'f'));
}

void TrioDriver::setTable(int index, const QList<double> &values)
{
    if (values.isEmpty()) {
        return;
    }
    static const QString pattern("Table(%1, %2)");
    QStringList list;
    foreach (double value, values) {
        list << QString::number(value, 'f');
    }
    execute(pattern.arg(index).arg(list.join(", ")));
}

void TrioDriver::setTable(int index, const QJsonArray &values)
{
    if (values.isEmpty()) {
        return;
    }
    static const QString pattern("Table(%1, %2)");
    QStringList list;
    foreach (QJsonValue value, values) {
        list << QString::number(value.toDouble(), 'f');
    }
    execute(pattern.arg(index).arg(list.join(", ")));
}

void TrioDriver::onConnected()
{
    qCInfo(debug, "Trio connected");

    /* 停止所有程序 */
    m_status.baseCommandId = m_stream->execute("Halt", ChannelProject);
    m_stream->wait("Halt", ChannelProject);

    /* 获取控制器信息 */
    static const QString infoFormat(R"(?#6, "@CONTROLLER_INFO %1 "; %2; "%")");
    m_stream->execute(infoFormat.arg("model", "CONTROL"));
    m_stream->execute(infoFormat.arg("serial", "SERIAL_NUMBER"));
    m_stream->execute(infoFormat.arg("version", "VERSION"));
    m_stream->execute(infoFormat.arg("fpgaVersion", "FPGA_VERSION"));
    m_stream->execute(infoFormat.arg("servoPeriod", "SERVO_PERIOD"));
    m_stream->execute(infoFormat.arg("tableSize", "TSIZE"));
    m_stream->execute(infoFormat.arg("digitalInputCount", "NIN"));
    m_stream->execute(infoFormat.arg("digitalOutputCount", "NOP"));
    m_stream->execute(infoFormat.arg("digitalIoCount", "NIO"));
    m_stream->execute(infoFormat.arg("analogInputCount", "N_ANA_IN"));
    m_stream->execute(infoFormat.arg("analogOutputCount", "N_ANA_OUT"));
    m_stream->execute(infoFormat.arg("axisCount", "PRMBLK(5)[0]"));
    m_stream->execute(infoFormat.arg("memoryFree", "FREE"));
    m_stream->execute(infoFormat.arg("hasMemoryCard", "FILE \"DETECT\""));
    m_stream->execute(infoFormat.arg("featureEnabled", "FEATURE_ENABLE"));

    /* 等待所有控制器信息返回. */
    m_stream->execute("?#6, \"CONTROLLER_INFO_DONE\"");
    m_status.state = RequestInfo;
    m_status.progress = 0;
    m_status.total = m_stream->tailCommandId() - m_status.baseCommandId;
    emit statusChanged(m_status);

    m_stream->wait("CONTROLLER_INFO_DONE", Channel6, [=] () {
        auto version = QVersionNumber::fromString(m_info.version);
        if (version < CONTROLLER_VERSION_REQUIRED) {
            qCWarning(debug) << "Controller version too low" << version << ", expecting" << CONTROLLER_VERSION_REQUIRED;
            return disconnectFromHost();
        }
        m_status.baseCommandId = m_stream->tailCommandId();
        /* 下发各程序 */
        initAxisProgram();
        m_stream->execute("DEL " + statusServerName, ChannelProject);
        m_warden->start();

        runProgram(axisProgramName);
        m_stream->execute("?#6, \"AXIS_CONFIGURED\"", ChannelProject);

        m_status.state = ConfigureAxis;
        m_status.total = m_stream->tailCommandId() - m_status.baseCommandId;
        m_status.progress = 0;
        emit statusChanged(m_status);

        m_stream->wait("AXIS_CONFIGURED", Channel6, [=] () {
            m_status.state = Normal;
            switchMode(ModeManual);
            m_blockContext = -2;
            initParameterBlock();
            requestParameterBlock();
        });
    });
}

void TrioDriver::onStateChanged(QAbstractSocket::SocketState state)
{
    if (state == UnconnectedState || state == ClosingState) {
        if (m_status.state == RequestInfo) {
            qCCritical(debug) << "Early disconnect.";
        }
        m_programCache.clear();
        m_parameterRequestCount = 0;

        m_currentMode = ModeUninitialized;
        emit modeChanged(m_currentMode);

        m_status = ControllerStatus();
        emit statusChanged(m_status);

        m_info = ControllerInfo();
        emit controllerInfoChanged(m_info);
    } else if (state != ConnectedState) {
        m_status.state = Connecting;
        m_status.progress = 0;
        m_status.total = 0;
        emit statusChanged(m_status);
    }
}

void TrioDriver::onChannelDataArrived(int channel, const QString &line, char lineEnding)
{
    if (channel == ChannelProject && lineEnding == '\r' && m_blockContext == -1) {
        const auto items = line.splitRef(',', QString::SkipEmptyParts);
        for (const auto &item: items) {
            const auto fields = item.split(':');
            int index = fields.value(0).toInt();
            parseGlobalStatus(m_globalKeys.value(index), fields.value(1).toString());
        }
        m_blockContext = -2;
    } else if (channel == ChannelProject && lineEnding == '\r' && uint(m_blockContext) < uint(m_axis.count())) {
        const auto items = line.splitRef(',', QString::SkipEmptyParts);
        for (const auto &item: items) {
            const auto fields = item.split(':');
            int index = fields.value(0).toInt();
            parseAxisStatus(m_blockContext, axisKeys.value(index), fields.value(1).toDouble());
        }
        m_blockContext++;
        if (m_blockContext >= m_axis.count()) {
            requestParameterBlock();
        }
    } else if (channel == ChannelProject && line.startsWith("QUEUE_STATUS")) {
        m_queue->setProgramStatus(line.split(" ").value(1).toInt());
        m_blockContext = -1;
    } else if (channel == ChannelProject && line.startsWith("? \"QUEUE_STATUS \"")) {
    } else if (channel == ChannelProject && line.startsWith("PRMBLK(4,0")) {
        m_blockContext = -1;
    } else if (channel == ChannelProject && line.startsWith("PRMBLK(4,1")) {
        m_blockContext = 0;
        m_parameterRequestCount--;
    } else {
        if (channel == Channel6 && line.startsWith("CURRENT_MODE")) {
            m_currentMode = KeyToEnum(line.section(' ', -1, -1), m_currentMode);
            emit modeChanged(m_currentMode);
            m_status.progress = 0;
            m_status.total = 0;
            m_status.state = Normal;
            emit statusChanged(m_status);
            if (m_currentMode != m_destMode) {
                switchMode(m_destMode);
            }
        } else {
            QRegularExpressionMatch match = tokenPattern.match(line);
            if (match.hasMatch()) {
                qCDebug(debug) << "Got token" << match.captured(1);
                QString token = match.captured(1);
                if (channel == Channel6 && token.startsWith("CONTROLLER_INFO")) {
                    const QString name = token.section(' ', 1, 1);
                    const QString value = token.section(' ', 2, -1);
                    MetaType::writeGadgetProperty(m_info, name, value);
                    emit controllerInfoChanged(m_info);
                    qCInfo(debug) << "Controller info:" << name << value;
                }
                emit tokenReceived(token);
            }
        }
        if (!line.isEmpty()) {
            emit channelDataArrived(channel, line);
        }
    }
}

void TrioDriver::onCommandSent(int id)
{
    if (id > m_status.baseCommandId && (id - m_status.baseCommandId) <= m_status.total) {
        m_status.progress = id - m_status.baseCommandId;
        emit statusChanged(m_status);
    }
}

void TrioDriver::parseGlobalStatus(const QString &key, const QString &value)
{
    double v = value.toDouble();
    if (key.startsWith("INPUTS")) {     // INPUTS0 / INPUTS1
        int offset = key.rightRef(1).toInt();
        int bits = qRound(v);
        if (m_ioStatus.size() < 32) {
            m_ioStatus.resize(32);
        }
        for (int index = 0; index < 16; ++index) {
            m_ioStatus.setBit(index + (offset * 16), bits & (1 << index));
        }
        emit ioStatusChanged(m_ioStatus);
//    } else if (key == "MOTION_ERROR") {
//        m_info.motionError = v;
//        emit controllerInfoChanged(m_info);
    } else if (key == "SYSTEM_LOAD") {
        m_info.cpuLoad = v;
        emit controllerInfoChanged(m_info);
    } else if (key == "SYSTEM_ERROR") {
        m_info.systemError = v;
        emit controllerInfoChanged(m_info);
    }
    emit globalStatusChanged(key, v);
}

void TrioDriver::parseAxisStatus(int axisIndex, const QString &key, qreal value)
{
    Axis *axis = m_axis.value(axisIndex);
    if (!axis) {
        return;
    }
    AxisStatus status = axis->status();
    if (key == "DPOS") {
        status.position = value;
    } else if (key == "AXISSTATUS") {
        int v = qRound(value);

        status.limitedForwardInput = v & FORWARD_HARDWARE_LIMIT;
        status.limitedReverseInput = v & REVERSE_HARDWARE_LIMIT;
        status.limitedForward = v & FS_LIMIT_ACTIVE;
        status.limitedReverse = v & RS_LIMIT_ACTIVE;
    } else if (key == "MTYPE") {
        status.moveType = AxisStatus::MoveType(qRound(value));
    } else if (key == "CORNER_MODE") {
        status.datumDone = qRound(value);
    } else if (key == "ATYPE") {
        status.axisType = qRound(value);
    } else {
        qCWarning(debug, "Axis %d: Invalid axis report %s: %f", axisIndex, qPrintable(key), value);
        return;
    }
    axis->setStatus(status);

    qCDebug(debug, "Axis %d: %s - %f", axisIndex, qPrintable(key), value);
}

void TrioDriver::refreshAxisList(const QList<Axis *> &axisList)
{
    m_axis = axisList;
    m_axisIndex.clear();
    QStringList axisId;
    for (auto *axis: axisList) {
        m_axisIndex[axis->axisId()] = axis;
        axisId << QString::number(axis->axisId());
    }
    m_blockAxisList = axisId.join(',');
    if (isConnected()) {
        initAxisProgram();
        initParameterBlock();
    }
}

void TrioDriver::initParameterBlock()
{
    /* 复位(9) 所有 block */
    execute("PRMBLK(9)", ChannelProject);
    /* 设置(0), 块0, 全局参数(1) */
    execute(QString("PRMBLK(0, 0, 1, %1)").arg(m_globalKeys.join(", ")), ChannelProject);
    /* 设置(0), 块1, 轴参数(0) */
    execute(QString("PRMBLK(0, 1, 0, %1)").arg(axisKeys.join(", ")), ChannelProject);
}

void TrioDriver::requestParameterBlock()
{
    while (m_parameterRequestCount < 1) {
        /* 请求QUEUE的状态 */
        m_stream->execute(QString("? \"QUEUE_STATUS \";PROC_STATUS PROC(%1)").arg(QUEUE_PROCESS_NUMBER), ChannelProject, false);
        /* 请求(4), 块0 */
        m_stream->execute(QString("PRMBLK(4,0)"), ChannelProject, false);
        /* 请求(4), 块1 */
        m_stream->execute(QString("PRMBLK(4,1,%1)").arg(m_blockAxisList), ChannelProject, false);
        m_parameterRequestCount++;
    }
}

void TrioDriver::switchMode(TrioDriver::MotionMode mode)
{
    if (m_status.state != Normal) {
        return;
    }
    m_status.baseCommandId = m_stream->tailCommandId();
    if (mode == ModeManual) {
        execute("RapidStop");
        setParameter("WDOG", 1);
        for (Axis *axis: m_axis) {
            setAxisParameter(axis->axisId(), "FWD_JOG", -1);
            setAxisParameter(axis->axisId(), "REV_JOG", -1);
        }
    } else if (mode == ModeRemote) {
        for (Axis *axis: m_axis) {
            setAxisParameter(axis->axisId(), "FWD_JOG", axis->config().jogForwardInput);
            setAxisParameter(axis->axisId(), "REV_JOG", axis->config().jogReverseInput);
        }
    } else if (mode == ModeCircular) {
        execute("RapidStop");
    } else if (mode == ModeProgram) {
        stopProgram("QUEUE");
        execute("RapidStop");
    } else if (mode == ModeTcp) {
        execute("RapidStop");
    }
    QString modeName = EnumToKey(mode);
    execute(QString("?#6, \"CURRENT_MODE %1\"").arg(modeName));
    m_stream->wait(QString("CURRENT_MODE %1").arg(modeName), Channel6);
    m_status.state = SwitchingMode;
    m_status.total = m_stream->tailCommandId() - m_status.baseCommandId;
    m_status.progress = 0;
    emit statusChanged(m_status);
}
