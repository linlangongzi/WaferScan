#include "PipeManager.h"
#include "ui_PipeManager.h"
#include "ParameterCopy.h"
#include <Core/CoreConstants.h>
#include <Core/DialogAbout.h>
#include <Core/FileOperateManager.h>
#include <Core/ICore.h>
#include <Core/Platform.h>
#include <Imaging/ImageOptions.h>
#include <Imaging/PanelCScan.h>
#include <Imaging/UTDocument.h>
#include <QCloseEvent>
#include <QFileDialog>
#include <QGridLayout>
#include <QMap>
#include <QMutex>
#include <QProgressDialog>
#include <QSplitter>
#include <QTableWidget>
#include <QVector>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTInstrument.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/PanelAScan.h>
#include <allrising/Util.h>

#ifdef OPC
HANDLE hOPCInitEvent;       // OPC初始化
HANDLE hOPCReadEvent;       // OPC读值
HANDLE hOPCWriteEvent;      // OPC写值
extern CLSID clsOPCServer;

QMutex g_ItemCS;
GlobalItemValue g_ItemValue;
#endif

quint16 g_startFlag;

static PipeManager *m_instance = nullptr;

using namespace Core::Constants;

PipeManager::PipeManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PipeManager)
{
    m_instance = this;
    ui->setupUi(this);

    PipeGlobalConfig pipeConfig = convertFromJson<PipeGlobalConfig>(ReadFileAsJson(platform()->Get("GlobalConfigFileName").toString()));

    for (int index = 0; index < pipeConfig.channels.count(); ++index) {
        UTChannel *channel = UltrasonicManager::channel(index);
        if (channel) {
            const PipeChannelConfig &info = pipeConfig.channels.at(index);
            channel->setName(info.channelName());
            channel->setDescription(info.channelDescription());
        }
    }

    InitUI();
    InitTestMode(pipeConfig);

#ifdef OPC
    InitOPC();
#endif // OPC

    Connect();

#ifdef ARS_ENABLE_LOG                                                           /* 这个开关仅在使用Release脚本时才开启 */
    /* Debug日志面板在所有组件加载前加载。 */
    PanelDebugLog *log = new PanelDebugLog(this);
    log->setAllowedAreas(Qt::AllDockWidgetAreas);
    addDockWidget(Qt::BottomDockWidgetArea, log);
    log->close();
#endif

#if defined(Q_OS_WIN32) && !defined(Q_OS_WIN64)
    qCritical() << tr("编译环境不是64位。");
#endif

    TIMER_PAINT_MARK = startTimer(200);
    TIMER_SAVE_FILE = startTimer(300);
}

PipeManager::~PipeManager()
{
    m_instance = nullptr;
    delete ui;

    WriteFileAsJsonArray<PlotAlarmGroup>("AlarmOptions.json", m_PanelAlarm->alarmGroups());
    WriteFileAsJson(platform()->Get("GlobalConfigFileName").toString(), convertToJson(m_testMode->globalConfig()));

#ifdef OPC
    m_OPCComponent.StopThread();
#endif // OPC

    if (TIMER_PAINT_MARK > 0) {
        killTimer(TIMER_PAINT_MARK);
    }

    if (TIMER_SAVE_FILE > 0) {
        killTimer(TIMER_SAVE_FILE);
    }
}

PipeManager *PipeManager::instance()
{
    return m_instance;
}

void PipeManager::ReportError(const QString &message)
{
    qCritical() << "ReportError" << message;
    QMetaObject::invokeMethod(instance(), "notifyError", Q_ARG(QString, message));
}

void PipeManager::ResetImageOptions()
{
    int groupMask = m_testMode->DataManager()->GetProbeBoxStatus();
    PipeGlobalConfig globalConfig = m_testMode->globalConfig();
    const QList<ImageOptions> imageOptions = globalConfig.imageOptions(m_mode, groupMask, paramInfo.orderInfo);
    for (int index = 0; index < imageOptions.count(); ++index) {
        UTLayer *layer = document()->GetLayerByID(index + 1);
        if (layer) {
            layer->SetImageOptions(imageOptions[index]);
            layer->Reset();
        } else {
            qWarning() << "Can't reset image options for layer" << index;
        }
    }

    m_bscan->setViewOptions(globalConfig.plots(m_mode, groupMask, paramInfo.orderInfo));
}

QString PipeManager::ModeName(Mode mode)
{
    switch (mode) {
    case Mode::FinalPipe:   return tr("成品管");
    case Mode::FinalBar:    return tr("成管棒");
    case Mode::SamplePipe:  return tr("试样管");
    case Mode::SampleBar:   return tr("试样棒");
    default:                return tr("手动");
    }
}

const PipeManager::AllParameterInfo *PipeManager::ParameterInfo()
{
    return &paramInfo;
}

/*!
 * \brief MainWindow::keepTestingFileExist 获取断电续扫临时文件的存在状态
 * \return  true:  存在
 *         false:  不存在
 */
bool PipeManager::keepTestingFileExist()
{
    QFile file(QString("%1%2").arg(DATA_BACK_FILE_NAME, ARS_FILE_EXT_PARAMETER));
    if (!file.open(QFile::ReadOnly)) {
        return false;
    }

    file.close();
    return true;
}

QMessageBox::StandardButton PipeManager::dialogYesOrNo(const QString &title, const QString &details)
{
    return QMessageBox::question(this, title, details, QMessageBox::Yes | QMessageBox::No);
}

