#include "ScanPathRingEdit.h"
#include "ui_ScanPathRingEdit.h"
#include "AcquisitionConstants.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QInputDialog>
#include <QLoggingCategory>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/MetaType/JsonConverter.h>
#include <utils/algorithm.h>
//#include <Core/ICore.h>
//#include <Ultrasonic/UltrasonicManager.h>

extern QMap<QVariant, QVariant> elementTypeMap;

static Q_LOGGING_CATEGORY(debug, "acquire.ring");

using namespace Acquisition;

ScanPathRingEdit::ScanPathRingEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanPathRingEdit)
{
    ui->setupUi(this);

//    ui->channels->setItemDelegate(new ChannelDelegate(this));

    ui->centerType->addItem(tr("直接输入"), ScanPathRingOptions::DirectInput);
    ui->centerType->addItem(tr("计算"), ScanPathRingOptions::Calculate);

    ui->direction->addItem(tr("上表面"), ScanPathRingOptions::Above);
    ui->direction->addItem(tr("内侧面"), ScanPathRingOptions::Inside);
    ui->direction->addItem(tr("外侧面"), ScanPathRingOptions::Outside);

    connect(ui->radius, SIGNAL(valueChanged(double)), this, SLOT(refreshCenter()));
    connect(ui->centerType, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshCenter()));
}

ScanPathRingEdit::~ScanPathRingEdit()
{
    delete ui;
}

void ScanPathRingEdit::setOptions(const ScanPathRingOptions &options)
{
    ui->scanAxis->clear();
    for (Axis *axis: MotionManager::axisList()) {
        ui->scanAxis->addItem(axis->name(), axis->config().axisId);
    }
    ui->scanAxis->setCurrentIndex(ui->scanAxis->findData(options.scanAxis));

    ui->linearSpeed->setValue(options.linearSpeed);
    ui->stepInterval->setValue(options.stepInterval);
    ui->angleStart->setValue(options.angleStart);
    ui->angleEnd->setValue(options.angleEnd);
    ui->pressingAngle->setValue(options.pressingAngle);
    ui->depth->setValue(options.depth);

    ui->direction->setCurrentIndex(options.direction);
    ui->centerType->setCurrentIndex(options.centerType);
    ui->centerX->setValue(options.center.x());
    ui->centerY->setValue(options.center.y());
    ui->radius->setValue(options.radius);
    ui->innerRadius->setValue(options.innerRadius);
    ui->height->setValue(options.height);
    ui->probeInterval->setValue(options.probeInterval);

    ui->channels->clear();
    ui->channels->setColumnCount(2);
    const int rowCount = options.multiChannels.count();
    for (int row = 0; row < rowCount; ++row) {
        auto *item = new QTreeWidgetItem(ui->channels);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ChannelList channels = options.multiChannels.value(row);
        for (int column = 0; column < ui->channels->columnCount(); ++column) {
            int channelIndex = channels.value(column);
            UTChannel *channel = UltrasonicManager::channel(channelIndex);
            if (channel) {
                item->setData(column, Qt::DisplayRole, channel->name());
                item->setData(column, Qt::UserRole, channelIndex);
            } else {
                item->setData(column, Qt::DisplayRole, tr("无"));
                item->setData(column, Qt::UserRole, -1);
            }
        }
    }

    refreshCenter();
    resizeChannelItems();
}

ScanPathRingOptions ScanPathRingEdit::options() const
{
    ScanPathRingOptions options;

    options.scanAxis = ui->scanAxis->currentData(Qt::UserRole).toInt();
    options.linearSpeed = ui->linearSpeed->value();
    options.stepInterval = ui->stepInterval->value();
    options.angleStart = ui->angleStart->value();
    options.angleEnd = ui->angleEnd->value();
    options.pressingAngle = ui->pressingAngle->value();
    options.depth = ui->depth->value();

    options.direction = ScanPathRingOptions::StepDirection(ui->direction->currentData(Qt::UserRole).toInt());
    options.centerType = ScanPathRingOptions::CenterType(ui->centerType->currentData(Qt::UserRole).toInt());
    options.center.setX(ui->centerX->value());
    options.center.setY(ui->centerY->value());
    options.radius = ui->radius->value();
    options.innerRadius = ui->innerRadius->value();
    options.height = ui->height->value();
    options.probeInterval = ui->probeInterval->value();

    options.multiChannels.clear();
    const int topLevelItemCount = ui->channels->topLevelItemCount();
    for (int row = 0; row < topLevelItemCount; ++row) {
        ChannelList channels;
        for (int column = 0; column < ui->channels->columnCount(); ++column) {
            int channelIndex = ui->channels->topLevelItem(row)->data(column, Qt::UserRole).toInt();
            channels << channelIndex;
        }
        options.multiChannels << channels;
    }

    return options;
}

void ScanPathRingEdit::on_addChannel_annulus_clicked()
{
    auto *item = new QTreeWidgetItem(ui->channels);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    for (int column = 0; column < ui->channels->columnCount(); ++column) {
        item->setData(column, Qt::DisplayRole, tr("无"));
        item->setData(column, Qt::UserRole, -1);
    }
}

void ScanPathRingEdit::on_removeChannel_annulus_clicked()
{
    delete ui->channels->takeTopLevelItem(ui->channels->currentIndex().row());
}

void ScanPathRingEdit::on_centerType_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    const auto type = ScanPathRingOptions::CenterType(ui->centerType->currentData().toInt());

    if (type == ScanPathRingOptions::DirectInput) {
        ui->centerContainer->setEnabled(true);
    } else if (type == ScanPathRingOptions::Calculate) {
        ui->centerContainer->setEnabled(false);
    }
}

void ScanPathRingEdit::refreshCenter()
{
    const auto type = ScanPathRingOptions::CenterType(ui->centerType->currentData().toInt());
    if (type != ScanPathRingOptions::Calculate) {
        return;
    }

    const qreal radius = ui->radius->value();
//    QSettings *settings = Core::ICore::settings();
//    settings->beginGroup(K_GROUP_RING);
    const QPointF placementOrigin = QPointF(platform()->Get("AnnulusDatumPointX").toDouble(), platform()->Get("AnnulusDatumPointY").toDouble());
    const QPointF placementDirection = QPointF(platform()->Get("AnnulusDirectionX").toDouble(), platform()->Get("AnnulusDirectionY").toDouble());
//    settings->endGroup();

    QPointF p = placementOrigin + radius * placementDirection;
    ui->centerX->setValue(p.x());
    ui->centerY->setValue(p.y());
}

void ScanPathRingEdit::resizeChannelItems()
{
    for (int index = 0; index < ui->channels->topLevelItemCount(); ++index) {
        ui->channels->resizeColumnToContents(index);
    }
}
