#include "ControlPanel.h"
#include "DialogCheating.h"
#include "ui_ControlPanel.h"

#include <allrising/ThrottleUpdate.h>
#include <arsCore/Platform.h>
#include <BScanView/PanelBScan.h>
#include <Core/DialogAbout.h>
#include <Core/DocumentManager.h>
#include <Core/IDocument.h>
#include <Defect/PanelDefect.h>
#include <DialogGlobalConfig/DialogGlobalConfig.h>
#include <Core/ICore.h>
#include <PanelChannel/PanelChannel.h>
#include <PanelInformation/PanelInformation.h>
#include <PanelProbeBScan/PanelProbeBScan.h>
#include <PanelSensorDebug/PanelSensorDebug.h>
#include <PanelSensorStatus/PanelSensorStatus.h>
#include <ProbeNoManager/ProbeNoManager.h>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProgressDialog>
#include <QShortcut>
#include <QStatusBar>
#include <QTcpServer>
#include <QTcpSocket>
#include <QValueAxis>
#include <Ultrasonic/InstrumentStatusButton.h>
#include <Ultrasonic/UTInstrument.h>
#include <Ultrasonic/UTSetup.h>
#include <UTInstrumentDriver/PXI100/DataDef.h>
#include <Waveform/PanelAScan.h>
#include <Waveform/PanelUTSetup.h>
#include <Wheel/PanelDocumentInfo.h>
#include <Wheel/PanelTrainInfo.h>
#include <WheelDataset/WheelDataset.h>

using namespace QtCharts;
using namespace NS_PXI100;

static const char PART_TYPE[] = "TrainWheel";
static const char FILENAME_RAW[] = "Train.utstream";
static const char FILENAME_INFO[] = "TrainInfo.json";
static const char FILENAME_REPORT[] = "Report.xlsx";
static Q_LOGGING_CATEGORY(debug, "ut.wheel"/*, QtWarningMsg*/)

static const bool converterTrainInfo = registerJsonConverter<TrainInfo>();
static const bool converterWheelConfig = registerJsonConverter<WheelConfig>();
static const bool converterChannelGroup = registerJsonConverter<ChannelGroup>();
static const bool converterChannelConfig = registerJsonConverter<ChannelConfig>();
static const bool converterWheelSetLayout = registerJsonConverter<WheelSetLayout>();
static const bool converterFeatureFilterConfig = registerJsonConverter<FeatureFilterConfig>();
static const bool converterGlobalConfigOptions = registerJsonConverter<GlobalConfigOptions>();

ControlPanel *ControlPanel::instance = NULL;

template <typename Dest, typename Source>
Dest reinterpretAs(Source source)
{
    union U {
        Source s;
        Dest d;
    } data;
    data.s = source;
    return data.d;
}

ControlPanel::ControlPanel(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ControlPanel)
{
    if (instance) {
        qFatal("ControlPanel may only be constructed once.");
        return;
    }
    instance = this;

    ui->setupUi(this);

    initializeUi();
    Core::ICore::mainWindow()->statusBar()->addWidget(new InstrumentStatusButton(this), 1);
    registerMenu();
    createServer();

    connect(DocumentManager::instance(), &DocumentManager::currentDocumentChanged, this, &ControlPanel::onDocumentChanged);
    for (auto instrument: platform()->Instruments()->GetRegistry<UTInstrument>()) {
        connect(instrument, &UTInstrument::DataArrived, this, &ControlPanel::onRawDataReceived);
        connect(instrument, &UTInstrument::rawMessageArrived, this, &ControlPanel::onRawMessageReceived);
    }

    wheelDataset = new WheelDataset(this);
    setWheelDataset(wheelDataset);
    readConfigFile();

    cheatShortcut = new QShortcut(this);
    connect(cheatShortcut, &QShortcut::activated, this, &ControlPanel::loadCheatFile);
    cheatShortcut->setKey(QKeySequence(platform()->Get(K_CHEAT_SHORTCUT).toString(), QKeySequence::PortableText));
    for (int index = 1; index <= 9; ++index) {
        auto *shortcut = new QShortcut(this);
        shortcut->setKey(tr("F%1").arg(index));
        connect(shortcut, &QShortcut::activated, this, &ControlPanel::switchCheatFile);
        cheatSwitchShortcut << shortcut;
    }
}

ControlPanel::~ControlPanel()
{
    delete ui;

    saveConfigFile();

    if (autoStartTimerId != 0) {
        killTimer(autoStartTimerId);
    }
}

ControlPanel *ControlPanel::getInstance()
{
    return instance;
}

int ControlPanel::channelIdToWheel(qint64 channelId)
{
    return qint16(channelId >> 32);
}

int ControlPanel::channelIdToChannel(qint64 channelId)
{
    return (0xFFFFFFFF & channelId);
}

qint64 ControlPanel::makeChannelId(int wheel, int channel)
{
    return (qint64(wheel) << 32) | channel;
}

bool ControlPanel::isValidFrame(const UTSharedDataFrame &frame)
{
    /* 空指针的过滤 */
    if (frame.isNull()) {
        return false;
    }

    /* 错误数据的过滤 */
    const UTSharedDataFrame &f = frame;
    if (f->CData.size() < NUM_FEATURES_ADATA) {
        return false;
    }

    if (ControlPanel::channelId(frame) == 0) {
        return false;
    }

    /*  I闸门未触发帧过滤 */
    if (qFuzzyCompare(f->CData.value(ControlPanel::GATE_I1_TIME), 0)) {
        return false;
    }

    return true;
}

UTSharedDataFrame ControlPanel::setWheelId(UTSharedDataFrame frame, int wheel)
{
    frame->Pose[ControlPanel::WHEEL_NO] = wheel;
    return frame;
}

int ControlPanel::wheelId(const UTSharedDataFrame &frame)
{
    return reinterpretAs<int>(frame->Pose.value(ControlPanel::WHEEL_NO));
}

int ControlPanel::channelId(const UTSharedDataFrame &frame)
{
    return reinterpretAs<int>(frame->Pose.value(ControlPanel::CHN_NO));
}

