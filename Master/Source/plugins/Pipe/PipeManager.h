#pragma once

#include <Pipe/PanelAlarm.h>
#include <Pipe/TestModeBase.h>
#include <Pipe/PipeParameter.h>
#include <Pipe/PanelStatusFrame.h>
#include <Pipe/PipePanelBScanMain.h>
#include "Ars1000/PipeDataDef.h"

#ifdef OPC
#   include <Opc/SimpleFree.h>
#   include <Opc/DataDefine.h>
#endif

#include <QMenu>
#include <QEvent>
#include <QCheckBox>
#include <QMessageBox>
#include <QTimerEvent>
#include <QWidget>

class UTChannel;
class PanelAScan;
class PanelCScan;
class DialogAbout;
class PanelAutoSave;
class PipeParameter;
class ParameterCopy;
class PipePanelBScan;

namespace Ui {
class PipeManager;
}

class PipeManager : public QWidget
{
    Q_OBJECT

public:
    typedef QMap<qint32, PanelAScan*> AScanMap;
    typedef QMap<qint32, PanelCScan*> CScanMap;
    typedef TestModeBase::Mode Mode;
    typedef PipeDetect::OrderInfo OrderInfo;
    typedef PipeDetect::ProcessInfo ProcessInfo;
    typedef PipeDetect::AllParameterInfo AllParameterInfo;

    enum FILEOPERATETYPE
    {
        NEWFILE,
        OPENFILE,
        SAVEFILE
    };

public:
    explicit PipeManager(QWidget *parent = nullptr);
    ~PipeManager() override;
    static PipeManager *instance();
    static void ReportError(const QString &message);
    void ResetImageOptions();
    static QString ModeName(Mode mode);
    const AllParameterInfo *ParameterInfo();
    bool keepTestingFileExist();
    QMessageBox::StandardButton dialogYesOrNo(const QString &title, const QString &details);

#ifdef OPC
    bool WriteOPCItem(OPCHANDLE *phServer, VARIANT *itemArray, const unsigned long &ulSize);
#endif // OPC

signals:
    void ResultChanged(const TestResultType &result);
    void sendDataSegment(UTDataSegment sharedata);

public slots:
    void OnCurChannelChanged(qint32 ascanIndex, qint32 channelIndex);
    int OpenFile(QString name = "");
    int CloseFile();
    void WriteOPCData(const QString &key, const quint32 &value);
    void OnOrderParamChanged(const OrderInfo &info);                // 向电气写入所需要的材料信息
    void OnProcessInfoChanged(const ProcessInfo &info);             // 工艺参数变化
    void OnTestModeChanged(Mode mode);                              // 检测模式切换
    void OnKeepTestingChanged(bool keepTestingFlag);                // 断电续扫切换
    void ResetAScanNum();                                           // 重置A扫窗口数量
    void ResetCScanNum();                                           // 重置C扫窗口数量
    void SetReturnInfo2OPC(quint32 boxIndex, quint32 posX);         // 发送缺陷回位参数
    void OnStartFlagArrived(quint16 flag);                          // 探伤使能变化响应
    void OnProbeBoxStatusChanged(quint16 status);                   // 探头盒下落状态变化响应
    void SetCoderMark(quint16 mark);                                // 设置编码器标志位
    void SetResult(const TestResultType &result);                   // 判伤结果
    void OnFoundFeature(qreal pos);                                 // 发现缺陷处理喷标
    void OnViewPosChanged(const int &id, const QPointF &pt);        // 缺陷回位响应

protected:
    void closeEvent(QCloseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private slots:
    int NewFile();                                      // 新建文件
    void SaveFile(bool rename = false);                 // 保存文件
    void SaveAs();                                      // 另存为
   // void ClearFile();                                   // 清楚文件---
    void reloadFile();                                  // 重新加载数据文件
    void notifyError(const QString &message);           // 通知错误信息

/// 主窗口菜单栏按钮操作
    void on_actionConnect_triggered();                  // 链接下位机
    void on_actionDisconnect_triggered();               // 断开链接
    void on_actionGlobalOptions_triggered();            // 全局配置
    void on_actionOtherParameter_triggered();           // 其他参数窗口
    void on_actionParameterCopy_triggered();            // 参数复制
    void on_actionViewManager_triggered();              // 视图管理界面
    void on_actionOpen_triggered();                     // 打开文件
    void on_actionSave_triggered();                     // 保存文件
    void on_actionClose_triggered();                    // 关闭文件
    void on_actionReloadFile_triggered();               // 重新加载
    void on_actionReJudge_triggered();                  // 重新判伤
    void on_actionAbout_triggered();                    // 关于
    void on_actionInstrumentReady_triggered();          // 仪器就绪接口
    void on_actionLock_triggered();                     // 参数锁定
    void on_actionUnlock_triggered();                   // 参数解锁

    // 测试接口
    void on_actionTest_triggered();                     // 检测模式-模拟切换
    void on_actionKeepTesting_triggered();              // 断电续扫标志-模拟切换
    void on_actionReturnMark_triggered();               // 缺陷回位标志-模拟切换
    void on_actionTestStart_triggered();                // 开始探伤-模拟设置
    void on_actionTestEnd_triggered();                  // 结束探伤-模拟设置
    void on_actionProbeBoxStatus_triggered();           // 探头盒下落状态-模拟切换
    void on_actionReturnPos_triggered();                // 缺陷回位坐标-模拟发送
    void on_actionKeepTesting_2_triggered();            // 断点续扫位置-模拟发送
    void on_actionPaintMark_triggered();                // 喷标

private:
    void InitUI();
    void InitOPC();
    void InitTestMode(const PipeGlobalConfig &pipeConfig);
    void Connect();
    void CloseSetupWidgetAll();
    void SaveBScanImage();
    void resetMaps();
    bool createParameterBackFile();
    void removeParameterBackFile();

private:
    Ui::PipeManager *ui;
    Mode m_mode = TestModeBase::InvalidMode;
    PanelAlarm *m_PanelAlarm = nullptr;                                         // 报警窗口 DockWidget
    AScanMap m_mapAScan;                                                        // A扫指针容器
    PipePanelBScanMain *m_bscan = nullptr;                                      // B扫指针容器
    CScanMap m_mapCScan;                                                        // C扫指针容器
    QList<QDockWidget *> m_CScanDockVector;                                     // C扫Dock容器
    PipeParameter *m_PipeParameter = nullptr;                                   // 参数设置视图
    ParameterCopy *m_ParameterCopy = nullptr;                                   // 参数复制
    PanelStatusFrame *m_PanelStatusFrame = nullptr;
    TestModeBase *m_testMode = nullptr;
    PanelAutoSave *m_PanelAutoSave = nullptr;
    AllParameterInfo paramInfo;

    int TIMER_PAINT_MARK = -1;
    int TIMER_SAVE_FILE = -1;
    quint16 startFlag = 0;
    bool keepTestingStatus = false;
    qreal positionMax = 0;
    QMap<int, bool> paintMarkMap;
    QMap<int, bool> waitingPaintMap;
    QJsonObject parameterObject = QJsonObject();

    const quint32 ASCAN_COUNT_MAX = 25;
    const quint32 BSCAN_COUNT_MAX = 5;
    const quint32 CSCAN_COUNT_MAX = 5;
    const qreal keepTestingOffset = 315;
    const qreal keepTesingLengthMin = 2000;

#ifdef OPC
    CSimpleFree m_OPCComponent; // OPC线程
#endif // OPC
};
