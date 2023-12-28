#include "Benchmark.h"
#include "InstrumentOptionsEdit.h"
#include "ui_InstrumentOptionsEdit.h"
#include <Ultrasonic/UltrasonicManager.h>

InstrumentOptionsEdit::InstrumentOptionsEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentOptionsEdit),
    configHandler(this, &InstrumentOptionsEdit::config, &InstrumentOptionsEdit::setConfig),
    setupHandler(this, &InstrumentOptionsEdit::setup, &InstrumentOptionsEdit::setSetup)
{
    ui->setupUi(this);

    ui->deviceType->addItems(UltrasonicManager::availableDriverNames());

    configHandler.add(ui->name, &InstrumentConfig::name);
    configHandler.add(ui->deviceType, &InstrumentConfig::deviceType);
    configHandler.add(ui->address, &InstrumentConfig::address);
    configHandler.add(ui->channelCount, &InstrumentConfig::channelCount);

    setupHandler.add(ui->prf, &InstrumentSetup::prf);
    setupHandler.add(ui->prf, &InstrumentSetup::transmitVoltage);
    setupHandler.add(ui->syncMode, &InstrumentSetup::pulseMode);

    ui->command->installEventFilter(this);

    startTimer(1000);
}

InstrumentOptionsEdit::~InstrumentOptionsEdit()
{
    delete ui;
}

UTInstrument *InstrumentOptionsEdit::instrument() const
{
    return m_instrument;
}

InstrumentConfig InstrumentOptionsEdit::config() const
{
    if (m_instrument) {
        return m_instrument->config();
    }
    return InstrumentConfig();
}

InstrumentSetup InstrumentOptionsEdit::setup() const
{
    return m_instrument->setup();
}

void InstrumentOptionsEdit::setInstrument(UTInstrument *instrument)
{
    if (m_instrument) {
        disconnect(m_instrument, nullptr, this, nullptr);
    }
    m_instrument = instrument;
    if (instrument) {
        connect(instrument, &UTInstrument::instrumentStatusChanged, this, &InstrumentOptionsEdit::onConnectionChanged);
        onConnectionChanged(instrument->instrumentStatus());

        connect(instrument, &UTInstrument::instrumentInfoChanged, this, &InstrumentOptionsEdit::onInstrumentInfoChanged);
        onInstrumentInfoChanged(instrument->instrumentInfo());

        ui->commandHistory->setDocument(instrument->messageLog());

        ui->name->setText(instrument->name());
        InstrumentConfig config = instrument->config();
        configHandler.setControls(config);
        refreshBenchmark();
    }
}

void InstrumentOptionsEdit::setConfig(const InstrumentConfig &config)
{
    if (m_instrument) {
        m_instrument->setConfig(config);
    }
    configHandler.setControls(config);
}

void InstrumentOptionsEdit::setSetup(const InstrumentSetup &setup)
{
    m_instrument->setSetup(setup);
}

void InstrumentOptionsEdit::refreshBenchmark()
{
    if (!m_instrument) {
        return;
    }
    IInstrumentDriver *driver = m_instrument->instrumentDriver();
    if (!driver) {
        return;
    }
    Benchmark *benchmark = driver->benchmark();
    if (!benchmark) {
        ui->benchmark->setEnabled(false);
        return;
    }

    ui->benchmark->setHeaderLabels(QStringList() << "" << benchmark->GetColName().values());
    ui->benchmark->clear();

    QMap<int, QTreeWidgetItem *> rowEntries;                                    /*!< 行号 - item 查找表 */
    const auto rowNames = benchmark->GetRowName();
    const auto rowParents = benchmark->GetRowParent();
    for (auto it = rowNames.cbegin(); it != rowNames.cend(); ++it) {
        QTreeWidgetItem *item;
        int parentId = rowParents[it.key()];
        if (rowParents.contains(it.key()) && rowEntries.contains(parentId)) {
            QTreeWidgetItem *parentItem = rowEntries[parentId];
            item = new QTreeWidgetItem(parentItem);
        } else {
            item = new QTreeWidgetItem(ui->benchmark);
        }
        item->setText(0, it.value());
        rowEntries[it.key()] = item;
    }

    auto data = benchmark->Snapshot();
    clock_t time = data.take(QPoint(INT_MAX, INT_MAX));
    for (auto it = data.cbegin(); it != data.cend(); ++it) {
        const QPoint p = it.key();
        if (rowEntries.contains(p.y())) {
            rowEntries[p.y()]->setData(p.x() + 1, Qt::DisplayRole, it.value() / time * CLOCKS_PER_SEC);
        }
    }
    ui->benchmark->expandAll();
}

bool InstrumentOptionsEdit::eventFilter(QObject *watched, QEvent *event)
{
    /* 按 Enter 时发送仪器命令. QLineEdit::returnPressed 会导致对话框的确定被按下. */
    if (watched == ui->command && event->type() == QEvent::KeyPress) {
        auto *e = static_cast<QKeyEvent *>(event);
        if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
            if (m_instrument && !ui->command->text().isEmpty()) {
                m_instrument->sendCommand(ui->command->text());
                ui->command->clear();
            }
            event->accept();
            return true;
        }
    }
    return false;
}

void InstrumentOptionsEdit::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (ui->benchmark->isVisibleTo(this)) {
        qDebug() << "Refreshing benchmark";
        refreshBenchmark();
    }
}

void InstrumentOptionsEdit::onConnectionChanged(const InstrumentStatus &status)
{
    bool locked = (status.state != InstrumentStatus::Disconnected);

    if (m_instrument) {
        ui->connectionStatus->setText(m_instrument->instrumentStatus().text());
    }
    ui->connect->setVisible(!locked);
    ui->disconnect->setVisible(locked);

    ui->deviceType->setDisabled(locked);
    ui->address->setDisabled(locked);
    ui->channelCount->setDisabled(locked);
}

void InstrumentOptionsEdit::onInstrumentInfoChanged(const InstrumentInfo &info)
{
    ui->model->setText(info.model);
    ui->hardwareVersion->setText(info.hardwareVersion);
    ui->softwareVersion->setText(info.softwareVersion);
    ui->fpgaVersion->setText(info.fpgaVersion);
    ui->serialNumber->setText(info.serialNumber);
    ui->manufacturer->setText(info.manufacturer);

    ui->resource->setText(tr("%1 通道 / %2 编码器 / %3 硬件闸门").arg(info.channelCount).arg(info.encoderCount).arg(info.gateCount));
}

void InstrumentOptionsEdit::on_connect_clicked()
{
    if (m_instrument) {
        m_instrument->Connect();
    }
}

void InstrumentOptionsEdit::on_disconnect_clicked()
{
    if (m_instrument) {
        m_instrument->Disconnect();
    }
}