int ControlPanel::timestamp(const UTSharedDataFrame &frame)
{
    return qRound(frame->CData.value(ControlPanel::TIMESTAMP));
}

int ControlPanel::wheelSetId(UTSharedDataFrame frame)
{
    return qMax(0, ControlPanel::wheelId(frame)) / 2 + 1;
}

qreal ControlPanel::frameValue(const UTSharedDataFrame &frame, int featureIndex, qreal scale)
{
    if (featureIndex < frame->CData.count()) {
        return frame->CData.value(featureIndex) * scale;
    }

    return 0;
}

int ControlPanel::instrumentIndex(const UTSharedDataFrame &frame)
{
    int channel = ControlPanel::channelId(frame);
    return instrumentIndex(channel);
}

bool ControlPanel::datasetValid(WheelDataset *pointer)
{
    if (pointer) {
        return true;
    }

    return false;
}

int ControlPanel::instrumentIndex(int channel)
{
    return qBound(0, channel / 10000 - 1, platform()->Instruments()->GetNumItems() - 1);
}

bool ControlPanel::newFile(const QString &dir)
{
    DocumentInfo info;
    info.partType = PART_TYPE;
    info.partIdentifier = trainInfo.trainId;

    QString error;
    if (!DocumentManager::createDocument(dir, info, &error)) {
        QMessageBox::warning(this, QString(), tr("创建文件失败: %1").arg(error));
        return false;
    }
    WriteFileAsJson(QDir(dir).filePath(FILENAME_INFO), convertToJson(trainInfo));
    DocumentManager::setCurrentDocument(dir);

    UTStreamFile file;
    QString fileName = QDir(dir).filePath(FILENAME_RAW);
    file.setFileName(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, QString(), tr("新建文件失败"), QMessageBox::Yes);
        return false;
    }

    return true;
}

QString ControlPanel::askFilePath()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                QString(),
                                                QString(),
                                                "所有文件 (Train.utstream;*_flangeData.txt);;原始探伤数据 (Train.utstream);;旧版本数据 (*_flangeData.txt)");
    if (path.isEmpty()) {
        qCDebug(debug) << "Ask file path: got empty path, aborting.";
        return QString();
    }

    QFileInfo fi(path);
    if (DocumentManager::isValidDocument(fi.path())) {
        DocumentManager::setCurrentDocument(fi.path());
    } else {
        qCDebug(debug) << "Ask file path: invalid document" << path << fi.path();
        DocumentManager::setCurrentDocument(QString());
        Core::ICore::mainWindow()->setWindowFilePath(path);
    }

    IDocument *doc = DocumentManager::currentDocument();
    DocumentInfo docInfo = doc->info();
    if (doc->isValid() && (docInfo.partType != PART_TYPE && !docInfo.partType.isEmpty())) {
        QMessageBox::warning(this, QString(), tr("不支持打开 %1 类型的数据文件").arg(docInfo.partType));
        return QString();
    }

    return path;
}

QString ControlPanel::createFilePath()
{
    QString trainId = trainInfo.trainId;
    QString path = globalConfig.documentPath;

    if (path.isEmpty()) {
        path = "Data/{date}/{train}-{date}-{time}";
    }

    path.replace("{date}", QDate::currentDate().toString("yyyy-MM-dd"));
    path.replace("{time}", QTime::currentTime().toString("hh-mm-ss"));
    path.replace("{train}", trainId);
    return path;
}

UTSharedDataSegment ControlPanel::readFile(const QString &path)
{
    UTStreamFile file;
    file.setFileName(path);
    UTSharedDataSegment segment;
    if (!file.open(QIODevice::ReadOnly)) {
        return segment;
    }

    static const int maximum = 10000;
    QProgressDialog progress(this);
    progress.setLabelText(tr("正在读取文件..."));
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setMaximum(maximum);

    QMultiMap<int, UTSharedDataFrame> frames;
    while (!file.atEnd() && !progress.wasCanceled()) {
        segment = file.ReadSegment(1024);
        for (const auto &frame: segment) {
            if (isSensorChannel(frame)) {
                continue;
            }
            frames.insert(timestamp(frame), frame);
        }

        progress.setValue(qreal(file.pos()) / file.size() * maximum);
    }

    if (progress.wasCanceled()) {
        return UTSharedDataSegment();
    }

    return frames.values();
}

void ControlPanel::createServer()
{
    QTcpServer *server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, [=]() {
        while (server->hasPendingConnections()) {
            QTcpSocket *socket = server->nextPendingConnection();
            ui->log->appendPlainText(tr("%1 已连接").arg(socket->peerAddress().toString()));

            connect(socket, &QTcpSocket::disconnected, [=](){
                ui->log->appendPlainText(tr("%1 断开连接").arg(socket->peerAddress().toString()));
            });

            connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
            connect(socket, &QTcpSocket::readyRead, [=]() {
                static const char delimeter = '\n';
                qint64 bytes = socket->bytesAvailable();
                QByteArray data = socket->peek(bytes);
                ui->log->appendPlainText(tr("new data: \n%1").arg(QString::fromUtf8(data)));

                int lastEol = data.lastIndexOf(delimeter);
                if (lastEol < 0) {
                    return;
                }

                socket->read(lastEol + 1);
                data.truncate(lastEol);

                QStringList lines = QString::fromUtf8(data).split(delimeter, QString::SkipEmptyParts);
                TrainInfo info;
                for (const QString &line: lines) {
                    QString key = line.section(':', 0, 0);
                    QString value = line.section(':', 1, -1);
                    QString type = trainType(value);

                    ui->log->appendPlainText(tr("key: %1, value: %2, type: %3").arg(key).arg(value).arg(type));

                    MetaType::writeGadgetProperty(info, key, value);
                    MetaType::writeGadgetProperty(info, QString("trainType"), type);
                }

                /*!< 如果已经手动开始检测又收到了车号，车辆信息会丢掉，数据会追加到已经开始检测的数据文件中 */
                if (autoStartTimerId == 0) {
                    setTrainInfo(info);
                    on_actionStart_triggered();
                    autoStartLastTime = QTime::currentTime();
                    autoStartTimerId = startTimer(autoStartTimeSpace);
                }
            });
        }
    });

    if (server->listen(QHostAddress::Any, 9999)) {
        ui->log->appendPlainText(tr("server ready"));
    } else {
        ui->log->appendPlainText(tr("init server failed"));
    }
}

