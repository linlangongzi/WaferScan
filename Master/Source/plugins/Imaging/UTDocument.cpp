#include "UTDocument.h"
#include <Acquisition/ScanPlanModel.h>
#include <Acquisition/ScanTask.h>
#include <Core/CoreConstants.h>
#include <Core/ICore.h>
#include <Imaging/DefectItemModel.h>
#include <Motion/Axis.h>
#include <QFileInfo>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QProgressDialog>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/Util.h>

static Q_LOGGING_CATEGORY(debug, "core.doc");

static UTDocument *m_instance = nullptr;
using namespace Core::Constants;

UTDocument::UTDocument(QObject *parent) :
    QObject(parent),
    dataFile(nullptr)
{
    m_instance = this;

    m_task = new ScanTask(nullptr, this);
    CheckLastTestIdle();
    layerMap.clear();
    InitDoc();
}

UTDocument::~UTDocument()
{
    if (dataFile.isOpen()) {
        dataFile.close();
    }
    m_instance = nullptr;
}

/*! 初始化文件 */
void UTDocument::InitDoc()
{
    // 初始化扫描计划与工艺集合
    setupManager = UltrasonicManager::setups();
//    setupManager->FromJson(QJsonArray());
    // 打开未命名文件
    dataFile.setFileName( GetCurrentFileName() );
    dataFilePath = QString();
    if ( !dataFile.open( QIODevice::ReadWrite ) ) {
        qCWarning(debug) << "InitDoc Open Undefined file failed";
        return;
    }
    connect(&dataFile, &UTStreamFile::DataReady, this, &UTDocument::OnDataReady);
}

/*!
 * \brief UTDocument::GetParameterFileName 根据数据文件名称获取参数文件名称
 * \param dataFileName          数据文件名称
 * \return                      参数文件名称
 */
QString UTDocument::GetParameterFileName(const QString &dataFileName)
{
    int pos = dataFileName.lastIndexOf(ARS_FILE_EXT_DATA);
    if (pos < 0) {                                                              // 没找到
        return dataFileName + ARS_FILE_EXT_PARAMETER;
    }
    QString parameterFileName(dataFileName);
    parameterFileName.replace(pos, QString(ARS_FILE_EXT_DATA).length(), ARS_FILE_EXT_PARAMETER);
    return parameterFileName;
}

/*! 返回当前文件名称 */
QString UTDocument::GetCurrentFileName()
{
    if ( dataFile.isOpen() ) {
        return dataFile.fileName();
    }
    return tr("新建文件%1").arg(ARS_FILE_EXT_DATA);
}

/*! 获取当前文件路径 */
QString UTDocument::GetCurrentFilePath()
{
    return dataFilePath;
}

/*!
 * \brief UTDocument::Open 打开文件
 * \param name             文件名称
 * \return                 0：成功
 *                         1：关闭旧文件失败
 *                         2：新文件打开失败
 *                         3：更新文件缓冲区失败
 */
int UTDocument::Open(const QString &name)
{
    saveFileEnable = false;
    if ( !Close() ) {
        return 1;
    }
    // 打开新文件需要清空数据缓冲区以及更新参数缓冲区
    QJsonValue parameter = ReadFileAsJson(GetParameterFileName(name));
    loadJson(parameter);

    // 打开新数据文件
    dataFile.setFileName(name);
    QFileInfo info(name);
    dataFilePath = info.path();
    if (!dataFile.open(QIODevice::ReadWrite) && !dataFile.open(QIODevice::ReadOnly)) {
        return 2;
    }

    notifyFileOpened();
    return 0;
}

int UTDocument::cutFiles(const QStringList &fileNameList)
{
    static int segmentLength = 10240;
    static const int maximum = 10000;

    QProgressDialog progress(Core::ICore::mainWindow());
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setMinimum(0);
    progress.setMaximum(maximum);
    for (const auto &fileName: fileNameList) {
        progress.setLabelText(tr("正在精简文件 %1").arg(fileName));
        UTStreamFile streamFile;
        UTStreamFile tempFile;
        streamFile.setFileName(fileName);
        QFileInfo tempInfo(fileName);
        QString tempFileName = tempInfo.absolutePath() +"\\~" + tempInfo.completeBaseName() + tr(".ard");
        tempFile.setFileName(tempFileName);
        if (streamFile.open(QIODevice::ReadWrite) && tempFile.open(QIODevice::ReadWrite)) {
            tempFile.SetDataEnable(true);
            streamFile.SetDataEnable(true);
            streamFile.seek(0);
            UTDataSegment segment;
            while (streamFile.hasMoreFrame() && !progress.wasCanceled()) {
                progress.setValue(qreal(streamFile.pos()) / qreal(streamFile.size()) * maximum);
                segment = streamFile.ReadSegment(segmentLength);
                for (auto &data: segment) {
                    data.AData.clear();
                }
                tempFile.WriteSegment(segment);
            }
            streamFile.close();
            streamFile.remove();
            tempFile.close();
            tempFile.rename(fileName);
        }
    }
    progress.hide();
    return 0;
}

