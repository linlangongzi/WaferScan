#include "ParameterCopy.h"
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include "ui_ParameterCopy.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QProgressDialog>

ParameterCopy::ParameterCopy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterCopy)
{
    ui->setupUi(this);
    initConnections();
}

ParameterCopy::~ParameterCopy()
{
    delete ui;
}

void ParameterCopy::Copy()
{
    qint32 sourceChannel = ui->sourceChannel->value();
    qint32 channelStart = ui->startChannel->value();
    qint32 channelEnd = ui->endChannel->value();
    qint32 channelCount = channelEnd - channelStart + 1;

    if (channelCount <= 0) {
        QMessageBox::information(this, QString(), tr("没有目标通道可以复制参数"), QMessageBox::Yes);
        return;
    }

    if (channelCount == 1 && sourceChannel == channelStart) {
        QMessageBox::information(this, QString(), tr("目标通道号无效"), QMessageBox::Yes);
        return;
    }

    int maxIndex = UltrasonicManager::channelCount();
    UTSetup sourceSetup =UltrasonicManager::channel(sourceChannel)->setup();
    for (int channel = channelStart; channel <= channelEnd; ++channel) {
        if (channel > maxIndex) {
            QMessageBox::warning(this, QString(), tr("复制完成"), QMessageBox::Close);
            return;
        }
        UltrasonicManager::channel(channel)->setSetup(sourceSetup);
    }
    QMessageBox::warning(this, QString(), tr("复制完成"), QMessageBox::Close);
}

void ParameterCopy::LoadSetup()
{
    const QString fileName = QFileDialog::getOpenFileName(this, tr("导入工艺"), QString(), tr("工艺文件 (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }
    //
    QJsonValue doc = ReadFileAsJson(fileName);
    if (doc.isArray()) {
        QJsonArray array = doc.toArray();
        int count = array.count();
        for (int index = 0; index < count; index++) {
            QJsonObject json = array.at(index).toObject();

            if (json.isEmpty()) {
                QMessageBox::warning(this, QString(), tr("导入失败: 无法打开文件，或文件格式不正确"));
                return;
            }

            if (json["Type"] != "UTSetup") {
                QMessageBox::critical(this, QString(), tr("导入失败: 文件格式不正确"));
                return;
            }
            foreach (auto *channel, UltrasonicManager::channels()) {
                channel->setSetup(UTSetup::fromJson(json));
            }
        }
    }
    QMessageBox::information(this, QString(), tr("工艺导入成功"));
}

void ParameterCopy::SaveSetup()
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hhmmss");;
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出工艺"), time, tr("工艺文件 (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }

    QJsonValue setupArray = convertToJson(UltrasonicManager::setups());
    QString errorString;
    bool ret = WriteFileAsJson(fileName, setupArray, true, &errorString);
    if (ret) {
        QMessageBox::information(this, QString(), tr("工艺导出成功"));
    } else {
        QMessageBox::critical(this, QString(), tr("导出失败, 无法写入文件: %1").arg(errorString));
    }
}

void ParameterCopy::Yes()
{
    processInfo.isMark = ui->markSwitch->isChecked();
    processInfo.paintInterval = ui->paintInterval->value();

    emit ProcessInfoChanged(processInfo);
    close();
}

void ParameterCopy::initConnections()
{
    connect(ui->copy, &QPushButton::clicked, this, &ParameterCopy::Copy);
    connect(ui->importSetup, &QPushButton::clicked, this, &ParameterCopy::LoadSetup);
    connect(ui->exportSetup, &QPushButton::clicked, this, &ParameterCopy::SaveSetup);
}