void ControlPanel::analyzeFrame(const UTSharedDataFrame &frame)
{
    int time = ControlPanel::timestamp(frame);
    int location = controlPanel()->probePosition(frame);
    int instrumentIndex = ControlPanel::instrumentIndex(frame);
    auto &confirmed = confirmedWheels[instrumentIndex];
    auto &unconfirmed = unconfirmedWheels[instrumentIndex];

    // 判定是否属于已经分析出的真实车轮
    for (auto &wheelLocation: confirmed) {
        if (wheelLocation.add(time, location)) {
            return;
        }
    }

    // 在临时表中查找是否有符合的轮
    for (int index = unconfirmed.count() - 1; index >= 0; --index) {
        auto &wheelLocation = unconfirmed[index];
        if (wheelLocation.add(time, location)) {
            if (wheelLocation.isReal) {
                confirmed.append(wheelLocation);
                unconfirmed.removeAt(index);
            }

            return;
        }
    }

    WheelPosition newWheel;
    newWheel.add(time, location);
    unconfirmed.append(newWheel);
}

void ControlPanel::analyzeWheels(const UTSharedDataSegment &segment)
{
    if (!ControlPanel::datasetValid(wheelDataset)) {
        return;
    }

    confirmedWheels.clear();
    unconfirmedWheels.clear();

    int lastTime = 0;
    int frameCount = segment.count();
    const auto control = controlPanel();
    static int refreshStep = 10000;

    QProgressDialog progress(this);
    progress.setLabelText(tr("正在识别车轮..."));
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setMaximum(frameCount);

    for (int index = 0; index < frameCount; ++index) {
        const auto &frame = segment[index];
        int time = ControlPanel::timestamp(frame);
        int instrumentIndex = ControlPanel::instrumentIndex(frame);
        auto &confirmed = confirmedWheels[instrumentIndex];
        auto &unconfirmed = unconfirmedWheels[instrumentIndex];

        if (time != lastTime) {
            int elapsed = time - lastTime;
            if (elapsed < 0) {
                qFatal("Unexpected negative elapsed %d", elapsed);
            }
            for (auto &wheel: confirmed) {
                wheel.addConfidence(-elapsed);
            }

            for (int index = unconfirmed.count() - 1; index >= 0; --index) {
                auto &wheel = unconfirmed[index];
                wheel.addConfidence(-elapsed);

                if (wheel.hasFinished) {
                    unconfirmed.removeAt(index);
                }
            }

            lastTime = time;
        }

        analyzeFrame(frame);

        if (progress.wasCanceled()) {
            return;
        }

        if (index % refreshStep == 0) {
            progress.setValue(index);
        }
    }

    progress.setLabelText(tr("正在成像..."));
    const auto &radius = globalConfig.wheelConfig.radius;
    wheelDataset->clearData();

    UTSharedDataSegment unusedFrames;
    for (int index = 0; index < frameCount; ++index) {
        const auto &frame = segment[index];
        int time = ControlPanel::timestamp(frame);
        int count = 0;
        for (auto &confirmed: confirmedWheels) {
            for (int wheelSetIndex = 0; wheelSetIndex < confirmed.count(); ++wheelSetIndex) {
                auto &positions = confirmed[wheelSetIndex].positions;

                if (!positions.contains(time)) {
                    continue;
                }

                qreal &position = positions[time];
                qreal pos =  control->probePosition(frame);

                if (position >= pos - radius && position <= pos + radius) {
                    int wheelId = wheelSetToIndex(wheelSetIndex + 1, ControlPanel::instrumentIndex(frame));
                    wheelDataset->add(ControlPanel::setWheelId(frame, wheelId));
                    ++count;
                }
            }
        }

        if (count == 0) {
            unusedFrames << frame;
        }

        if (progress.wasCanceled()) {
            return;
        }

        if (index % refreshStep == 0) {
            progress.setValue(index);
        }
    }
    wheelDataset->unusedData = unusedFrames;

    emit wheelDataset->judgeReady();
}

void ControlPanel::updatePlot(QChart *chart, const QList<WheelPosition> &wheels, qreal &begin, qreal &end)
{
    qCDebug(debug) << "Updating plot" << chart << "with" << wheels.count() << "wheels";
    chart->removeAllSeries();

    for (int index = 0; index < wheels.count(); ++index) {
        const auto &wheel = wheels[index];
        qCDebug(debug) << "Adding series" << index << "/" << wheels.count() << "with" << wheel.positions.count() << "points";

        auto series = new QLineSeries;
        series->setUseOpenGL(true);
        for (auto it = wheel.positions.begin(); it != wheel.positions.end(); ++it) {
            qreal x = it.key();
            series->append(QPointF(it.key(), it.value()));

            begin = qMin(begin, x);
            end = qMax(begin, x);
        }
        series->setName(QString::number(index));

        chart->addSeries(series);
    }
    chart->createDefaultAxes();

    chart->axisY()->setRange(0, 13000);
}

