#pragma once

#include "Axis.h"
#include "ControllerInfo.h"
#include <QBitArray>
#include <QtNetwork/QTcpSocket>
#include <allrising/MetaType/MetaType.h>

const qreal driverResolution = 0.001;

class MotionManager;

namespace Motion {

class TrioAllocator;
class TrioQueue;
class TrioStream;
class TrioWarden;

class MOTION_EXPORT TrioDriver : public QTcpSocket
{
    Q_OBJECT

    Q_PROPERTY(ControllerConfig config READ config WRITE setConfig)
    Q_PROPERTY(ControllerStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(ControllerInfo controllerInfo READ controllerInfo NOTIFY controllerInfoChanged)
    Q_PROPERTY(QBitArray ioStatus READ ioStatus NOTIFY ioStatusChanged)

public:
    using AxesMotion = QMap<int, double>;

    enum MotionMode {
        ModeUninitialized,
        ModeManual,
        ModeRemote,
        ModeCircular,
        ModeProgram,
        ModeTcp
    };
    Q_ENUM(MotionMode)

    explicit TrioDriver(QObject *parent = nullptr);
    ~TrioDriver() override;
    ControllerConfig config() const;
    ControllerStatus status() const;
    ControllerInfo controllerInfo() const {return m_info;}
    MotionMode mode() const {return m_currentMode;}

public slots:
    void execute(const QString &command, int channel = -1);
    void sendRaw(const QString &command);

    void connectToController();
    void disconnectFromController();
    void setConfig(const ControllerConfig &config);
    void setMode(MotionMode mode);
    void setParameter(const QString &key, double value);
    void setAxisParameter(int axis, const QString &key, double value);
    void setAxisConfig(int axis, const AxisConfig &config);
    void setAxisSetup(int axisId, const AxisSetup &setup);

    void forward(int axis);
    void reverse(int axis);
    void cancel(int axisIndex = -1);
    void halt();
    void moveAbs(int axis, double position);
    void moveRel(int axis, double position);
    void moveAbs(const QHash<QUuid, qreal> &axisMotion);
    void dumpDebug();

signals:
    void globalStatusChanged(QString key, double value);
    void ioStatusChanged(QBitArray status);
    void tokenReceived(QString token);
    void channelDataArrived(int channel, QString line);
    void controllerInfoChanged(ControllerInfo info);
    void modeChanged(MotionMode mode);
    void statusChanged(ControllerStatus status);

public:
    TrioQueue *queue() {return m_queue;}
    QBitArray ioStatus() const;

protected:
    bool isConnected() const {return state() == ConnectedState;}
    int maxAxisId() const;
    int virtualAxisId() const;

protected slots:
    void addGlobalStatus(const QString &name);
    void initAxisProgram();
    void initProgram(const QString &path, const QString &name);
    int runProgram(const QString &name, int processNumber = -1);
    void stopProgram(const QString &name, int processNumber = -1);
    int setProgram(const QString &name, const QStringList &lines);
    void setVr(int index, double value);
    void setTable(int index, double value);
    void setTable(int index, const QList<double> &values);
    void setTable(int index, const QJsonArray &values);
    void onConnected();
    void onStateChanged(SocketState state);
    void onChannelDataArrived(int channel, const QString &line, char lineEnding);
    void onCommandSent(int id);
    void parseGlobalStatus(const QString &key, const QString &value);
    void parseAxisStatus(int axisIndex, const QString &key, qreal value);
    void refreshAxisList(const QList<Axis *> &axisList);
    void initParameterBlock();
    void requestParameterBlock();
    void switchMode(MotionMode mode);

protected:
    TrioAllocator *m_vrAlloc = nullptr;
    TrioAllocator *m_tableAlloc = nullptr;
    TrioQueue *m_queue = nullptr;
    TrioStream *m_stream = nullptr;
    TrioWarden *m_warden = nullptr;

private:
    /*!< 禁止外界直接写入和读取socket */
    using QTcpSocket::read;
    using QTcpSocket::write;

    ControllerConfig m_config;
    ControllerStatus m_status;
    QList<Axis *> m_axis;                                                       /*!< 轴列表 */
    QMap<int, Axis *> m_axisIndex;                                              /*!< 以轴ID索引的轴列表 */
    QStringList m_globalKeys;
    int m_parameterRequestCount = 0;
    ControllerInfo m_info;
    QBitArray m_ioStatus;                                                       /*!< 当前 IO 状态 */
    int m_sineTable = -1;
    const int m_sineTableSize = 10240;

    QHash<QString, QStringList> m_programCache;                                 /*!< MC 中的程序缓存 */
    int m_blockContext = -2;
    QString m_blockAxisList;
    MotionMode m_currentMode = ModeUninitialized;
    MotionMode m_destMode = ModeManual;

    friend class ::MotionManager;
    friend class TrioQueue;
    friend class TrioStream;
    friend class TrioWarden;
};

}   /* namespace Motion */