#ifdef OPC
bool PipeManager::WriteOPCItem(OPCHANDLE *phServer, VARIANT *itemArray, const unsigned long &ulSize)
{
    IUnknown *pIunknown;
    HRESULT hr = m_OPCComponent.GetIO2Interface(IID_IOPCAsyncIO2, (IUnknown**)&pIunknown);

    if (FAILED(hr)) {
        return FALSE;
    }

    IOPCAsyncIO2 *m_IOPCAsyncIO2 = nullptr;
    hr = pIunknown->QueryInterface(IID_IOPCAsyncIO2, (void**)&m_IOPCAsyncIO2);

    if (FAILED(hr)) {
        pIunknown->Release();
        return FALSE;
    }

    DWORD dwCancelID;
    // 钢板参数
    // 探伤模式
    HRESULT *pErrors;
    HRESULT r1;
    LPWSTR ErrorStr;

    if (m_OPCComponent.m_pErrors[2] != S_OK) {  // Item not available
        QMessageBox::warning(nullptr, "Error", tr("OPC Item SysSelfTestResult不可用，不能用异步读功能!"), QMessageBox::Yes, QMessageBox::Yes);
        m_IOPCAsyncIO2->Release();
        pIunknown->Release();
        return FALSE;
    }

    r1 = m_IOPCAsyncIO2->Write( ulSize,         // [in]  写 1 Item
                                phServer,       // [in]  定义的Item
                                itemArray,      // [in]  定义的值
                                2,              // [in]  客户 transaction ID
                                &dwCancelID,    // [out] 服务器 Cancel ID
                                &pErrors        // [out] 服务器返回的错误码
                                );

    if (r1 == S_FALSE) {
        m_OPCComponent.m_IOPCServer->GetErrorString(pErrors[0], LOCALE_ID, &ErrorStr);
    }

    if (FAILED(r1)) {
        QMessageBox::warning(nullptr, "Error", tr("异步写Item 错误"), QMessageBox::Yes, QMessageBox::Yes);
    } else { // release [out] parameter in case of not failed
        CoTaskMemFree(pErrors);
    }

    m_IOPCAsyncIO2->Release();
    pIunknown->Release();

    ::WaitForSingleObject(hOPCWriteEvent, INFINITE);
    ResetEvent(hOPCWriteEvent);

    return TRUE;
}
#endif // OPC

/*!
 * \brief MainWindow::OnCurChannelChanged
 * \param ascanIndex
 * \param channelIndex
 * 通过报警列表切换对应A扫窗口中的监视通道
 */
void PipeManager::OnCurChannelChanged(qint32 ascanIndex, qint32 channelIndex)
{
    m_mapAScan[ascanIndex]->setChannelId(channelIndex);
}

/*!
 * \brief MainWindow::OpenFile 打开文件
 * \return -1: 取消操作
 *         -2: 打开文件失败
 *          0: 成功
 *        ···: 同Close结果
 */
int PipeManager::OpenFile(QString name)
{
    resetMaps();

    // 关闭原来数据文件
    int ret = CloseFile();
    if (ret != 0) {
        return ret;
    }

    // 清空缓冲区
    document()->clear();

    // 打开文件
    QString fileName = name;
    if (fileName == "") {
        fileName = FileOperateManager::openFile();
        if ( fileName.isEmpty() ) {
            return -1;
        }
    }

    ret = document()->Open(fileName);
    if (ret != 0) {
        QMessageBox::warning(this, QString(), tr("文件打开失败: %1").arg(ret));
        return -2;
    }

    m_bscan->setViewOptions(convertFromJson<QList<PlotView>>(document()->Get("BScanOptions")));
    m_bscan->resetData();
    document()->RequestDataReadAll();

    if (m_PipeParameter) {
        QMessageBox::StandardButton button = QMessageBox::question(this,
                                                                   tr("数据加载"),
                                                                   tr("是否加载数据文件中的订单参数"),
                                                                   QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes) {
            m_PipeParameter->FromJson(document()->Get("OrderParameter"), false);
        }
    }

    return 0;
}