UTSharedDataSegment ControlPanel::analysisRawData(const UTSharedDataSegment &dataRaw)
{
    UTSharedDataSegment segment;

    for (auto f: dataRaw) {
        for (int i = 0; i < AEC_CHN_NUM; i++) {
            int channel = f->CData[16 * 8 + 2 + i];
            UTSharedDataFrame frame = UTSharedDataFrame::create();

            frame->CData.resize(NUM_FEATURES_ADATA);                                                    // 增加伤波B闸门内波形宽度、时间戳、传感器
            frame->CData[ControlPanel::GATEB_WAVEWIDTH] = f->CData[NUM_FRAME_CDATA - 18 + i * 2 + 1];   // 伤波B闸门内波形宽度
            frame->CData[ControlPanel::TIMESTAMP] = f->CData[NUM_FRAME_CDATA - 2];                      // 时间戳
            frame->CData[ControlPanel::SENSOR_NO] = f->CData[NUM_FRAME_CDATA - 19];                     // 传感器
            frame->CData[ControlPanel::SOFT_CHANNEL_IDX] = f->CData[0];                                 // 软通道号
            frame->CData[ControlPanel::AEC_INDEX] = f->CData[1];                                        // AEC序号

            int nIdx = i * 16 + 2;                                                                  // 去掉头两个不要 软通道号和数据标签
            for (int j = nIdx; j < nIdx + 8; j++) {
                if (j < NUM_FRAME_CDATA) {
                    frame->CData[j - nIdx] = f->CData[j];
                }
            }

            frame->Pose.resize(3);
            frame->Pose[ControlPanel::CHN_NO] = channel;
            frame->Pose[ControlPanel::WHEEL_NO] = 0;                            // 由于下位机发来的轮组号没有参考价值,所以轮号暂时设置为0,便于后期处理
            frame->Pose[ControlPanel::TRAIN_NO] = 0;                            // 保留此位置以便兼容旧数据，新数据不记录车号

            if (ControlPanel::isValidFrame(frame)) {
                segment << frame;
            }
        }
    }

    return segment;
}

WheelSetLayout ControlPanel::currentLayout() const
{
    for (const auto &layout: globalConfig.wheelSetLayouts) {
        if (layout.trainType == globalConfig.trainType) {
            return layout;
        }
    }

    return WheelSetLayout();
}

ChannelGroup ControlPanel::channelGroup(int channel)
{
    channel = channel % 10000;
    for (auto group: globalConfig.channelGroups) {
        if (channel >= (group.start % 10000) && channel <= (group.end % 10000)) {
            return group;
        }
    }

    return ChannelGroup();
}

QString ControlPanel::channelGroupName(const UTSharedDataFrame &frame)
{
    if (frame.isNull()) {
        return QString();
    }

    return channelGroup(ControlPanel::channelId(frame)).name;
}

GlobalConfigOptions ControlPanel::getGlobalConfig() const
{
    return globalConfig;
}

ChannelConfig ControlPanel::channelConfig(const UTSharedDataFrame &frame) const
{
    return channelConfigs.value(ControlPanel::channelId(frame));
}

QHash<int, ChannelConfig> ControlPanel::getChannelConfigs() const
{
    return channelConfigs;
}

TrainInfo ControlPanel::getTrainInfo()
{
    if (ControlPanel::datasetValid(wheelDataset)) {
        trainInfo.wheelSetCount = wheelDataset->wheelSetCount();
    }

    return trainInfo;
}

QString ControlPanel::trainType(const QString &trainId) const
{
    QRegExp reg;
    for (const auto &layout: globalConfig.wheelSetLayouts) {
        reg.setPattern(layout.pattern);
        if (reg.exactMatch(trainId)) {
            return layout.trainType;
        }
    }

    return tr("未设置");
}

QString ControlPanel::wheelName(int wheel) const
{
    QList<WheelSetLayout> layouts = globalConfig.wheelSetLayouts;
    QString name = tr("%1%2").arg(sideByWheel(wheel)).arg(wheel / 2 + 1);;
    int count = layouts.size();

    for (int index = 0; index < count; ++index) {
        WheelSetLayout layout = layouts.value(index);
        if (globalConfig.trainType == layout.trainType) {                     // 找到对应的车型
            if (wheel < layout.wheelName.count()) {
                return layout.wheelName.value(wheel);
            }

            return name;                                                        // 对应车型中没有设置此轮名称
        }
    }

    return name;                                                                // 没有找到对应的车型
}

QString ControlPanel::wheelName(const UTSharedDataFrame &frame) const
{
    const int wheel = ControlPanel::wheelId(frame);
    return wheelName(wheel);
}

QString ControlPanel::sideByWheel(int wheel) const
{
    QString side = tr("右");
    if (wheel % 2 == 1) {
        side = tr("左");
    }

    return side;
}

QString ControlPanel::sideByChannel(int channel) const
{
    return channel >= 20000 ? tr("左") : tr("右");
}

QString ControlPanel::mergeText(const UTSharedDataFrame &frame) const
{
    ChannelConfig config = channelConfig(frame);
    QString merge = config.merge ? tr("最大值") : tr("最小值");
    return tr("%1 %2").arg(EnumToKey(config.featureType)).arg(merge);
}

QString ControlPanel::judgmentText(const UTSharedDataFrame &frame) const
{
    QString judgment;
    ChannelConfig config = channelConfig(frame);
    for (auto filter: config.judgments) {
        judgment.append(tr("%1(%2,%3) ").arg(EnumToKey(filter.featureType)).arg(filter.min).arg(filter.max));
    }

    return judgment;
}

QString ControlPanel::extremeText(const UTSharedDataFrame &frame) const
{
    if (frame.isNull()) {
        return 0;
    }

    FeatureFilterConfig::FeatureType type = channelConfig(frame).featureType;
    QString unit = (type % 2) ? tr("%") : tr("us");
    return tr("%1 %2").arg(frame->CData.value(type)).arg(unit);
}

QString ControlPanel::frameText(const UTSharedDataFrame &frame) const
{
    if (frame.isNull()) {
        return QString();
    }

    qreal timeI1 = frame->CData[ControlPanel::GATE_I1_TIME];
    qreal timeA = frame->CData[ControlPanel::GATE_A_TIME];
    qreal timeB = frame->CData[ControlPanel::GATE_B_TIME];
    qreal timeI2 = frame->CData[ControlPanel::GATE_I2_TIME];

    int channel = ControlPanel::channelId(frame);
    UTSetup *setup = platform()->Setups()->GetByID<UTSetup>(channel);
    if (!setup) {
        return QString();
    }

    double speed = setup->Get("Velocity").toDouble();

    return tr("轮号: %1\n").arg(wheelName(frame)) +
           tr("通道号: %1\n").arg(channel) +
           tr("时间戳: %1\n").arg(ControlPanel::timestamp(frame)) +
           tr("AEC卡编号: %1\n").arg(aecCardId(frame)) +
           tr("传感器编号: %1\n").arg(sensorId(frame)) +
           tr("I1闸门时间: %1us / %2mm (%3 m/s))\n").arg(timeI1).arg(setup->timeToDistance(timeI1)).arg(speed) +
           tr("I1闸门幅值: %1%\n").arg(frame->CData[ControlPanel::GATE_I1_AMP]) +
           tr("A闸门时间: %1us / %2mm (%3 m/s))\n").arg(timeA).arg(setup->timeToDistance(timeA)).arg(speed) +
           tr("A闸门幅值: %1%\n").arg(frame->CData[ControlPanel::GATE_A_AMP]) +
           tr("B闸门时间: %1us / %2mm (%3 m/s))\n").arg(timeB).arg(setup->timeToDistance(timeB)).arg(speed) +
           tr("B闸门幅值: %1%\n").arg(frame->CData[ControlPanel::GATE_B_AMP]) +
           tr("I2闸门时间: %1us / %2mm (%3 m/s))\n").arg(timeI2).arg(setup->timeToDistance(timeI2)).arg(speed) +
           tr("I2闸门幅值: %1%").arg(frame->CData[ControlPanel::GATE_I2_AMP]);
}