/*!
 * \brief UTDocument::NewFile   新建文件
 * \param name                  文件名
 * \return                      0:成功
 *                              1：关闭当前文件失败
 *                              2：新建参数文件失败
 *                              3：新建数据文件失败
 */
int UTDocument::NewFile(const QString &name)
{
    // 关闭当前文件
    if (!Close()) {
        return 1;
    }
    // 打开新参数文件
    loadJson(QJsonValue());

    // 打开新数据文件清空文件内容
    dataFile.setFileName(name);
    QFileInfo info(name);
    dataFilePath = info.path();
    if (!dataFile.open(QIODevice::ReadWrite) && !dataFile.open(QIODevice::ReadOnly)) {
        return 3;
    }
    dataFile.resize(0);
    clear();

    notifyFileOpened();
    return 0;
}

/*!
 * \brief UTDocument::Close 关闭文件
 * \return                  true: 成功
 *                          false:失败
 */
bool UTDocument::Close(bool ignore)
{
    Q_UNUSED(ignore);
    Save();
    clear();
    if (dataFile.isOpen()) {
        dataFile.close();
        dataFilePath = QString();
    }
    return true;
}

/*!
 * \brief UTDocument::Save 保存文件
 * \return  0：成功
 *          1：缓冲区未打开
 *          2：数据文件无路径，需要调用另存为
 *          3：参数文件保存失败
 */
int UTDocument::Save(QString name)
{
    if (!saveFileEnable) {
        return 0;
    }
    if (name.isEmpty()) {
        if (dataFilePath.isEmpty()) {
            return 2;
        }
        name = dataFile.fileName();
    }

    QString errorString;
    if (!WriteFileAsJson(GetParameterFileName(name), saveJson(), true, &errorString)) {
        return 3;
    }
    saveFileEnable = false;
    return 0;
}

/*!
 * \brief UTDocument::Clear 清空数据缓存
 * \return  0: 成功
 *          1: 修改尺寸失败
 */
void UTDocument::clear()
{
    loadJson(QJsonObject());
}

int UTDocument::Reload()
{
    foreach (UTLayer *layer, layerMap) {
        layer->Reset();
    }
    RequestDataReadAll();
    return 0;
}

/*! 请求读取文件中的全部数据 */
void UTDocument::RequestDataReadAll()
{
    static int segmentLength = 10240;
    if (dataFile.isOpen()) {                                                   // 读完文件再读缓冲区
        dataFile.RequestReadAll(segmentLength);
    }
    dataFile.SetDataEnable(true);
}

void UTDocument::OnDataArrived(const UTDataSegment &segment)
{
    if ( dataFile.isOpen() ) {
        dataFile.OnDataArrived(segment);
    }
    for (UTLayer *layer: layerMap) {
        layer->OnDataArrived(segment);
    }
}

void UTDocument::SetDataEnable(bool enable)
{
    if ( dataFile.isOpen() ) {
        dataFile.SetDataEnable(enable);
    }
}

DefectItemModel *UTDocument::GetDefectModel(int key)
{
    if ( layerMap.contains(key) ) {
        return layerMap.value(key)->GetModel();
    }
    return nullptr;
}

void UTDocument::Set(const QString &key, const QJsonValue &value)
{
    saveFileEnable = true;
    data[key] = value;
}

void UTDocument::setSaveFile(bool saved)
{
    saveFileEnable = saved;
}

void UTDocument::setOrigin(const QVector3D &origin)
{
    if (m_origin == origin) {
        return;
    }

    m_origin = origin;
    emit originChanged(m_origin);
}

/*! 序列化layer */
void UTDocument::InitializeLayers()
{
    for (int index = 0; index < NUM_LAYERS; ++index) {
        auto *layer = new UTLayer(this);
        int id = NewLayerID();
        layer->SetID(id);
        layerMap.insert(id, layer);
    }
    loadJson(GetParameterFileName(dataFile.fileName()));
}

/*! 通过layerID获取layer */
UTLayer *UTDocument::GetLayerByID(int layerID)
{
    return layerMap.value(layerID);
}

/*! 分配一个新的layerID */
int UTDocument::NewLayerID()
{
    int id = 1;
    while (true) {
        if ( !layerMap.contains(id) ) {
            return id;
        }
        id++;
    }
    Q_UNREACHABLE();
}

/*! 获得LayerMap */
QMap<int, UTLayer *> UTDocument::GetLayerMap()
{
    return layerMap;
}

void UTDocument::OnDataReady(const UTDataSegment &segment)
{
    foreach (UTLayer *layer, layerMap) {
        layer->OnDataArrived(segment);
    }
    emit DataReady(segment);
}

/*! 加载扫描计划 */
void UTDocument::LoadScan(ScanPlan *plan)
{
    QJsonObject json;
    if (plan) {
        json = plan->toJson();
    }
    m_task->scanPlan()->fromJson(json);
    LoadSetups();
}

void UTDocument::LoadSetups()
{
    // 获取扫描开始时的通道开关状态
    setupManager = UltrasonicManager::setups();
    saveFileEnable = true;
}

