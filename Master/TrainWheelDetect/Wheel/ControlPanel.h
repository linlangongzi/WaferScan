#pragma once

#include <QTime>
#include <QLineSeries>
#include <QDockWidget>
#include <utils/treemodel.h>
#include <Defect/DefectItem.h>
#include <QtCharts/QChartView>
#include <Wheel/WheelPosition.h>
#include "WheelConfig.h"
#include <Imaging/ReportWriterQtExcel.h>

#define GLOBAL_CONFIG_FILE "GlobalConfigOptions.json"

class PanelAScan;
class PanelBScan;
class PanelDefect;
class WheelDataset;
class PanelChannel;
class PanelInformation;

struct ReportImage
{
    QMap<int, QImage> bscanImages;
    QMap<int, QImage> wheelImages;
};

class PanelProbeBScan;
class PanelSensorDebug;
class PanelSensorStatus;

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QDockWidget
{
    Q_OBJECT

    Q_PROPERTY(GlobalConfigOptions getGlobalConfig READ getGlobalConfig WRITE setGlobalConfig NOTIFY globalConfigChanged)

public:
    enum POSE_e {
        CHN_NO = 0, // 通道号
        WHEEL_NO,   // 轮号
        TRAIN_NO    // 车号
    };

    enum Features {
        GATE_I1_TIME = 0,
        GATE_I1_AMP,
        GATE_A_TIME,
        GATE_A_AMP,
        GATE_B_TIME,
        GATE_B_AMP,
        GATE_I2_TIME,
        GATE_I2_AMP,
        SENSOR_NO,
        GATEB_WAVEWIDTH,
        TIMESTAMP,
        SOFT_CHANNEL_IDX,
        AEC_INDEX
    };
    Q_ENUM(Features)

    explicit ControlPanel(QWidget *parent = 0);
    ~ControlPanel();

    static ControlPanel *getInstance();
    static int channelIdToWheel(qint64 channelId);
    static int channelIdToChannel(qint64 channelId);
    static qint64 makeChannelId(int wheel, int channel);
    static bool isValidFrame(const UTSharedDataFrame &frame);
    static UTSharedDataFrame setWheelId(UTSharedDataFrame frame, int wheel);
    static int wheelId(const UTSharedDataFrame &frame);
    static int channelId(const UTSharedDataFrame &frame);
    static int timestamp(const UTSharedDataFrame &frame);
    static int wheelSetId(UTSharedDataFrame frame);
    static qreal frameValue(const UTSharedDataFrame &frame, int featureIndex, qreal scale);
    static int instrumentIndex(int channel);
    static int instrumentIndex(const UTSharedDataFrame &frame);
    static bool datasetValid(WheelDataset *pointer);

    bool newFile(const QString &dir);
    QString askFilePath();
    QString createFilePath();
    UTSharedDataSegment readFile(const QString &path);

    void createServer();
    void analyzeFrame(const UTSharedDataFrame &frame);
    void analyzeWheels(const UTSharedDataSegment &segment);
    void updatePlot(QtCharts::QChart *chart, const QList<WheelPosition> &wheels, qreal &begin, qreal &end);
    UTSharedDataSegment analysisRawData(const UTSharedDataSegment &dataRaw);    // 拆分数据包

    WheelSetLayout currentLayout() const;
    ChannelGroup channelGroup(int channel);
    QString channelGroupName(const UTSharedDataFrame &frame);

    GlobalConfigOptions getGlobalConfig() const;
    ChannelConfig channelConfig(const UTSharedDataFrame &frame) const;
    QHash<int, ChannelConfig> getChannelConfigs() const;
    TrainInfo getTrainInfo();
    QString trainType(const QString &trainId) const;
    QString wheelName(int wheel) const;
    QString wheelName(const UTSharedDataFrame &frame) const;
    QString sideByWheel(int wheel) const;
    QString sideByChannel(int channel) const;
    QString mergeText(const UTSharedDataFrame &frame) const;
    QString judgmentText(const UTSharedDataFrame &frame) const;
    QString extremeText(const UTSharedDataFrame &frame) const;
    QString frameText(const UTSharedDataFrame &frame) const;

    qreal channelRotation(const UTSharedDataFrame &frame);
    qreal probePosition(const UTSharedDataFrame &frame) const;
    int probeLocation(const UTSharedDataFrame &frame) const;
    int sensorId(const UTSharedDataFrame &frame) const;
    int aecCardId(const UTSharedDataFrame &frame) const;
    int wheelSetToIndex(int wheelSet, int insturmendIndex);
    int currentWheel() const {return m_currentWheel;}
    int currentChannel() const {return m_currentChannel;}
    WaveType waveType() const {return m_waveType;}

    bool isValidChannel(int channel) const;
    bool isSensorChannel(const UTSharedDataFrame &frame) const;

    void setWaveType(WaveType type);
    void setTrainType(const QString &type);
    void setTrainInfo(const TrainInfo &info);
    void setGlobalConfig(const GlobalConfigOptions &config);

signals:
    void scanStoped();
    void scanStarted();
    void currentWheelChanged(int wheel);
    void trainInfoChanged(const TrainInfo &info);
    void currentChannelChanged(int currentChannel);
    void globalConfigChanged(GlobalConfigOptions config);

public slots:
    void onRawMessageReceived(int type, const QVariant &message);
    void onRawDataReceived(const UTSharedDataSegment &dataRaw);
    void onDocumentChanged();
    void setCurrentWheel(int wheel);
    void setCurrentChannel(int currentChannel);

private slots:
    void loadCheatFile();
    void switchCheatFile();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionLoad_triggered();
    void on_actionStatistics_triggered();
    void on_actionGenerateReport_triggered();
    void on_actionOpenReport_triggered();
    void on_actionConfig_triggered();
    void on_actionPlot_triggered();
    void on_actionMakeImage_triggered();
    void on_modify_clicked(bool checked);

    // 测试
    void on_actionDestruct_triggered();
    void on_actionStructure_triggered();

private:
    void initializeUi();
    void registerMenu();
    void readConfigFile();
    void saveConfigFile();
    void resetSelectionChannel();
    void timerEvent(QTimerEvent *event);
    void setWheelDataset(WheelDataset *dataset);
    ReportWriter::ReportData makeReport(Utils::TreeModel<DefectItem, DefectItem> *model,
                                        const QJsonObject &json,
                                        ReportImage images);

private:
    static ControlPanel *instance;

    Ui::ControlPanel *ui;
    PanelAScan *panelAScan = NULL;                                              // A扫
    PanelBScan *panelBScan = NULL;                                              // B扫
    PanelDefect *panelDefect = NULL;                                            // 缺陷
    PanelChannel *panelChannel = NULL;                                          // 工艺
    PanelProbeBScan *panelProbeBScan = NULL;                                    // 探头B扫
    PanelInformation *panelInformation = NULL;                                  // 检测状态
    PanelSensorDebug *panelSensorDebug = NULL;                                  // 传感器调试
    PanelSensorStatus *panelSensorStatus = NULL;                                // 传感器监视
    WheelDataset *wheelDataset = NULL;

    int autoStartTimerId = 0;                                                   // 自动开始、结束检测功能 计时器Id
    QTime autoStartLastTime = QTime(0, 0, 0, 0);
    const int autoStartTimeSpace = 90000;                                       // 来车一次所需要持续的检测时间 ms

    bool started = false;                                                       // 检测开始、结束标志
    int trainStatusFromMfc = 0;                                                 // 板卡中 行车状态
    QHash<int, QList<WheelPosition> > confirmedWheels;
    QHash<int, QList<WheelPosition> > unconfirmedWheels;

    int m_currentWheel = 0;                                                     // 当前轮号
    int m_currentChannel = 10000;                                               // 当前通道
    WaveType m_waveType = WaveType::WaveSurface;                                // 伤波/底波标志
    TrainInfo trainInfo;                                                        // 车辆信息
    GlobalConfigOptions globalConfig;                                           // 全局配置
    QHash<int, ChannelConfig> channelConfigs;                                   // 通道参数配置
    QShortcut *cheatShortcut;
    QList<QShortcut *> cheatSwitchShortcut;
};

static inline ControlPanel *controlPanel()
{
    return ControlPanel::getInstance();
}