qreal ControlPanel::channelRotation(const UTSharedDataFrame &frame)
{
    int channel = ControlPanel::channelId(frame);
    ChannelGroup group = channelGroup(channel);

    int middle = channel % 10000;
    int start = group.start % 10000;
    int end = group.end % 10000;

    if (middle > start && middle < end) {
        return qreal(middle - start) / qreal(end - start) * 360.0;
    }

    return 0;
}

qreal ControlPanel::probePosition(const UTSharedDataFrame &frame) const
{
    return channelConfig(frame).position;
}

int ControlPanel::probeLocation(const UTSharedDataFrame &frame) const
{
    return channelConfig(frame).location;
}

int ControlPanel::sensorId(const UTSharedDataFrame &frame) const
{
    if (frame.isNull()) {
        return 0;
    }

    return channelConfigs.value(ControlPanel::channelId(frame)).sensorId;
}

int ControlPanel::aecCardId(const UTSharedDataFrame &frame) const
{
    if (frame.isNull()) {
        return 0;
    }

    return channelConfigs.value(ControlPanel::channelId(frame)).aecCardId;
}

int ControlPanel::wheelSetToIndex(int wheelSet, int insturmendIndex)
{
    if (wheelSet < 1) {
        return 0;
    }

    return (wheelSet - 1) * 2 + insturmendIndex;
}

bool ControlPanel::isValidChannel(int channel) const
{
    return channelConfigs.contains(channel);
}

bool ControlPanel::isSensorChannel(const UTSharedDataFrame &frame) const
{
    int channel = ControlPanel::channelId(frame);
    if ((channel >= 14001 && channel <= 14132) ||
        (channel >= 24001 && channel <= 24132)) {
        return true;
    }

    return false;
}

void ControlPanel::setWaveType(WaveType type)
{
    if (m_waveType == type) {
        return;
    }

    m_waveType = type;
    resetSelectionChannel();

    emit currentChannelChanged(m_currentChannel);
}

void ControlPanel::setCurrentWheel(int wheel)
{
    if (m_currentWheel == wheel || wheel < 0) {
        return;
    }

    m_currentWheel = wheel;
    emit currentWheelChanged(m_currentWheel);
}

void ControlPanel::setCurrentChannel(int currentChannel)
{
    if (m_currentChannel == currentChannel) {
        return;
    }

    m_currentChannel = currentChannel;
    resetSelectionChannel();

    emit currentChannelChanged(currentChannel);
}

void ControlPanel::loadCheatFile()
{
    QString file = platform()->Get(K_CHEAT_FILE_PATH).toString();
    if (file.isEmpty()) {
        return;
    }

    auto segment = readFile(file);

    /* 叠加随机噪声 */
    const qreal noise = platform()->Get(K_CHEAT_FILE_NOISE).toDouble();
    for (auto &frame: segment) {
        if (frame.isNull() || frame->CData.count() < 8) {
            continue;
        }
        for (int i = 1; i < 8; i += 2) {
            frame->CData[i] = qBound<CDataValue>(0, frame->CData[i] + qreal(qrand() - (RAND_MAX / 2)) / (RAND_MAX / 2) * noise, 100);
        }
    }

    analyzeWheels(segment);
}

void ControlPanel::switchCheatFile()
{
    QShortcut *shortcut = qobject_cast<QShortcut *>(sender());
    int index = cheatSwitchShortcut.indexOf(shortcut);
    if (shortcut && index >= 0) {
        QString path = convertFromJson<QStringList>(platform()->Get(K_CHEAT_FILE_LIST_PATH)).value(index);
        platform()->Set(K_CHEAT_FILE_PATH, path);
        auto *label = Core::ICore::mainWindow()->statusBar()->findChild<InstrumentStatusButton *>();
        if (label) {
            label->setToolTip(tr("%1: %2").arg(index + 1).arg(path));
        }
    }
}

void ControlPanel::setTrainType(const QString &type)
{
    globalConfig.trainType = type;
}

void ControlPanel::setTrainInfo(const TrainInfo &info)
{
    trainInfo = info;
    setTrainType(trainInfo.trainType);

    emit trainInfoChanged(trainInfo);
}

void ControlPanel::setGlobalConfig(const GlobalConfigOptions &config)
{
    channelConfigs.clear();
    globalConfig = config;

    for (auto channelConfig: globalConfig.channelConfig) {
        channelConfigs.insert(channelConfig.channel, channelConfig);
    }

    if (datasetValid(wheelDataset)) {
        wheelDataset->clearData();
    }

    emit globalConfigChanged(globalConfig);
}

void ControlPanel::onRawMessageReceived(int type, const QVariant &message)
{
    if (type == IInstrumentDriver::MessageDigitalIo) {                         /* 数字IO状态更新, 来自MFC卡 */
        if (!globalConfig.plcOpen) {
            return;
        }

        const QByteArray data = message.toByteArray();
        /* data:包含mfc卡地址0x0~0x110的所有数据
         * 根据关键字表:0x110地址定义: 驶入:101-0x5, 离开:011-0x3, 其他:111-0x7 */
        if (0x110 < data.size()) {
            quint8 status = data.at(0x110);
            if (trainStatusFromMfc == status) {
                return;
            }

            trainStatusFromMfc = status;
            if ((status & 0x7) == 0x5) {
                on_actionStart_triggered();
            } else if ((status & 0x7) == 0x3) {
                on_actionStop_triggered();
            }
        }
    }
}

