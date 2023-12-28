#include "PanelImport.h"
#include "DialogImageRecord.h"
#include "ImageRecorder.h"
#include "ImagingGlobal.h"
#include "ui_PanelImport.h"

#include <Core/DocumentManager.h>
#include <Core/ICore.h>
#include <QFileDialog>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QProgressDialog>

static Q_LOGGING_CATEGORY(debug, "image.import");

/*!
 * \brief fromJson
 * \param value
 * \return 将一些旧版本.arp文件中的Setups转换为QHash<setupId, newSetup>
 */
QHash<int, UTSetup> fromJson(const QJsonValue &value)
{
    QHash<int, UTSetup> setups;
    QJsonArray array = value.toArray();
    if (array.isEmpty()) {
        return setups;
    }

    for (QJsonValue value: array) {
        QJsonObject obj = value.toObject();

        UTSetup setup;
        setup.setPrf(obj["PRF"].toInt());
        setup.setName(obj["Name"].toString());
        setup.setZero(obj["Zero"].toDouble());
        setup.setDelay(obj["Delay"].toDouble());
        setup.setRange(obj["Range"].toDouble());
        setup.setSuppress(obj["Suppress"].toDouble());
        setup.setGain(obj["Gain"].toDouble());
        setup.setGainRef(obj["GainRef"].toDouble());
        setup.setAmplify1(obj["Amplify1"].toInt(1023));
        setup.setAmplify2(obj["Amplify2"].toInt(1023));
        setup.setPulseWidth(obj["PulseWidth"].toInt());
        setup.setPulserEnable(obj["PulserEnable"].toBool());
        setup.setProbeBand(obj["ProbeBand"].toVariant().toInt());
        setup.setProbeMultiplex(obj["ProbeMultiplex"].toInt());
        setup.setDac(InterpolateTable::fromJson(obj["TCG"]));
        setup.setDacMode(obj["TcgEnable"].toBool() ? UTSetup::DacMode::DacTcg: UTSetup::DacMode::DacDisabled);
        setup.setSampleAverage(obj["SampleAverage"].toVariant().toInt());
        setup.setTransmitVoltage(obj["TransmitVoltage"].toVariant().toInt());
        setup.setRectifierMode(UTSetup::RectifierMode(obj["RectifierMode"].toVariant().toInt()));
        setup.setImpedance(obj["Impedance"].toString() == "HIGH" ? UTSetup::Impedance::ImpedanceHigh : UTSetup::Impedance::ImpedanceLow);
        setup.setTransmitMode(obj["TransmitMode"].toString() == "SELF" ? UTSetup::TransmitMode::PulseEcho : UTSetup::TransmitMode::TransmitThrough);
        setup.setTransmitEnergy(obj["TransmitEnergy"].toString() == "HIGH" ? UTSetup::TransmitEnergy::EnergyHigh : UTSetup::TransmitEnergy::EnergyLow);
        setup.setTimebaseUnit(obj["TimebaseUnit"].toString() == "DISTANCE" ? UTSetupTimebase::TimebaseUnit::Distance : UTSetupTimebase::TimebaseUnit::Time);

        for (int index = 1; index <= 4; ++index) {
            UTSetupGate gate;
            gate.setColor(QColor(obj[QString("Gate%1Color").arg(index)].toString()));
            gate.setEnabled(obj[QString("Gate%1Enable").arg(index)].toBool());
            gate.setStart(obj[QString("Gate%1Start").arg(index)].toDouble());
            gate.setWidth(obj[QString("Gate%1Width").arg(index)].toDouble());
            gate.setThreshold(obj[QString("Gate%1Amp").arg(index)].toDouble());

            if (index > 1) {
                gate.setSyncIndex(0);
            } else {
                gate.setTimeMode(UTSetupGate::Trigger);
            }

            setup.appendGate(gate);
        }
        setups[obj["ID"].toVariant().toInt()] = setup;
    }
    return setups;
}

PanelImport::PanelImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PanelImport)
{
    ui->setupUi(this);
}

