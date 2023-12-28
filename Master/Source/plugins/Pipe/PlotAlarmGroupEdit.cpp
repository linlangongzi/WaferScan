#include "PlotAlarmGroupEdit.h"
#include "ui_PlotAlarmGroupEdit.h"
#include <Ars1000/PipeInstrumentDriver.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <QInputDialog>
#include <QListWidget>
#include <QShortcut>

PlotAlarmGroupEdit::PlotAlarmGroupEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotAlarmGroupEdit)
{
    ui->setupUi(this);

    ui->featureIndex->addItems(PipeDetect::PipeInstrumentDriver::InitDataHeader().GetDescriptions());

    connect(new QShortcut(QString("Ctrl+C"), this), &QShortcut::activated, this, [=]() {
        writeClipboard(option());
    });
    connect(new QShortcut(QString("Ctrl+V"), this), &QShortcut::activated, this, [=]() {
        setOption(readClipboard<PlotAlarmGroup>());
    });
}

PlotAlarmGroupEdit::~PlotAlarmGroupEdit()
{
    delete ui;
}

PlotAlarmGroup PlotAlarmGroupEdit::option() const
{
    PlotAlarmGroup option;

    option.name = ui->name->text();
    option.inactiveColor = ui->inactiveColor->color();
    option.activeColor = ui->activeColor->color();
    option.featureIndex = ui->featureIndex->currentIndex() - 1;

    for (int index = 0; index < ui->indicators->topLevelItemCount(); ++index) {
        QTreeWidgetItem *item = ui->indicators->topLevelItem(index);
        option.indicators << getIndicator(item);
    }

    return option;
}

void PlotAlarmGroupEdit::setOption(const PlotAlarmGroup &option)
{
    ui->name->setText(option.name);
    ui->inactiveColor->setColor(option.inactiveColor);
    ui->activeColor->setColor(option.activeColor);
    ui->featureIndex->setCurrentIndex(option.featureIndex + 1);

    ui->indicators->clear();
    for (const auto &indicator: option.indicators) {
        refreshIndicatorItem(new QTreeWidgetItem(ui->indicators), indicator);
    }
}

void PlotAlarmGroupEdit::on_indicators_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (!item) {
        return;
    }
    PlotAlarmIndicator indicator = getIndicator(item);
    bool ok = false;
    if (column == 0) {
        indicator.name = askName(item->text(0), &ok);
    } else if (column == 1) {
        indicator.channel = askChannel(item->data(1, Qt::UserRole).toInt(), &ok);
    }
    if (ok) {
        refreshIndicatorItem(item, indicator);
    }
}

void PlotAlarmGroupEdit::on_add_clicked()
{
    bool ok = false;
    PlotAlarmIndicator indicator;
    indicator.name = askName(QString(), &ok);
    if (!ok) {
        return;
    }
    indicator.channel = askChannel(0, &ok);
    if (!ok) {
        return;
    }
    refreshIndicatorItem(new QTreeWidgetItem(ui->indicators), indicator);
}

int PlotAlarmGroupEdit::askChannel(int channelId, bool *ok)
{
    QStringList channels;
    for (auto channel: UltrasonicManager::channels()) {
        channels << QString("%1 (%2)").arg(channel->name()).arg(channel->channelId());
    }
    QString channel = QInputDialog::getItem(this, QString(), tr("通道"), channels, channelId, false, ok);
    return channels.indexOf(channel);
}

QString PlotAlarmGroupEdit::askName(QString name, bool *ok)
{
    return QInputDialog::getText(this, QString(), tr("报警项名称"), QLineEdit::Normal, name, ok);
}

void PlotAlarmGroupEdit::refreshIndicatorItem(QTreeWidgetItem *item, const PlotAlarmIndicator &indicator)
{
    item->setText(0, indicator.name);
    UTChannel *channel = UltrasonicManager::channel(indicator.channel);
    QString channelName = channel ? channel->name() : "";
    item->setText(1, QString("%1 (%2)").arg(channelName).arg(indicator.channel));
    item->setData(1, Qt::UserRole, indicator.channel);
}

PlotAlarmIndicator PlotAlarmGroupEdit::getIndicator(QTreeWidgetItem *item)
{
    PlotAlarmIndicator indicator;
    indicator.name = item->text(0);
    indicator.channel = item->data(1, Qt::UserRole).toInt();
    return indicator;
}
