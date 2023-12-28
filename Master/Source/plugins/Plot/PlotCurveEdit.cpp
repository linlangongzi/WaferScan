#include "PlotCurveEdit.h"
#include "ui_PlotCurveEdit.h"
#include <Ars1000/PipeInstrumentDriver.h>
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QShortcut>
#include <Ultrasonic/UltrasonicManager.h>

PlotCurveEdit::PlotCurveEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotCurveEdit)
{
    ui->setupUi(this);

    ui->channels->setEmptyText(tr("无"));
    for (auto *channel: UltrasonicManager::channels()) {
        ui->channels->addItem(channel->name(), channel->channelId());
    }

    const UTDataHeader header = PipeDetect::PipeInstrumentDriver::InitDataHeader();
    ui->featureIndex->addItems(header.GetDescriptions());
    ui->featureBaseIndex->addItems(header.GetDescriptions());

    int index = 0;
    for (auto axis: MotionManager::axisList()) {
        ui->axis->addItem(axis->name(), index);
        ++index;
    }

    connect(new QShortcut(QString("Ctrl+C"), this), &QShortcut::activated, this, [=]() {
        writeClipboard(option());
    });
    connect(new QShortcut(QString("Ctrl+V"), this), &QShortcut::activated, this, [=]() {
        setOption(readClipboard<PlotCurve>());
    });
}

PlotCurveEdit::~PlotCurveEdit()
{
    delete ui;
}

PlotCurve PlotCurveEdit::option() const
{
    PlotCurve option;

    option.name = ui->name->text();
    option.enable = ui->enable->isChecked();
    option.color = ui->color->color();
    option.lineWidth = ui->lineWidth->value();
    for (const auto &index: ui->channels->selectedItemData()) {
        option.channels.setAxis(index.toInt());
    }
    option.axis = ui->axis->currentIndex();
    option.interval = ui->interval->value();
    option.featureIndex = ui->featureIndex->currentIndex();
    option.featureBaseIndex = ui->featureBaseIndex->currentIndex() - 1;
    option.average = ui->average->value();
    option.scale = ui->scale->value();
    option.correct = ui->correct->value();
    option.reduce = PlotCurve::ReducePolicy(ui->reduce->currentIndex());
    option.offset = SplitNumber<qreal>(ui->offset->text());
    option.comment = ui->comment->text();

    return option;
}

void PlotCurveEdit::setOption(PlotCurve option)
{
    if (m_option == option) {
        return;
    }

    ui->name->setText(option.name);
    ui->enable->setChecked(option.enable);
    ui->color->setColor(option.color);
    ui->lineWidth->setValue(option.lineWidth);

    ui->channels->deselectAll();
    for (int index: option.channels.toList()) {
        ui->channels->selectItem(index);
    }

    ui->axis->setCurrentIndex(option.axis);
    ui->interval->setValue(option.interval);
    ui->featureIndex->setCurrentIndex(option.featureIndex);
    ui->featureBaseIndex->setCurrentIndex(option.featureBaseIndex + 1);
    ui->average->setValue(option.average);
    ui->scale->setValue(option.scale);
    ui->correct->setValue(option.correct);
    ui->reduce->setCurrentIndex(option.reduce);
    ui->offset->setText(JoinNumber(option.offset));
    ui->comment->setText(option.comment);

    m_option = option;
    emit optionChanged(option);
}