PanelImport::~PanelImport()
{
    delete ui;
}

void PanelImport::on_load_clicked()
{
    // 打开旧数据文件
    QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), "*.ard");
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, QString(), tr("打开文件 %1 失败: %2").arg(fileName).arg(file.errorString()));
        return;
    }

    // 初始化进度条
    static const int maximum = 10000;
    QProgressDialog progress(this);
    progress.setLabelText(tr("正在读取文件..."));
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setMaximum(maximum);

    // 创建document
    DocumentInfo docInfo;
    QFileInfo fileInfo(file);
    docInfo.partIdentifier = ui->identifier->text();
    if (docInfo.partIdentifier.isEmpty()) {                                     // 没有给定识别符则使用文件名称
        docInfo.partIdentifier = fileInfo.completeBaseName();
    }

    QString docPath = "{date}/{identifier}-{date}-{time}";
    docPath.replace("{date}", fileInfo.lastModified().toString("yyyy-MM-dd"));
    docPath.replace("{time}", fileInfo.lastModified().toString("hh-mm-ss"));
    docPath.replace("{identifier}", docInfo.partIdentifier);
    docPath = QDir(Core::ICore::documentPath()).filePath(docPath);

    QString error;
    if (!DocumentManager::createDocument(docPath, docInfo, &error)) {
        QMessageBox::warning(this, QString(), tr("创建文档失败: %1").arg(error));
        return;
    }

    if (!m_recorder) {
        m_recorder = new ImageRecorder(this);
    }
    m_recorder->clear();
    m_header = UTDataHeader();

    QDataStream s(&file);
    ADataFrame waveform;
    CDataFrame gates;
    QVector<float> pose;
    QVector<float> context;

    s.setByteOrder(QDataStream::LittleEndian);
    s.setFloatingPointPrecision(QDataStream::SinglePrecision);

    while (!s.atEnd() && !progress.wasCanceled()) {
        s >> waveform >> gates >> pose >> context;
        if (s.status() != QDataStream::Ok) {
            break;
        }
        if (m_header.featureCount() <= 0) {
            readyToRead(fileName, gates.count(), pose.count());
        }
        if (!m_recorder->recordCount()) {
            break;
        }

        for (ADataValue &v: waveform) {
            v = qBound(ADataValue(0), v, ADataValue(1023));
            v *= m_waveformScale;
        }

        UTDataFrame frame(m_header);
        frame.setPositions(pose);
        frame.setWaveform(waveform);
        for (auto it = m_gateLookup.begin(); it != m_gateLookup.end(); ++it) {
            frame.setFeature(it.value(), gates.value(it.key()));
        }

        // 旧数据I闸门伤时是以delay为0点，不是以zero为零点，此处要加上delay
        int channelId = frame.channelId();
        if (m_syncOffset.contains(channelId)) {
            frame.setGateTime(0, frame.gateTime(0) + m_syncOffset[channelId]);
        }

        frame.setFeatureAsInt(m_header.contextIndex(UTDataHeader::ContextChannel), context.value(0));
        frame.setFeatureAsInt(m_header.contextIndex(UTDataHeader::ContextCoderDirection), context.value(3));
        m_recorder->acquireFrame(frame);
        progress.setValue(qreal(file.pos()) / file.size() * maximum);
    }

    progress.hide();
    if (progress.wasCanceled()) {
        return;
    }

    DocumentManager::currentDocument()->save();
    hide();
}