void UTDocument::notifyFileOpened()
{
    Core::ICore::mainWindow()->setWindowFilePath(GetCurrentFileName());

    emit FileOpened();
}

void UTDocument::loadJson(const QJsonValue &value)
{
    QJsonObject json = value.toObject();
    if (json.isEmpty()) {
        for (auto it = layerMap.begin(); it != layerMap.end(); ++it) {
            UTLayer *layer = it.value();
            if (!layer || !layer->GetModel()) {
                continue;
            }
            layer->GetModel()->clear();
        }
    }
    m_task->scanPlan()->fromJson(json["ScanPlan"].toObject());
    m_task->setData(convertFromJson<ScanTaskData>(json["ScanTask"]));
    for (const QJsonValue &value: json["Layers"].toArray()) {
        if (!value.isObject()) {
            continue;
        }
        QJsonObject obj = value.toObject();
        int id = obj["ID"].toString().toInt();
        if (layerMap.contains(id)) {
            UTLayer *layer = layerMap[id];
            ImageOptions options = ImageOptions::fromJson(json["Transfrom"]);
            options.setAxisX(json["XAxis"].toInt(1) - 1);                       /* 旧版本的数据文件中，没有按Layer保存ImageOptions，若Layer中不存在，尝试用全局配置覆盖它 */
            options.setAxisY(json["YAxis"].toInt(2) - 1);
            layer->FromJson(obj, options);
        }
    }
    if (json.contains("ChannelSetupMap")) {
        QJsonArray setups = json["Setups"].toArray();
        QMap<int, UTSetup> setupMap;
        foreach (const QJsonValue &setup, setups) {
            QJsonObject obj = setup.toObject();
            setupMap.insert(obj["ID"].toString().toInt(), convertFromJson<UTSetup>(obj));
        }
        setupManager.clear();
        auto channelSetupMap = convertFromJson<QMap<int, int>>(json["ChannelSetupMap"]);
        for (auto it = channelSetupMap.begin(); it != channelSetupMap.end(); ++it) {
            if (setupMap.contains(it.value())) {
                setupManager.insert(it.key(), setupMap.value(it.value()));
            }
        }
        json.remove("ChannelSetupMap");
    } else {
        setupManager = convertFromJson<QMap<int, UTSetup>>(json["Setups"]);
    }
    m_origin = convertFromJson<QVector3D>(json["Origin"]);
    data = json;
}

QJsonValue UTDocument::saveJson() const
{
    QJsonObject json = data;
    json["Setups"] = convertToJson(setupManager);
    json["ScanPlan"] = m_task->scanPlan()->toJson();
    json["ScanTask"] = convertToJson(m_task->data());
    json["Origin"] = convertToJson(m_origin);
    QJsonArray array;
    for (UTLayer *layer: layerMap) {
        array.append(layer->ToJson());
    }
    json["Layers"] = array;
    return json;
}

/*!
 * \brief UTDocument::CheckLastTestIdle
 * 查询最后一个探伤是否正常结束,如果否,则进行断电续扫的准备工作
 */
void UTDocument::CheckLastTestIdle()
{
    QString fileBack = QString("%1%2").arg(DATA_BACK_FILE_NAME, ARS_FILE_EXT_PARAMETER);
    QJsonValue value = ReadFileAsJson(fileBack).toObject();
    if (value == QJsonValue()) {
        return;
    }

    QJsonObject obj = value.toObject();
    if (!obj["DataFileNeedRename"].toBool()) {
        return;
    }

    QString oldName = BUFFERNAME_DATA;
    QString newName = QString("%1%2").arg(DATA_BACK_FILE_NAME, ARS_FILE_EXT_DATA);

    QFile file(newName);
    file.remove();
    file.setFileName(oldName);
    if (file.rename(newName)) {
        obj["DataFileNeedRename"] = false;
    }

    WriteFileAsJson(fileBack, QJsonValue(obj));
}

/*! 根据通道号获取记录的工艺(关联检测会自动连接到该仪器的0通道) */
UTSetup UTDocument::setupForChannel(int channelGlobalIndex)
{
    UTSetup setup = setupManager.value(channelGlobalIndex);
    if (setup.alterSource()) {  // 关联检测
        UTChannel *channel = UltrasonicManager::channel(channelGlobalIndex);
        if (channel && channel->instrumentIndex() != 0) {
            UTChannel *firstChannel = channel->instrument()->channel(0);
            if (firstChannel) {
                int firstChannelIndex = firstChannel->channelId();
                UTSetup alterSetup = document()->setupForChannel(firstChannelIndex);
                setup = alterSetup;
            }
        }
    }
    return setup;
}

ScanPlan *UTDocument::scanPlan()
{
    return m_task->scanPlan();
}

ScanTask *UTDocument::scanTask()
{
    return m_task;
}

QJsonValue UTDocument::Get(const QString &key) const
{
    return data.value(key);
}

QVector3D UTDocument::origin() const
{
    return m_origin;
}

void UTDocument::setScanTask(ScanTask *task)
{
    m_task->setData(task->data());
    m_task->setScanPlan(task->scanPlan());
    saveFileEnable = true;
}

UTDocument *document()
{
    return m_instance;
}
