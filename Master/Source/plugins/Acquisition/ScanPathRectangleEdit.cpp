#include "ScanPathRectangleEdit.h"
#include "ui_ScanPathRectangleEdit.h"
#include <Acquisition/AxisSelector.h>
#include <Core/LicenseManager.h>
#include <Motion/EndPoint.h>
#include <Motion/EndPointSelector.h>
#include <Motion/MotionManager.h>
#include <QInputDialog>
#include <QLoggingCategory>
#include <QMessageBox>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/DialogUTSetup.h>
#include <Waveform/PanelUTSetup.h>
#include <utils/algorithm.h>

static Q_LOGGING_CATEGORY(debug, "acquire.rect.edit")

ScanPathRectangleEdit::ScanPathRectangleEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanPathRectangleEdit),
    m_handler(this, &ScanPathRectangleEdit::options, &ScanPathRectangleEdit::setOptions)
{
    ui->setupUi(this);
    ui->featuresTwoPointZero->setVisible(false);
    ui->endPoints->setCurrentIndex(-1);
    connect(ui->scanAxes, QOverload<int>::of(&AxisSelector::currentIndexChanged), this, &ScanPathRectangleEdit::resetSpeedBoxRange);

    for (UTChannel *channel: UltrasonicManager::channels()) {
        ui->channel->addItem(channel->name(), channel->channelId());
    }
    initHandler();

    for (UTChannel *channel: UltrasonicManager::channels()) {
        connect(channel, &UTChannel::setupChanged, this, &ScanPathRectangleEdit::updateLinearSpeedValue);
    }
}

void ScanPathRectangleEdit::initHandler()
{
    m_handler.addHandler(ui->endPoints, &EndPointSelector::setCurrentUuid, &EndPointSelector::currentUuidChanged, &ScanPathRectangleOptions::endPointId);
    m_handler.addHandler(ui->scanAxes, &AxisSelector::setCurrentUuid, &AxisSelector::currentAxisUuidChanged, &ScanPathRectangleOptions::scanAxisId);
    m_handler.addHandler(ui->topLeft, &PointXYZ::setNativePos, &PointXYZ::pointChanged, &ScanPathRectangleOptions::start);
    m_handler.addHandler(ui->bottomRight, &PointXYZ::setNativePos, &PointXYZ::pointChanged, &ScanPathRectangleOptions::end);
    m_handler.addHandler(ui->channelsOffsets, &ChannelTreeWidget::setChannels, &ChannelTreeWidget::channelsChanged, &ScanPathRectangleOptions::channels);
    m_handler.addHandler(ui->channelsOffsets, &ChannelTreeWidget::setProbeIntervals, &ChannelTreeWidget::probeIntervalsChanged, &ScanPathRectangleOptions::probeIntervalList);
    m_handler.add(ui->channel, &ScanPathRectangleOptions::channel);
    m_handler.add(ui->scanInterval, &ScanPathRectangleOptions::scanInterval);
    m_handler.add(ui->linearSpeed, &ScanPathRectangleOptions::linearSpeed);
    m_handler.add(ui->stepInterval, &ScanPathRectangleOptions::stepInterval);
    m_handler.add(ui->probeInterval, &ScanPathRectangleOptions::probeInterval);
    m_handler.add(ui->gainOffset, &ScanPathRectangleOptions::gainOffset);
    m_handler.add(ui->setupEnable, &ScanPathRectangleOptions::setupEnable);
    m_handler.add(ui->unidirection, &ScanPathRectangleOptions::unidirection);
    m_handler.add(ui->userLinearSpeed, &ScanPathRectangleOptions::userLinearSpeedEnable);
    m_handler.add(ui->scanLink, &ScanPathRectangleOptions::alterSourceEnable);
}

ScanPathRectangleEdit::~ScanPathRectangleEdit()
{
    delete ui;
}

void ScanPathRectangleEdit::updateLinearSpeedValue()
{
    ui->linearSpeedValue->setText(QString("%1 mm/s").arg(m_options.calculatedAdvicedSpeed()));
    ui->linearSpeed->setStyleSheet(m_options.linearSpeed > m_options.calculatedAdvicedSpeed() ? QString("color: red;") : QString());
}

void ScanPathRectangleEdit::setOptions(const ScanPathRectangleOptions &options)
{
    using Options = ScanPathRectangleOptions;
    m_options = options;
    updateLinearSpeedValue();

    {
        QSignalBlocker blocker(ui->scanAxes);
        ui->scanAxes->setFilter([=] (Axis *axis) {
            EndPoint *endPoint = MotionManager::endPoint(options.endPointId);
            if (endPoint == nullptr) {
                return false;
            }
            return endPoint->axisList().contains(axis);
        });
        blocker.unblock();
    }

    QSignalBlocker blocker(ui->single);
    ui->single->setChecked(options.channelSelect == Options::Single);
    blocker.unblock();

    QSignalBlocker block(ui->parallel);
    ui->parallel->setChecked(options.channelSelect == Options::Parallel);
    block.unblock();

    QSignalBlocker offsetsBlock(ui->parallelList);
    ui->parallelList->setChecked(options.channelSelect == Options::ParallelList);
    offsetsBlock.unblock();

    ui->setupEdit->setEnabled(options.setupEnable);
    ui->waveformSaveNone->setChecked(options.waveformSave == Options::WaveformSaveNone);
    ui->waveformSaveAll->setChecked(options.waveformSave == Options::WaveformSaveAll);
    ui->waveformSavePart->setChecked(options.waveformSave == Options::WaveformSavePart);
    ui->featureSaveAll->setChecked(options.featureSave == Options::FeatureSaveAll);
    ui->featureSavePart->setChecked(options.featureSave == Options::FeatureSavePart);

    ui->linearSpeed->setVisible(options.userLinearSpeedEnable);
    ui->linearSpeedValue->setVisible(!options.userLinearSpeedEnable);
    ui->gainOffset->setEnabled(options.alterSourceEnable);

    ui->channelsOffsets->setEnabled(options.channelSelect != Options::Single);
    ui->probeInterval->setEnabled(options.channelSelect == Options::Parallel);
    ui->channel->setEnabled(options.channelSelect == Options::Single);
    m_handler.setControls(m_options);
}