void ControlPanel::onRawDataReceived(const UTSharedDataSegment &dataRaw)
{
    if (!started || dataRaw.isEmpty()) {
        return;
    }

    UTStreamFile file;
    file.setFileName(DocumentManager::currentDocumentDir().filePath(FILENAME_RAW));
    if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) {
        qCInfo(debug) << "Failed opening raw data file:" << file.errorString();
        return;
    }

    const auto data = analysisRawData(dataRaw);
    file.WriteSegment(data);
}

void ControlPanel::registerMenu()
{
    QMenu *m = new QMenu(tr("功能"), this);
    m->addAction(ui->actionStart);
    m->addAction(ui->actionStop);
    m->addSeparator();
    m->addAction(ui->actionLoad);
    m->addSeparator();
    m->addAction(ui->actionGenerateReport);
    m->addAction(ui->actionOpenReport);
    m->addSeparator();
    m->addAction(ui->actionStatistics);
    m->addSeparator();
    m->addAction(ui->actionConfig);
    m->addSeparator();
    m->addAction(ui->actionStructure);
    m->addAction(ui->actionDestruct);

    Core::ICore::mainWindow()->menuBar()->addMenu(m);
}

void ControlPanel::onDocumentChanged()
{
    auto json = ReadFileAsJson(DocumentManager::currentDocumentDir().filePath(FILENAME_INFO));
    const auto trainInfo = convertFromJson<TrainInfo>(json);
    setTrainInfo(trainInfo);
}

void ControlPanel::readConfigFile()
{
    GlobalConfigOptions globalConfigOptions = convertFromJson<GlobalConfigOptions>(ReadFileAsJson(GLOBAL_CONFIG_FILE));
    auto &groups = globalConfigOptions.channelGroups;
    if (groups.isEmpty()) {
        ChannelGroup g;

        g.start = 10000;
        g.end = 10095;
        g.name = tr("外分层");
        g.view = 1;
        groups << g;

        g.start = 10096;
        g.end = 10191;
        g.name = tr("内分层");
        g.view = 1;
        groups << g;

        g.start = 11000;
        g.end = 11191;
        g.name = tr("轮缘");
        g.view = 0;
        groups << g;

        g.start = 12000;
        g.end = 12119;
        g.name = tr("正向外侧");
        g.view = 2;
        groups << g;

        g.start = 12120;
        g.end = 12239;
        g.name = tr("正向内侧");
        g.view = 2;
        groups << g;

        g.start = 13000;
        g.end = 13119;
        g.name = tr("负向外侧");
        g.view = 3;
        groups << g;

        g.start = 13120;
        g.end = 13239;
        g.name = tr("负向内侧");
        g.view = 3;
        groups << g;
    }
    setGlobalConfig(globalConfigOptions);
}

void ControlPanel::saveConfigFile()
{
    WriteFileAsJson(GLOBAL_CONFIG_FILE, convertToJson(globalConfig));
}

void ControlPanel::resetSelectionChannel()
{
//    for (auto instrument: platform()->Instruments()->GetRegistry<UTInstrument>()) {
//        instrument->Channelchanged(m_currentChannel, m_waveType);
//    }
}

void ControlPanel::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == autoStartTimerId) {
        QTime curTime = QTime::currentTime();
        if (autoStartLastTime.msecsTo(curTime) >= autoStartTimeSpace) {
            if (autoStartTimerId != 0) {
                killTimer(autoStartTimerId);
            }

            autoStartTimerId = 0;
            autoStartLastTime = QTime(0, 0, 0, 0);
            on_actionStop_triggered();
        }
    }
}

void ControlPanel::setWheelDataset(WheelDataset *dataset)
{
    panelBScan->setWheelDataset(dataset);
    panelDefect->setWheelDataset(dataset);
    panelProbeBScan->setWheelDataset(dataset);
}

ReportWriter::ReportData ControlPanel::makeReport(Utils::TreeModel<DefectItem, DefectItem> *model, const QJsonObject &json, ReportImage images)
{
    ReportWriter::ReportData data;
    ReportWriter::ReportRow global;
    ReportWriter::ReportTable &globals = data[QString()];
    ReportWriter::ReportTable &defects = data["defects"];
    ReportWriter::ReportTable &bscanImages = data["bscanImages"];

    /* 全局key */
    for (auto key: json.keys()) {
        global[key] = json[key].toVariant();
    }

    QString pattern = QObject::tr("yyyy/MM/dd hh:mm");
    const auto documentInfo = DocumentManager::currentDocument()->info();
    global["testTime"] = documentInfo.creationTime.toString(pattern);
    global["printTime"] = QDateTime::currentDateTime().toString(pattern);

    /* 轮图 */
    global["picture"] = images.wheelImages.value(-1);                   // -1：全部车轮的key

    /* 有缺陷车轮的B扫 */
    QSet<int> wheels;
    QMap<int, DefectItem *> defectItems;
    model->forItemsAtLevel<1>([&](DefectItem *defectItem) {
        wheels << defectItem->data(DefectItem::ColWheel, Qt::EditRole).toInt();
        defectItems.insert(defectItem->data(DefectItem::ColIndex, Qt::DisplayRole).toInt(), defectItem);
    });

    QList<int> wheelsList = wheels.toList();
    qSort(wheelsList.begin(), wheelsList.end());

    for (auto wheel: wheelsList) {
        QVariantMap imageItem;
        imageItem["bscanName"] = wheelName(wheel);
        imageItem["image"] = images.bscanImages.value(wheel);
        imageItem["bscanIndex"] = wheel;
        bscanImages << imageItem;
    }

    // 缺陷列表
    for (const auto &item: defectItems) {
        auto row = item->toVariantMap();
        defects << row;
    }

    globals << global;

    return data;
}