int PipeManager::CloseFile()
{
    if (!document()->Close()) {
        int ret = QMessageBox::question(this, tr("关闭文件"),
            tr("已打开的数据文件有数据改动，是否保存?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);

        if (ret == QMessageBox::Save) {
            QFileInfo info(document()->GetCurrentFileName());

            if (info.path() == ".") {  // 无路径，是新建文件
                QString fileName = FileOperateManager::saveFile();

                if (fileName.isEmpty()) {
                    return -1;
                }

                int saveRet = document()->Save(fileName);
                if (!saveRet) {
                    return saveRet;
                }
            } else {  // 已经是已打开的数据文件
                return document()->Save();
            }
        } else if (ret == QMessageBox::Discard) {
            document()->clear();
            return -1;
        } else {
            return -1;
        }
    }

    return 0;
}

void PipeManager::WriteOPCData(const QString &key, const quint32 &value)
{
#ifdef OPC
    OPCHANDLE phServer;
    VARIANT values;

    if (key == "WSState") {                                     //写 仪器状态
        phServer = m_OPCComponent.m_pItemResult[1].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "PipeLength") {                           //写 管棒长度
        phServer = m_OPCComponent.m_pItemResult[2].hServer;
        values.vt = VT_UI4;
        values.ulVal = value;
    } else if (key == "PipeDiameter") {                         //写 管棒直径
        phServer = m_OPCComponent.m_pItemResult[3].hServer;
        values.vt = VT_UI4;
        values.ulVal = value;
    } else if (key == "PipeTestResult") {                       //写 探伤结果
        phServer = m_OPCComponent.m_pItemResult[4].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "SWTestMode") {                           //读 探伤模式
        phServer = m_OPCComponent.m_pItemResult[5].hServer;
        values.vt = VT_UI4;
        values.ulVal = value;
    } else if (key == "ProbeBoxStatus") {                       //写 探头盒数量
        phServer = m_OPCComponent.m_pItemResult[6].hServer;
        values.vt = VT_UI4;
        values.ulVal = value;
    } else if (key == "KeepTesting") {                          //读 续扫标志
        phServer = m_OPCComponent.m_pItemResult[7].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "KTPosition") {                           //写 续扫位置坐标
       phServer = m_OPCComponent.m_pItemResult[8].hServer;
       values.vt = VT_UI4;
       values.ulVal = value;
    } else if (key == "ReturnMark") {                           //读 缺陷回位标志
        phServer = m_OPCComponent.m_pItemResult[9].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ReturnBoxIndex") {                       //写 回位探头盒序号
        phServer = m_OPCComponent.m_pItemResult[10].hServer;
        values.vt = VT_UI4;
        values.ulVal = value;
    } else if (key == "ReturnPosition") {                       //写 续扫位置坐标
        phServer = m_OPCComponent.m_pItemResult[11].hServer;
        values.vt = VT_UI4;
        values.ulVal = value;
    } else if (key == "ProbeInFlawFlag1") {
        phServer = m_OPCComponent.m_pItemResult[12].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeInFlawFlag2") {
        phServer = m_OPCComponent.m_pItemResult[13].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeInFlawFlag3") {
        phServer = m_OPCComponent.m_pItemResult[14].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeInFlawFlag4") {
        phServer = m_OPCComponent.m_pItemResult[15].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeInFlawFlag5") {
        phServer = m_OPCComponent.m_pItemResult[16].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeOutFlawFlag1") {
        phServer = m_OPCComponent.m_pItemResult[17].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeOutFlawFlag2") {
        phServer = m_OPCComponent.m_pItemResult[18].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeOutFlawFlag3") {
        phServer = m_OPCComponent.m_pItemResult[19].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeOutFlawFlag4") {
        phServer = m_OPCComponent.m_pItemResult[20].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeOutFlawFlag5") {
        phServer = m_OPCComponent.m_pItemResult[21].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeWallFlawFlag1") {
        phServer = m_OPCComponent.m_pItemResult[22].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeWallFlawFlag2") {
        phServer = m_OPCComponent.m_pItemResult[23].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeWallFlawFlag3") {
        phServer = m_OPCComponent.m_pItemResult[24].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeWallFlawFlag4") {
        phServer = m_OPCComponent.m_pItemResult[25].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "ProbeWallFlawFlag5") {
        phServer = m_OPCComponent.m_pItemResult[26].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "TestStartFlag") {
        phServer = m_OPCComponent.m_pItemResult[27].hServer;
        values.vt = VT_UI1;
        values.bVal = value;
    } else if (key == "PaintPosition") {
        phServer = m_OPCComponent.m_pItemResult[28].hServer;
        values.vt = VT_UI4;
        values.ulVal = value;
    }

    WriteOPCItem(&phServer, &values, 1);
#else // OPC
    Q_UNUSED(key);
    Q_UNUSED(value);
#endif // OPC
}

void PipeManager::closeEvent(QCloseEvent *event)
{
    if (CloseFile()) {
        event->ignore();
        return;
    }
    if (QMessageBox::question(this, QString(), tr("确定要退出吗"), QMessageBox::Yes | QMessageBox::No ) != QMessageBox::Yes) {
        return event->ignore();
    }
}

void PipeManager::timerEvent(QTimerEvent *event)
{
    /* 定时喷标 */
    if (event->timerId() == TIMER_PAINT_MARK) {
        if (!waitingPaintMap.isEmpty()) {
            QMap<int, bool>::iterator it = waitingPaintMap.begin();
            WriteOPCData("PaintPosition", it.key());
            waitingPaintMap.remove(it.key());
        }
    }

    /* 定时存储参数临时文件 */
    if (startFlag && (event->timerId() == TIMER_SAVE_FILE) && m_bscan) {
        parameterObject["PositionMax"] = m_bscan->getPositionMax();
        WriteFileAsJson(tr("%1%2").arg(DATA_BACK_FILE_NAME, ARS_FILE_EXT_PARAMETER), parameterObject);
    }
}

/*!
 * \brief MainWindow::NewFile 打开新文件从当前位置开始记录
 */
int PipeManager::NewFile()
{
    int ret = document()->Close(true);
    if (ret != 0) {
        return ret;
    }

    // 清空数据缓冲区
    document()->clear();

    // 新建文件
    QString name = QString("./%1-%2%3").arg(ModeName(m_mode), QTime::currentTime().toString(), ARS_FILE_EXT_DATA);
    ret = document()->Open(name);

    if (ret != 0) {
        QMessageBox::warning(this, QString(), tr("新建文件失败: %1").arg(ret));
        return ret;
    }

    return 0;
}

void PipeManager::SaveFile(bool rename)
{
    document()->Set("BScanOptions", convertToJson(m_bscan->viewOptions()));

    QFileInfo info(document()->GetCurrentFileName());
    if ((info.path() == "." && !rename) || rename) {
        QString fileName = FileOperateManager::saveFile();
        if (fileName.isEmpty()) {
            return;
        }

        document()->Save(fileName);
        return;
    }

    document()->Save();
}

void PipeManager::SaveAs()
{
    QString fileName = FileOperateManager::saveFile();
    if (fileName.isEmpty()) {
        return;
    }

    document()->Save(fileName);
}

void PipeManager::reloadFile()
{
    resetMaps();
    m_bscan->resetData();
    document()->Reload();
}

void PipeManager::notifyError(const QString &message)
{
    QMessageBox::critical(this, QString(), message);
}

void PipeManager::InitUI()
{
    // 软件名称
    setWindowTitle(tr("PipeDetect %1").arg("V2.0"));

    // 报警面板
    m_PanelAlarm = new PanelAlarm(this);
    m_PanelAlarm->setAllowedAreas(Qt::AllDockWidgetAreas);
    Core::ICore::mainWindow()->addDockWidget(Qt::TopDockWidgetArea, m_PanelAlarm);

    // 订单信息面板
    m_PipeParameter = new PipeParameter(this);

    // 状态栏
    m_PanelStatusFrame = new PanelStatusFrame(this);
    ui->statusBar->addWidget(m_PanelStatusFrame);

    // 工艺参数面板
    m_ParameterCopy = new ParameterCopy(this);

    // A扫界面初始化
    m_mapAScan.clear();
    for (quint32 ascanIndex = 0; ascanIndex < ASCAN_COUNT_MAX; ++ascanIndex) {
        auto *ascan = new PanelAScan(this);
        QString tempNumber = ascanIndex > 8 ? tr("%1").arg(ascanIndex + 1) : tr("0%1").arg(ascanIndex + 1);
        ascan->setObjectName(tr("PanelAScan_%1").arg(tempNumber));
        ascan->setChannelId(ascanIndex);
//        ascan->ProjectCustomizeUI("PIPE_DETECT");
        m_mapAScan.insert(ascanIndex, ascan);
        m_PanelAlarm->AddAScan(tr("A扫%1").arg(ascanIndex + 1));
//        ascan->setAllowedAreas(Qt::AllDockWidgetAreas);
        ascan->setWindowTitle(tr("A扫%1").arg(ascanIndex + 1));
        ascan->ProjectCustomizeUI("PipeDetect");
        Core::ICore::mainWindow()->addDockWidget(Qt::LeftDockWidgetArea, qobject_cast<QDockWidget *>(ascan));
    }

    // B扫界面初始化
    m_bscan = new PipePanelBScanMain(this);
    QDockWidget *BScanDockWidget = new QDockWidget(this);
    BScanDockWidget->setObjectName("PipePanelBScanMain");
    BScanDockWidget->setWindowTitle(tr("B扫描"));
    BScanDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    BScanDockWidget->setWidget(m_bscan);
    m_bscan->setObjectName(tr("PanelBScan1"));
    Core::ICore::mainWindow()->addDockWidget(Qt::RightDockWidgetArea, BScanDockWidget);

    // C扫界面初始化
    for (quint32 cscanIndex = 0; cscanIndex < CSCAN_COUNT_MAX; ++cscanIndex) {
        auto *cscan = new PanelCScan(this);
        connect(cscan, SIGNAL(ViewPosChanged(int,QPointF)), this, SLOT(OnViewPosChanged(int,QPointF)));
        cscan->setObjectName(tr("PanelCScan_%1").arg(cscanIndex + 1));
        cscan->SetLayerID(cscanIndex + 1);
        m_mapCScan.insert(cscanIndex, cscan);
        QDockWidget *CScanDockWidget = new QDockWidget(this);
        CScanDockWidget->setObjectName(tr("DockCScan_%1").arg(cscanIndex + 1));
        CScanDockWidget->setWindowTitle(tr("C扫%1").arg(cscanIndex + 1));
        CScanDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
        CScanDockWidget->setWidget(cscan);
        Core::ICore::mainWindow()->addDockWidget(Qt::LeftDockWidgetArea, CScanDockWidget);
        m_CScanDockVector.push_back(CScanDockWidget);
    }
}

void PipeManager::InitOPC()
{
#ifdef OPC
    hOPCInitEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    m_OPCComponent.StartThread();
    IUnknown* m_pIServer = nullptr;
    m_OPCComponent.CreateComponent(clsOPCServer, IID_IOPCServer, &m_pIServer);
    ::WaitForSingleObject(hOPCInitEvent,INFINITE);
    ResetEvent(hOPCInitEvent);
    m_pIServer->Release();
    ::Sleep(10);

    hOPCReadEvent = CreateEvent(nullptr, TRUE, FALSE, L"OPCReadValue");
    hOPCWriteEvent = CreateEvent(nullptr, TRUE, FALSE, L"OPCWriteValue");
#endif // OPC
}

void PipeManager::InitTestMode(const PipeGlobalConfig &pipeConfig)
{
    m_testMode = new TestModeBase(m_mode, this);
    m_testMode->setGlobalConfig(pipeConfig);

#ifdef OPC
    g_ItemCS.lock();
    switch (m_mode) {
    case Mode::FinalPipe:   g_ItemValue.SWTestMode = 1; break;
    case Mode::FinalBar:    g_ItemValue.SWTestMode = 2; break;
    case Mode::SamplePipe:  g_ItemValue.SWTestMode = 3; break;
    case Mode::SampleBar:   g_ItemValue.SWTestMode = 4; break;
    default:                g_ItemValue.SWTestMode = 0; break;
    }
    g_ItemCS.unlock();
#endif  // OPC

    m_PanelAlarm->setAlarmGroups(ReadFileAsJsonArray<PlotAlarmGroup>("AlarmOptions.json"));
    OnTestModeChanged(m_mode);
}

void PipeManager::Connect()
{
    connect(document(), &UTDocument::DataReady, m_bscan, &PipePanelBScanMain::OnSegmentUpdate);
    connect(m_bscan, &PipePanelBScanMain::alarmsUpdated, m_PanelAlarm, &PanelAlarm::setAlarms);
    connect(m_PanelAlarm, &PanelAlarm::CurChannelChanged, this, &PipeManager::OnCurChannelChanged);
    connect(m_PipeParameter, &PipeParameter::ParamChanged, this, &PipeManager::OnOrderParamChanged);
    connect(m_ParameterCopy, &ParameterCopy::ProcessInfoChanged, this, &PipeManager::OnProcessInfoChanged);

    m_PipeParameter->OpenFile();

    for (UTInstrument *instrument: UltrasonicManager::instruments()) {
        if (instrument->config().deviceType == "PipeDetect") {
            connect(instrument, &UTInstrument::instrumentStatusChanged, m_PanelStatusFrame, &PanelStatusFrame::UpdatePLXConnectionStatus);
        }
    }

    connect(m_PanelStatusFrame, &PanelStatusFrame::TestModeChanged, this, &PipeManager::OnTestModeChanged);
    connect(m_PanelStatusFrame, &PanelStatusFrame::KeepTestingChanged, this, &PipeManager::OnKeepTestingChanged);
    connect(m_PanelStatusFrame, &PanelStatusFrame::TestStartFlagArrived, this, &PipeManager::OnStartFlagArrived);
    connect(m_PanelStatusFrame, &PanelStatusFrame::ProbeBoxStatusChanged, this, &PipeManager::OnProbeBoxStatusChanged);
    connect(this, &PipeManager::ResultChanged, m_PanelStatusFrame, &PanelStatusFrame::OnResultChanged);
}

void PipeManager::CloseSetupWidgetAll()
{
    foreach (auto ascan, m_mapAScan) {
        ascan->close();
    }
}

void PipeManager::SaveBScanImage()
{
    QPixmap pixmap = m_bscan->grab(QRect(0, 0, m_bscan->width(), m_bscan->height()));
    pixmap.save("./BscanPicture.png");
}

/*!
 * \brief MainWindow::resetMaps
 * 重置喷标容器
 */
void PipeManager::resetMaps()
{
    paintMarkMap.clear();
    waitingPaintMap.clear();
}

bool PipeManager::createParameterBackFile()
{
    parameterObject = QJsonObject();
    parameterObject["PositionMax"] = 0;
    parameterObject["DataFileNeedRename"] = true;
    parameterObject["TestMode"] = EnumToKey(m_mode);
    parameterObject["OrderParameter"] = m_PipeParameter->ToJson();

    QString path = QString("%1%2").arg(DATA_BACK_FILE_NAME, ARS_FILE_EXT_PARAMETER);
    return WriteFileAsJson(path, parameterObject);
}

void PipeManager::removeParameterBackFile()
{
    QFile file(QString("%1%2").arg(DATA_BACK_FILE_NAME, ARS_FILE_EXT_DATA));
    file.remove();

    file.setFileName(QString("%1%2").arg(DATA_BACK_FILE_NAME, ARS_FILE_EXT_PARAMETER));
    file.remove();
}

/*!
 * \brief MainWindow::SetPipeParm2OPC
 * \param info：材料信息
 * 通过OPC通信向电气写入所需的材料信息
 */
void PipeManager::OnOrderParamChanged(const OrderInfo &info)
{
    paramInfo.orderInfo = info;
    WriteOPCData("PipeLength", info.materialLength);
    WriteOPCData("PipeDiameter", info.materialDiameter);
    ResetImageOptions();
}

void PipeManager::OnProcessInfoChanged(const PipeManager::ProcessInfo &info)
{
    paramInfo.processInfo = info;
}

/*!
 * \brief MainWindow::OnTestModeChanged
 * \param mode：当前检测模式
 * 根据OPC传来的最新模式进行模式切换
 */
void PipeManager::OnTestModeChanged(Mode mode)
{
    int probeBoxStatus = m_testMode->DataManager()->GetProbeBoxStatus();
    disconnect(m_testMode->DataManager(), &DataManagerBase::SegmentArrived, m_bscan, &PipePanelBScanMain::OnSegmentUpdate);
    disconnect(m_PanelStatusFrame, &PanelStatusFrame::TestStartFlagArrived, m_testMode->DataManager(), &DataManagerBase::OnStartFlagArrived);

    m_testMode->SetMode(mode);
    m_testMode->DataManager()->SetProbeBoxStatus(probeBoxStatus);
    connect(m_testMode->DataManager(), &DataManagerBase::SegmentArrived, m_bscan, &PipePanelBScanMain::OnSegmentUpdate);
    connect(m_PanelStatusFrame, &PanelStatusFrame::TestStartFlagArrived, m_testMode->DataManager(), &DataManagerBase::OnStartFlagArrived);

    switch (mode) {
    case Mode::SamplePipe:
    case Mode::SampleBar:   SetCoderMark(1);    break;
    default:                SetCoderMark(0);    break;
    }

    // 在从其他模式变化成手动模式的时候，不进行窗口个数等options的重置
    if (mode != m_mode && mode == Mode::InvalidMode) {
        m_mode = mode;
        return;
    }

    m_mode = mode;

    ResetImageOptions();
    ResetAScanNum();
    ResetCScanNum();
}

void PipeManager::OnKeepTestingChanged(bool keepTestingFlag)
{
    if (keepTestingFlag) {
        if (keepTestingFileExist()) {
            parameterObject = ReadFileAsJson(QString("%1%2").arg(DATA_BACK_FILE_NAME, ARS_FILE_EXT_PARAMETER)).toObject();
            Mode mode = KeyToEnum(parameterObject["TestMode"].toString(), Mode::InvalidMode);
            if (mode != m_mode) {
                QMessageBox::information(this,
                    tr("断电续扫"),
                    tr("上次的检测模式 “%1” 与当前检测模式 “%2” 不匹配，请选择正确的模式后再进行断电续扫").arg(ModeName(mode), ModeName(m_mode))
                );
                keepTestingStatus = false;
                return;
            }

            QMessageBox::StandardButton btKeepTesting = dialogYesOrNo(tr("断电续扫"), tr("电气请求断电续扫，是否继续上次扫描？"));
            if (btKeepTesting == QMessageBox::Yes) {
                QMessageBox::StandardButton btOrder = dialogYesOrNo(tr("参数加载"), tr("是否加载参数文件中的订单参数？"));
                if (btOrder == QMessageBox::Yes) {
                    if (m_PipeParameter) {
                        m_PipeParameter->FromJson(parameterObject["OrderParameter"]);
                    }
                }

                /* 发送给电气必要的参数 */
                WriteOPCData("PipeLength", paramInfo.orderInfo.materialLength);
                WriteOPCData("PipeDiameter", paramInfo.orderInfo.materialDiameter);

                /* 获取最大位置 */
                qreal posMax = parameterObject["PositionMax"].toDouble();
                if (posMax < keepTesingLengthMin) {
                    QMessageBox::information(this,
                                             tr("断电续扫"),
                                             tr("上一次扫描长度小于%1mm，不满足续扫设定条件，请从头开始扫查").arg(keepTesingLengthMin));
                } else {
                    WriteOPCData("KTPosition", quint32(posMax - keepTestingOffset));
                    on_actionInstrumentReady_triggered();
                    keepTestingStatus = true;
                    return;
                }
            }
        } else {
            QMessageBox::information(this, tr("断电续扫"), tr("断电续扫所需的数据恢复文件不存在,只能进行正常扫描"));
        }
    }

    keepTestingStatus = false;
}

void PipeManager::ResetAScanNum()
{
    int groupMask = m_testMode->DataManager()->GetProbeBoxStatus();
    if (m_mode == Mode::InvalidMode) {
        groupMask = 0x1F;
    }

    MotionAxisList enabledChannels = m_testMode->globalConfig().enabledChannels(groupMask);
    m_PanelAlarm->ComboBoxClear();

    PanelAScan *first = nullptr;
    for (int index = 0; index < enabledChannels.count(); ++index) {
        PanelAScan *panel = m_mapAScan.value(index);
        if (panel) {
            panel->setVisible(enabledChannels.hasAxis(index));
            panel->setChannelId(index);
            m_PanelAlarm->AddAScan(tr("A扫 %1").arg(index + 1));

            if (first == nullptr && panel->isVisible()) {
                first = panel;
            }
            if (index >= 1) {
                Core::ICore::mainWindow()->tabifyDockWidget(qobject_cast<QDockWidget *>(m_mapAScan[index - 1]), qobject_cast<QDockWidget *>(m_mapAScan[index]));
            }
        }
    }
    if (first) {
        first->raise();
    }
}

void PipeManager::ResetCScanNum()
{
    int groupMask = m_testMode->DataManager()->GetProbeBoxStatus();
    if (m_mode == Mode::InvalidMode) {
        groupMask = 0x1F;
    }

    int count = m_CScanDockVector.count();
    MotionAxisList enabledChannels = m_testMode->globalConfig().enabledChannels(groupMask);
    QDockWidget *first = nullptr;
    for (int i = 0; i < count; ++i) {
        QDockWidget *panel = m_CScanDockVector.value(i);
        if (panel) {
            panel->setVisible(enabledChannels.hasAxis(i));
            if (first == nullptr) {
                first = panel;
            }
            if (i >= 1) {
                Core::ICore::mainWindow()->tabifyDockWidget(m_CScanDockVector[i - 1], m_CScanDockVector[i]);
            }
        }
    }
    if (first) {
        first->raise();
    }
}

void PipeManager::SetReturnInfo2OPC(quint32 boxIndex, quint32 posX)
{
    WriteOPCData("ReturnBoxIndex", boxIndex);
    WriteOPCData("ReturnPosition", posX);
}

void PipeManager::OnStartFlagArrived(quint16 flag)
{
    if (flag) {
        if (m_PipeParameter) {
            m_PipeParameter->saveToDocument();
        }

        document()->LoadSetups();
        SetResult(TestResultType::NoJudge);
//        DataPath::instance()->instrumentsCtrl()->Enable();    // TODO: Use internal state to handle data -> document

        NewFile();
        resetMaps();
        ResetImageOptions();

        if (m_bscan) {
            m_bscan->resetData();
            document()->Set("BScanOptions", convertToJson(m_bscan->viewOptions()));
        }

        if (keepTestingStatus) {
            /* 确保在恢复续扫数据的时候，startFlag开关已经开启 */
            DataManagerBase *dataManager = m_testMode->DataManager();
            if (dataManager) {
                dataManager->OnStartFlagArrived(flag);
            }

            UTStreamFile backupFile(tr("%1%2").arg(DATA_BACK_FILE_NAME, ARS_FILE_EXT_DATA));
            if (!backupFile.open(QIODevice::ReadOnly)) {
                QMessageBox::warning(this, tr("打开文件"), tr("打开数据文件失败，无法将之前的数据恢复到此次扫查中"));
                keepTestingStatus = false;
                return;
            }

            static const int maximum = 10000;
            QProgressDialog progress(this);
            progress.setLabelText(tr("正在恢复上次扫查数据..."));
            progress.setWindowModality(Qt::WindowModal);
            progress.setMinimumDuration(0);
            progress.setMinimum(0);
            progress.setMaximum(maximum);

            while (!backupFile.atEnd() && !progress.wasCanceled()) {
                UTDataSegment segment = backupFile.ReadSegment(1024);
                progress.setValue(double(backupFile.pos()) / double(backupFile.size()) * maximum);
                m_testMode->DataManager()->dataArrived(segment);
            }

            progress.hide();
        } else {
            createParameterBackFile();
        }
    } else {
//        DataPath::instance()->instrumentsCtrl()->Disable();   // TODO: Use internal state to handle data -> document
        SaveFile();
        SaveBScanImage();

        /* 判伤 */
        on_actionReJudge_triggered();

        /* 避免因续扫已经开始时，发现设备或工艺有问题需要临时结束而误删除临时文件 */
        if (keepTestingStatus) {
            QMessageBox::StandardButton btRemove = dialogYesOrNo(tr("断电续扫"), tr("续扫结束，是否删除临时文件"));
            if (btRemove == QMessageBox::No) {
                return;
            }
        }

        removeParameterBackFile();
    }

    startFlag = flag; // 放在此处赋值，用来定时存储参数临时文件所用，如果赋值过早，会保存出来一个空的临时文件
}

void PipeManager::OnProbeBoxStatusChanged(quint16 status)
{
    if (status != m_testMode->DataManager()->GetProbeBoxStatus() && status != 0) {
        m_testMode->DataManager()->SetProbeBoxStatus(status);
        ResetImageOptions();
        ResetAScanNum();
        ResetCScanNum();
    } else if (status == 0) {
        m_testMode->DataManager()->SetProbeBoxStatus(status);
    }
}

void PipeManager::SetCoderMark(quint16 mark)
{
    for (UTInstrument *instrument: UltrasonicManager::instruments()) {
        instrument->sendRawMessage(IInstrumentDriver::MessageEncoder, mark);
    }
}

void PipeManager::SetResult(const TestResultType &result)
{
    QString strResult = tr("未评判");
    switch (result) {
    case TestResultType::Accept:   strResult = tr("合格品");    break;
    case TestResultType::Reject:   strResult = tr("缺陷品");    break;
    case TestResultType::Decouple: strResult = tr("耦合不良品"); break;
    default:                                                   break;
    }

    QJsonArray array = platform()->Get("ReportKeys").toArray();
    int count = array.count();

    for (int i = 0; i < count; ++i) {
        auto obj = array[i].toObject();
        if (obj["Key"] == "Result") {
            obj["Value"] = strResult;
            array[i] = obj;
        }
    }

    platform()->Set("ReportKeys", array);
    emit ResultChanged(result);
}

void PipeManager::OnFoundFeature(qreal pos)
{
    if (!paramInfo.processInfo.isMark) {
        return;
    }

    auto posCur = int(pos);
    if (!paintMarkMap.contains(posCur)) {
        bool foundLast = false;
        int posPrev = -paramInfo.processInfo.paintInterval;         // 小于当前位置的最后一点
        auto posAfter = int(paramInfo.orderInfo.materialLength);     // 大于当前位置的第一个点

        for (QMap<int, bool>::iterator it = paintMarkMap.begin(); it != paintMarkMap.end(); ++it) {
            int posTemp = it.key();
            bool waitingPaint = it.value();
            if (waitingPaint) {
                paintMarkMap[posTemp] = false;
                waitingPaintMap.insert(posTemp, waitingPaint);
            }

            /* 获取前一个点 */
            if (posTemp < posCur) {
                posPrev = posTemp;
            }

            /* 获取后一个点 */
            if ((posTemp > posCur) && (!foundLast)) {
                foundLast = true;
                posAfter = posCur;
            }
        }

        if ((posCur - posPrev >= paramInfo.processInfo.paintInterval) &&
            (posAfter - posCur >= paramInfo.processInfo.paintInterval)) {
            paintMarkMap.insert(posCur, false);
            waitingPaintMap.insert(posCur, true);
        }
    }
}

void PipeManager::OnViewPosChanged(const int &id, const QPointF &pt)
{
    bool returnMark = false;

#ifdef OPC
    g_ItemCS.lock();
    returnMark = g_ItemValue.ReturnMark;
    g_ItemCS.unlock();
#endif

    if (returnMark) {
        SetReturnInfo2OPC(id, pt.x());
    }
}

void PipeManager::on_actionConnect_triggered()
{
    for (UTInstrument *instrument: UltrasonicManager::instruments()) {
        instrument->Connect();
    }
}

void PipeManager::on_actionDisconnect_triggered()
{
    for (UTInstrument *instrument: UltrasonicManager::instruments()) {
        instrument->Disconnect();
    }
}

/*!
 * \brief MainWindow::on_actionGlobalOptions_triggered
 * 全局配置窗口
 */
void PipeManager::on_actionGlobalOptions_triggered()
{
    Core::ICore::mainWindow()->showOptionsDialog({});
}

void PipeManager::on_actionOtherParameter_triggered()
{
    if (m_PipeParameter == nullptr) {
        return;
    }

    m_PipeParameter->show();
}

void PipeManager::on_actionParameterCopy_triggered()
{
    if (m_ParameterCopy == nullptr) {
        m_ParameterCopy = new ParameterCopy(this);
    }

    m_ParameterCopy->show();
}

void PipeManager::on_actionViewManager_triggered()
{
    m_testMode->UpdateCheckBoxs();
    m_testMode->show();
}

void PipeManager::on_actionOpen_triggered()
{
    OpenFile();
    SaveBScanImage();
    on_actionReJudge_triggered();
}

void PipeManager::on_actionSave_triggered()
{
    SaveFile();
}

void PipeManager::on_actionClose_triggered()
{
    CloseFile();
}

/*!
 * \brief MainWindow::on_actionReloadFile_triggered
 * 重新加载文件
 */
void PipeManager::on_actionReloadFile_triggered()
{
    reloadFile();
}

/*!
 * \brief MainWindow::on_actionReJudge_triggered
 * 发出探伤结束状态-重新判伤
 */
void PipeManager::on_actionReJudge_triggered()
{
    if (m_PanelAlarm) {
        SetResult(m_PanelAlarm->JudgeResult());
    }
}

void PipeManager::on_actionAbout_triggered()
{
    auto *about = findChild<DialogAbout *>();
    if (about) {
        about->exec();
    }
}

/*!
 * \brief MainWindow::on_actionTest_triggered
 * 检测模式模拟切换
 */
void PipeManager::on_actionTest_triggered()
{
#ifdef OPC
    g_ItemCS.lock();
    if (g_ItemValue.SWTestMode >= 4) {
        g_ItemValue.SWTestMode = 0;
    } else {
        g_ItemValue.SWTestMode++;
    }
    g_ItemCS.unlock();
#endif
}

/*!
 * \brief MainWindow::on_actionKeepTesting_triggered
 * 断电续扫标志模拟切换
 */
void PipeManager::on_actionKeepTesting_triggered()
{
#ifdef OPC
    g_ItemCS.lock();
    g_ItemValue.KeepTesting = !g_ItemValue.KeepTesting;
    g_ItemCS.unlock();
#endif
}

/*!
 * \brief MainWindow::on_actionReturnMark_triggered
 * 缺陷回位标志模拟切换
 */
void PipeManager::on_actionReturnMark_triggered()
{
#ifdef OPC
    g_ItemCS.lock();
    g_ItemValue.ReturnMark = !g_ItemValue.ReturnMark;
    g_ItemCS.unlock();
#endif
}

/*!
 * \brief MainWindow::on_actionTestStart_triggered
 * 开始探伤-测试按钮
 */
void PipeManager::on_actionTestStart_triggered()
{
//    DataPath::instance()->instrumentsCtrl()->Enable();    // TODO: Use internal state to handle data -> document

#ifdef OPC
    g_ItemCS.lock();
    g_ItemValue.TestStartFlag = true;
    g_ItemCS.unlock();
#endif
}

/*!
 * \brief MainWindow::on_actionTestEnd_triggered
 * 结束探伤-测试按钮
 */
void PipeManager::on_actionTestEnd_triggered()
{
    if (m_mapCScan.value(0)) {
//        DataPath::instance()->instrumentsCtrl()->Disable();   // TODO: Use internal state to handle data -> document
    }

#ifdef OPC
    g_ItemCS.lock();
    g_ItemValue.TestStartFlag = false;
    g_ItemCS.unlock();
#endif
}

/*!
 * \brief MainWindow::on_actionProbeBoxStatus_triggered
 * 探头盒序列模拟切换
 */
void PipeManager::on_actionProbeBoxStatus_triggered()
{
#ifdef OPC
    g_ItemCS.lock();
    if (g_ItemValue.ProbeBoxStatus == 0x7) {
        g_ItemValue.ProbeBoxStatus = 0;
    } else {
        g_ItemValue.ProbeBoxStatus = 0x7;
    }
    g_ItemCS.unlock();
#endif
}

/*!
 * \brief MainWindow::on_actionInstrumentReady_triggered
 * 仪器就绪响应
 */
void PipeManager::on_actionInstrumentReady_triggered()
{
#ifdef OPC
    g_ItemCS.lock();
    g_ItemValue.IsInstrumentReady = true;
    g_ItemCS.unlock();
#endif

    WriteOPCData("WSState", 1);
}

/*!
 * \brief MainWindow::on_actionLock_triggered
 * 参数锁定
 */
void PipeManager::on_actionLock_triggered()
{
#ifdef OPC
    g_ItemCS.lock();
    g_ItemValue.IsLocked = true;
    g_ItemCS.unlock();
#endif
}

/*!
 * \brief MainWindow::on_actionUnlock_triggered
 * 参数解锁
 */
void PipeManager::on_actionUnlock_triggered()
{
#ifdef OPC
    QMessageBox::StandardButton button = QMessageBox::question(nullptr, tr("参数解锁"), tr("是否确认解锁？"), QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::Yes) {
        g_ItemCS.lock();
        g_ItemValue.IsLocked = false;
        g_ItemCS.unlock();
    }
#endif
}

void PipeManager::on_actionReturnPos_triggered()
{
    bool returnMark = false;
#ifdef OPC
    g_ItemCS.lock();
    returnMark = g_ItemValue.ReturnMark;
    g_ItemCS.unlock();
#endif

    if (returnMark) {
        SetReturnInfo2OPC(1, 1000);
    }
}

void PipeManager::on_actionKeepTesting_2_triggered()
{
    bool keepTesting = false;
#ifdef OPC
    g_ItemCS.lock();
    keepTesting = g_ItemValue.KeepTesting;
    g_ItemCS.unlock();
#endif

    if (keepTesting) {
        WriteOPCData("KTPosition", 1500);
    }
}

void PipeManager::on_actionPaintMark_triggered()
{
    for (int i = 1; i <= 5; ++i) {
        WriteOPCData("PaintPosition", i * 100);
        QThread::msleep(20);
    }
}