ScanPathRectangleOptions ScanPathRectangleEdit::options() const
{
    return m_options;
}

void ScanPathRectangleEdit::onLicenseUpdated()
{
    bool develop = LicenseManager::instance()->hasDevelopLicense();
    ui->gainOffsetTitle->setVisible(develop);
    ui->gainOffset->setVisible(develop);
}

void ScanPathRectangleEdit::resetSpeedBoxRange()
{
    Axis *mainAxis = ui->scanAxes->currentAxis();
    if (mainAxis == nullptr) {
        return;
    }
    ui->linearSpeed->setMaximum(mainAxis->config().speedMax);
}

UTSetup ScanPathRectangleEdit::setup() const
{
    QList<UTChannel *> channels = UltrasonicManager::channels();
    if (channels.isEmpty()) {
        return UTSetup();
    }
    return channels.first()->setup();
}

void ScanPathRectangleEdit::on_waveformSavePart_toggled(bool checked)
{
    ui->waveformContainer->setEnabled(checked);
}

void ScanPathRectangleEdit::on_featureSavePart_toggled(bool checked)
{
    ui->featureContainer->setEnabled(checked);
}

int ScanPathRectangleEdit::currentChannelId() const
{
    if (ui->single->isChecked()) {
        return ui->channel->currentData(Qt::UserRole).toInt();
    }
    if (ui->parallel->isChecked() && ui->channelsOffsets->currentItem()) {
        return ui->channelsOffsets->currentItem()->data(0, Qt::UserRole).toDouble();
    }
    if (ui->parallelList->isChecked() && ui->channelsOffsets->currentItem()) {
        return ui->channelsOffsets->currentItem()->data(1, Qt::UserRole).toDouble();
    }
    return 0;
}

void ScanPathRectangleEdit::on_setupEdit_clicked()
{
    DialogUTSetup dialog;
    dialog.setSetup(setup());
    if (dialog.exec() == QDialog::Accepted) {
        m_handler.setField(&ScanPathRectangleOptions::setup, setup());
    }
}

void ScanPathRectangleEdit::on_setupCopyFrom_clicked()
{
    UTSetup setup = this->setup();
    QList<UTChannel *> channels = Utils::filtered(UltrasonicManager::channels(), [&](UTChannel *channel) {
            return channel->setup() != setup;
        });
    if (channels.isEmpty()) {
        return;
    }

    QList<QString> names = Utils::transform(channels, [] (UTChannel *channel) {
        return channel->name();
    });

    bool ok = false;
    QString name = QInputDialog::getItem(this, QString(), tr("通道选择"), names, 0, false, &ok);
    if (!ok) {
        return;
    }

    setup = channels[names.indexOf(name)]->setup();
    m_handler.setField(&ScanPathRectangleOptions::setup, setup);
}

void ScanPathRectangleEdit::on_setupCopyTo_clicked()
{
    UTSetup setup = this->setup();
    const QList<UTChannel *> channels = Utils::filtered(UltrasonicManager::channels(), [&](UTChannel *channel) {
        return channel->setup() != setup;
    });
    if (channels.isEmpty()) {
        return;
    }

    QMessageBox::StandardButton button = QMessageBox::question(this, QString(), tr("是否复制到全部探伤通道"), QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::Yes) {
        for (UTChannel *channel: channels) {
            channel->setSetup(setup);
        }
    } else {
        const QList<QString> names = Utils::transform(channels, [] (UTChannel *channel) {
            return channel->name();
        });
        int currentId = currentChannelId();
        if (uint(currentId) >= uint(names.count())) {
            currentId = 0;
        }
        bool ok = false;
        QString name = QInputDialog::getItem(this, QString(), tr("选择通道"), names, currentId, false, &ok);
        if (!ok) {
            return;
        }
        channels[names.indexOf(name)]->setSetup(setup);
    }
}

void ScanPathRectangleEdit::on_setupEnable_toggled(bool checked)
{
    ui->setupEdit->setEnabled(checked);
}

void ScanPathRectangleEdit::on_single_toggled(bool checked)
{
    if (checked) {
        m_handler.setField(&ScanPathRectangleOptions::channelSelect, ScanPathRectangleOptions::Single);
    }
}

void ScanPathRectangleEdit::on_parallel_toggled(bool checked)
{
    if (checked) {
        m_handler.setField(&ScanPathRectangleOptions::channelSelect, ScanPathRectangleOptions::Parallel);
    }
}

void ScanPathRectangleEdit::on_parallelList_toggled(bool checked)
{
    if (checked) {
        m_handler.setField(&ScanPathRectangleOptions::channelSelect, ScanPathRectangleOptions::ParallelList);
    }
    ui->channelsOffsets->setIntervalColumnVisible(!checked);
}

void ScanPathRectangleEdit::on_scanLink_toggled(bool checked)
{
    ui->gainOffset->setEnabled(checked);
}