void ControlPanel::initializeUi()
{
    auto main = Core::ICore::mainWindow();

    panelAScan = new PanelAScan(this);
    QDockWidget *ascanDock = new QDockWidget(tr("A扫"), this);
    ascanDock->setWidget(panelAScan);
    ascanDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    main->addDockWidget(Qt::TopDockWidgetArea, ascanDock);
//    connect(panelAScan, &PanelAScan::setupChanged, this, &ControlPanel::onSetupChanged);
    connect(this, &ControlPanel::currentChannelChanged, panelAScan, &PanelAScan::SetChannelIndex);

    panelDefect = new PanelDefect(this);
    panelDefect->setAllowedAreas(Qt::AllDockWidgetAreas);
    main->addDockWidget(Qt::TopDockWidgetArea, panelDefect);
    main->tabifyDockWidget(ascanDock, panelDefect);
    ascanDock->raise();
    connect(this, &ControlPanel::currentWheelChanged, panelDefect, &PanelDefect::setCurrentWheel);

    panelChannel = new PanelChannel(this);
    Core::ICore::mainWindow()->AddPanelToDockWidget(panelChannel, Qt::TopDockWidgetArea, tr("通道切换"), "PanelChannel");
    connect(this, &ControlPanel::currentChannelChanged, panelChannel, &PanelChannel::setCurrentChannel);

    panelSensorDebug = new PanelSensorDebug(this);
    QDockWidget *sensorDebugDock = new QDockWidget(tr("传感器调试"), this);
    sensorDebugDock->setWidget(panelSensorDebug);
    sensorDebugDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    main->addDockWidget(Qt::TopDockWidgetArea, sensorDebugDock);
    connect(this, &ControlPanel::currentChannelChanged, panelSensorDebug, &PanelSensorDebug::onChannelChanged);

    panelSensorStatus = new PanelSensorStatus(this);
    QDockWidget *sensorStatusDock = new QDockWidget(tr("传感器监视"), this);
    sensorStatusDock->setWidget(panelSensorStatus);
    sensorStatusDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    main->addDockWidget(Qt::TopDockWidgetArea, sensorStatusDock);

    panelBScan = new PanelBScan(this);
    main->addDockWidget(Qt::BottomDockWidgetArea, panelBScan);
    connect(this, &ControlPanel::currentWheelChanged, panelBScan, &PanelBScan::refreshLines);

    panelProbeBScan = new PanelProbeBScan(this);
    main->addDockWidget(Qt::BottomDockWidgetArea, panelProbeBScan);
    connect(this, &ControlPanel::currentWheelChanged, panelProbeBScan, &PanelProbeBScan::refresh);
    connect(this, &ControlPanel::currentChannelChanged, panelProbeBScan, &PanelProbeBScan::refresh);

    panelInformation = new PanelInformation(main);
    panelInformation->setVisible(false);
    panelInformation->setGeometry(800, 0, 300, 24);
    connect(this, &ControlPanel::scanStoped, panelInformation, &PanelInformation::OnScanEnd);
    connect(this, &ControlPanel::scanStarted, panelInformation, &PanelInformation::OnScanStart);

    // 车轮识别的调试功能暂时不给客户开启，避免导致图源过多视图卡顿
    ui->toolbar->hide();
    ui->tabWidget->removeTab(1);
}

void ControlPanel::on_actionStart_triggered()
{
    if (started) {
        return;
    }

    emit scanStarted();

    /* 写传感器状态，避免误操作导致检测失败 */
    auto data = UTSharedDataSimulation::create();
    data->sEnable = false;
    std::memset(data->sLightEye, 0, 9);

    for (auto instrument: platform()->Instruments()->GetRegistry<UTInstrument>()) {
        instrument->sendRawMessage(IInstrumentDriver::MessageTrainSensor, QVariant::fromValue(data));
        instrument->sendRawMessage(IInstrumentDriver::MessageTrainScanStatus, 0);
    }

    /* 清除视图、伤列表 */
    if (datasetValid(wheelDataset)) {
        wheelDataset->clearData();
    }

    /* 为了让下位机有时间将reset前的无效数据丢掉 */
    QThread::msleep(1000);

    /* 新建数据文件、开始接收数据 */
    newFile(createFilePath());

    started = true;

    const auto document = DocumentManager::currentDocument();
    auto info = document->info();
    info.aquisitionStart = QDateTime::currentDateTime();
    document->setInfo(info);

    auto setupPath = currentLayout().setupPath;
    if (QFileInfo(setupPath).exists()) {
        QJsonArray setups = ReadFileAsJson(setupPath).toArray();

        for (QJsonValue v: setups) {
            QJsonObject obj = v.toObject();
            auto id = ConfigObject::ToObjectID(obj["ID"]);
            UTSetup *setup = platform()->Setups()->GetByID<UTSetup>(id);

            if (!setup) {
                continue;
            }

            for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
                setup->Set(it.key(), it.value());
            }
        }
    }
}

void ControlPanel::on_actionStop_triggered()
{
    if (!started) {
        return;
    }

    started = false;

    emit scanStoped();

    /* 开启传感器调试，避免由于水位高度问题导致继电器频繁切换 */
    auto data = UTSharedDataSimulation::create();
    data->sEnable = true;
    std::memset(data->sLightEye, 0, 9);

    for (auto instrument: platform()->Instruments()->GetRegistry<UTInstrument>()) {
        instrument->sendRawMessage(IInstrumentDriver::MessageTrainSensor, QVariant::fromValue(data));
        instrument->sendRawMessage(IInstrumentDriver::MessageTrainScanStatus, 1);
    }

    const auto document = DocumentManager::currentDocument();
    auto info = document->info();
    info.aquisitionEnd = QDateTime::currentDateTime();
    document->setInfo(info);

    if (platform()->Get(K_CHEAT_AFTER_SCAN).toBool()) {
        loadCheatFile();
    } else {
        analyzeWheels(readFile(DocumentManager::currentDocumentDir().filePath(FILENAME_RAW)));
    }

    if (globalConfig.autoReport) {
        on_actionGenerateReport_triggered();
    }
}