void PanelImport::readyToRead(const QString &fileName, int numGate, int numPos)
{
    if (!m_recorder) {
        return;
    }

    m_header.setGateCount(4);
    m_header.setPositionCount(numPos);
    m_header.setContexts(UTDataHeader::ContextChannel | UTDataHeader::ContextCoderDirection);
    m_waveformMax = 1024;                                                       // 水浸
    m_gateLookup.clear();
    if (numGate == 18) {
        m_gateLookup[0] = m_header.gateTimeIndex(0);
        m_gateLookup[2] = m_header.gateAmpIndex(0);
        m_gateLookup[3] = m_header.gateTimeIndex(1);
        m_gateLookup[4] = m_header.gateAmpIndex(1);
        m_gateLookup[8] = m_header.gateTimeIndex(2);
        m_gateLookup[9] = m_header.gateAmpIndex(2);
        m_gateLookup[13] = m_header.gateTimeIndex(3);
        m_gateLookup[14] = m_header.gateAmpIndex(3);
    } else if (numGate == 8) {
        for (int gateIndex = 0; gateIndex < 4; ++gateIndex) {
            m_gateLookup[gateIndex * 2] = m_header.gateTimeIndex(gateIndex);
            m_gateLookup[gateIndex * 2 + 1] = m_header.gateAmpIndex(gateIndex);
        }
        m_waveformMax = 512;                                                    // 管探
    }
    m_waveformScale = (UTDataFrame::waveformMax() + 1) / m_waveformMax;
    qCInfo(debug) << "Using header:" << m_header << "Gates:" << m_gateLookup;

    QString configFileName = QFileInfo(fileName).completeBaseName() + ARS_FILE_EXT_PARAMETER;
    QString configFilePath = QDir(QFileInfo(fileName).absolutePath()).filePath(configFileName);
    QJsonObject configJson = ReadFileAsJson(configFilePath).toObject();

    ImageOptions fallbackOptions;
    if (configJson.contains("Transfrom")) {
        fallbackOptions = ImageOptions::fromJson(configJson["Transfrom"]);
        fallbackOptions.setAxisX(configJson["XAxis"].toInt(1) - 1);
        fallbackOptions.setAxisY(configJson["YAxis"].toInt(2) - 1);
    }

    QHash<int, UTSetup> newSetups;                                              // <channelId, UTSetup>
    QHash<int, UTSetup> oldSetups = fromJson(configJson["Setups"]);             // <setupId, UTSetup>
    QJsonObject channelSetupMap = configJson["ChannelSetupMap"].toObject();
    for (QString channel: channelSetupMap.keys()) {
        int setupId = channelSetupMap.value(channel).toInt();
        newSetups[channel.toInt()] = oldSetups[setupId];
    }

    m_syncOffset.clear();
    for (auto it = newSetups.begin(); it != newSetups.end(); ++it) {
        m_syncOffset[it.key()] = it.value().delay();
    }

    for (const QJsonValue &layerItem: configJson["Layers"].toArray()) {
        const QJsonObject layerJson = layerItem.toObject();
        const QString recordName = QString("image%1").arg(m_recorder->recordCount() + 1);

        if (layerJson.contains("ChannelFilter")) {
            MotionAxisList channels;
            for (const QJsonValue &value: layerJson["ChannelFilter"].toArray()) {
                channels.append(value.toBool());
            }
            if (channels.isEmpty()) {
                channels.append(true);
            }
            fallbackOptions.setChannels(channels);
        }

        RecordConfig record;
        record.header = m_header;
        /* 优先采用Layer中保存的ImageOptions */
        record.imageOptions = layerJson.contains("ImageOptions") ? ImageOptions::fromJson(layerJson["ImageOptions"]) : fallbackOptions;
        record.size = record.imageOptions.targetRect();

        RecordLayerConfig config;
        for (int index = 0; index < m_header.featureCount(); ++index) {
            config.fileName = QString::number(index) + ".layer";
            config.feature = index;
            record.layers << config;
        }
        qCInfo(debug) << "ImageRecord" << recordName << "Size" << record.size;
        m_recorder->addImageRecord(recordName + ".imagerecord", record, newSetups);

        ImageRecord *imageRecord = m_recorder->imageRecord(recordName + ".imagerecord");
        if (imageRecord) {
            DialogImageRecord dlg;
            dlg.setConfig(imageRecord->recordConfig());
            dlg.setWindowTitle(imageRecord->fileName());
            if (dlg.exec() == QDialog::Accepted) {
                imageRecord->setRecordConfig(dlg.config());
            }
        }
    }
}
