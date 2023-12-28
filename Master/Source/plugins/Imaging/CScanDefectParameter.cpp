#include "CScanDefectParameter.h"
#include "ui_CScanDefectParameter.h"
#include <Ars1000/PipeInstrumentDriver.h>
#include <Ars500/ARS500.h>
#include <Core/Platform.h>
#include <QDebug>
#include <QList>
#include <Ultrasonic/Material.h>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTDataHeader.h>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>

CScanDefectParameter::CScanDefectParameter(int channelIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CScanDefectParameter),
    sourceChannel(channelIndex)     // 判伤时使用的数据通道（globalIndex）
{
    ui->setupUi(this);
    InitUI();
#ifdef PIPE_DETECT
    const UTDataHeader header = PipeDetect::PipeInstrumentDriver::InitDataHeader();
#else
    const UTDataHeader header = Ars500::UTDriverARS500::InitDataHeader();
#endif
    const QStringList headers = header.GetDescriptions();
    ui->depthIndex->addItems(headers);
    ui->bottomDepthIndex->addItems(headers);
    ui->depthIndex->setCurrentIndex(header.GetTimeIndex(1, GateMaxPeak));
    ui->bottomDepthIndex->setCurrentIndex(header.GetTimeIndex(2, GateMaxPeak));
}

CScanDefectParameter::~CScanDefectParameter()
{
    delete ui;
}

void CScanDefectParameter::SetInfo(const UTDefectProcessorOptions &options)
{
    ui->hasDepth->setChecked(options.hasDepth);
    ui->hasBottomDepth->setChecked(options.hasBottomDepth);
    ui->depthIndex->setCurrentIndex(options.depthIndex);
    ui->bottomDepthIndex->setCurrentIndex(options.bottomDepthIndex);

    ui->gateValueSpinBox->setValue(options.threshold);
    ui->gateModeCombox->setCurrentIndex(options.thresholdMode);

    ui->dilation->setValue(options.dilation);

    ui->minOuterArea->setValue(options.minOuterArea);
}

UTDefectProcessorOptions CScanDefectParameter::GetOptions()
{
    UTDefectProcessorOptions option;

    option.threshold = ui->gateValueSpinBox->value();
    option.thresholdMode = UTDefectProcessorOptions::ThresholdMode(ui->gateModeCombox->currentIndex());
    option.hasDepth = ui->hasDepth->isChecked();
    option.hasBottomDepth = ui->hasBottomDepth->isChecked();
    option.depthIndex = ui->depthIndex->currentIndex();
    option.bottomDepthIndex = ui->bottomDepthIndex->currentIndex();
    option.materialVelocity = ui->materials->currentData().toDouble();
    option.dilation = ui->dilation->value();
    option.minOuterArea = ui->minOuterArea->value();

    return option;
}

void CScanDefectParameter::InitUI()
{
    ui->materials->clear();
    for (const Material &material: UltrasonicManager::materialModel()->save()) {
        ui->materials->addItem(material.name, material.longitudinalVelocity);
    }
    UTSetup setup = UltrasonicManager::channelSetup(sourceChannel);
    ui->materials->setCurrentText(setup.materialName());
}