void ControlPanel::on_actionLoad_triggered()
{
    const QString path = askFilePath();
    if (path.isEmpty()) {
        return;
    }

    if (platform()->Get(K_CHEAT_AFTER_SCAN).toBool()) {
        loadCheatFile();
    } else {
        analyzeWheels(readFile(path));
    }
}

void ControlPanel::on_actionStatistics_triggered()
{
    if (!datasetValid(wheelDataset)) {
        return;
    }

    QStringList csv;
    csv << tr("channel,totalCount\n");
    for (auto channelConfig: globalConfig.channelConfig) {
        int dataCount = 0;
        int channel = channelConfig.channel;
        int wheelCount = wheelDataset->wheelSetCount() * 2;

        for (int wheel = 0; wheel < wheelCount; ++wheel) {
            qint64 channelId = ControlPanel::makeChannelId(wheel, channel);
            dataCount += wheelDataset->channelFrames(channelId).count();
        }

        csv << tr("%1,%2\n").arg(channel).arg(dataCount);
    }

    UTStreamFile file;
    QString fileName = QFileDialog::getSaveFileName(this, tr("统计"), tr("统计文件名称"), tr("(*.csv)"));
    file.setFileName(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "", tr("文件打开失败"));
        return;
    }

    QByteArray data;
    for (auto line: csv) {
        data.append(line.toUtf8());
    }

    file.write(data);
}

void ControlPanel::on_actionGenerateReport_triggered()
{
    if (!datasetValid(wheelDataset)) {
        QMessageBox::warning(this, QString(), tr("打印报失败，数据集不存在"));
        return;
    }

    /* 图元的获取 */
    ReportImage images;
    images.wheelImages.insert(-1, panelDefect->renderWheels(1600));             // -1:所有轮 0+轮号

    int wheelCount = wheelDataset->wheelSetCount() * 2;
    for (int wheelId = 0; wheelId < wheelCount; ++wheelId) {
        ControlPanel::getInstance()->setCurrentWheel(wheelId);
        images.bscanImages.insert(wheelId, panelBScan->renderBScan());
    }
    setCurrentWheel(0);

    ReportWriterQtExcel report(this);
    QString outputPath = DocumentManager::currentDocumentDir().filePath(FILENAME_REPORT);

    const auto info = convertToJson(trainInfo).toObject();
    bool success = report.Generate(globalConfig.templatePath, outputPath, makeReport(panelDefect->model(), info, images));
    if (!success) {
        QMessageBox::warning(this, QString(), tr("导出报表失败 - %1\n%2").arg(report.GetLastError()).arg(outputPath));
        return;
    }
}

void ControlPanel::on_actionOpenReport_triggered()
{
    const auto reportPath = DocumentManager::currentDocumentDir().filePath(FILENAME_REPORT);
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(reportPath))) {
        QMessageBox::information(this, QString(), tr("打开报表失败\n%1").arg(reportPath));
        return;
    }
}

void ControlPanel::on_actionConfig_triggered()
{
    static int cheatCount = 0;
    auto mod = QApplication::keyboardModifiers();
    if (mod.testFlag(Qt::ShiftModifier)) {
        cheatCount++;
        if (cheatCount > 3) {
            DialogCheating cheating;
            cheating.exec();
            cheatShortcut->setKey(QKeySequence(platform()->Get(K_CHEAT_SHORTCUT).toString(), QKeySequence::PortableText));
            return;
        }
    }
    DialogGlobalConfig dialog;
    dialog.setConfigOptions(globalConfig);
    if (dialog.exec() == QDialog::Accepted) {
        setGlobalConfig(dialog.configOptions());
        saveConfigFile();
    }
}

void ControlPanel::on_actionPlot_triggered()
{
    qreal begin = Q_INFINITY;
    qreal end = -Q_INFINITY;
    updatePlot(ui->chartRight->chart(), confirmedWheels.value(0), begin, end);
    updatePlot(ui->chartLeft->chart(), confirmedWheels.value(1), begin, end);
    ui->chartRight->chart()->axisX()->setRange(begin, end);
    ui->chartLeft->chart()->axisX()->setRange(begin, end);
}

void ControlPanel::on_actionMakeImage_triggered()
{
    UTSharedDataSegment segment = readFile(askFilePath());
    if (segment.isEmpty()) {
        return;
    }

    int start = ControlPanel::timestamp(segment.first());
    int end = ControlPanel::timestamp(segment.last());
    const auto control = controlPanel();
    int division = 1000;
    qreal scale = division / qreal(end - start);
    QMap<QPair<int, int>, int> count;
    for (const auto &frame: segment) {
        int position = control->probeLocation(frame);
        if (ControlPanel::channelId(frame) >= 20000) {
            position += 500;
        }
        int time = ControlPanel::timestamp(frame);
        int timeKey = (time - start) * scale;
        count[qMakePair(position, timeKey)]++;
    }

    QFile file("statistics.m");
    file.open(QIODevice::ReadWrite);
    QTextStream s(&file);
    s << "m = zeros(" << 1000 << "," << division << ");" << endl;
    for (auto it = count.begin(); it != count.end(); ++it) {
        s << "m(" << (it.key().first + 1) << "," << (it.key().second + 1) << ") = " << it.value() << ";" << endl;
    }
    s << "image(m, 'CDataMapping', 'scaled');" << endl;
    s << "colorbar;" << endl;
}

void ControlPanel::on_modify_clicked(bool checked)
{
    if (checked) {
        ui->modify->setText(tr("保存"));
    } else {
        ui->modify->setText(tr("修改"));
    }

    ui->trainInfo->actionModify(checked);
    ui->documentInfo->actionModify(checked);
}

void ControlPanel::on_actionDestruct_triggered()
{
    if (wheelDataset) {
        delete wheelDataset;
        wheelDataset = NULL;
        setWheelDataset(wheelDataset);
        readConfigFile();
    }
}

void ControlPanel::on_actionStructure_triggered()
{
    if (wheelDataset) {
        delete wheelDataset;
    }

    wheelDataset = new WheelDataset(this);
    setWheelDataset(wheelDataset);
    readConfigFile();
}
