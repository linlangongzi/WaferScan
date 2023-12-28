#include "SixJoints.h"
#include "ui_SixJoints.h"
#include "QDoubleSpinBox"
#include "QSlider"
#include <QTimer>
#include <QTextStream>
#include <QDebug>
#include "SerialPort.h"

const char indexPropertyName[] = "index";

SixJoints::SixJoints(QWidget *parent, Platform::Type type) :
    QDialog(parent),
    ui(new Ui::SixJoints)
{
    ui->setupUi(this);
    stewartType = type;
    if (type == Platform::LinkType) {
        qDebug() << "This is steer structure";
        driveRate = 500000 / 360;
    } else if (type == Platform::ClassicType) {
        qDebug() << "This is stepper structure";
        driveRate = 10000 / 5;
    }

    stewart = new Platform(type);
    //轴控制模式下为每一个slider设置量程及零点
    //TCP模式下，为每一个slider设置量程及零点
    for (int index = 1; index <= 6; index++) {
        QSlider *slider = findChild<QSlider *>(QString("verticalSliderTCP%1").arg(index));
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(index));

        slider->setMaximum(stewart->range[index - 1][2]);
        slider->setMinimum(stewart->range[index - 1][0]);
        slider->setValue(stewart->range[index - 1][1]);

        doubleSpinBox->setMaximum(stewart->range[index - 1][2]);
        doubleSpinBox->setMinimum(stewart->range[index - 1][0]);
        doubleSpinBox->setValue(stewart->range[index - 1][1]);

        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onTCPValueChangedFromSpinBox()));
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onTCPValueChangedFromSlider()));
        doubleSpinBox->setProperty(indexPropertyName, index);
        slider->setProperty(indexPropertyName, index);
        poseGoal[index - 1] = stewart->range[index - 1][1];
        poseCurrent[index - 1] = stewart->range[index - 1][1];
    }

    //设置TCP模式不可用
    ui->groupBoxTCP->setEnabled(1);
    timerForRecode = new QTimer(this);
    timerForStart = new QTimer(this);
    timerForInterpolation = new QTimer(this);
    timerForInterpolation->start(50);
    connect(timerForInterpolation, SIGNAL(timeout()), this, SLOT(UpdatePose()));
}

SixJoints::~SixJoints()
{
    delete ui;
}

bool SixJoints::GetJoints(QVector<double> &joints)
{
    if (stewart->GetJoints(joints)) {
        return true;
    } else {
        return false;
    }
}

QVector<double> SixJoints::GetPose()
{
    QVector<double> pose(6);
    for (int id = 1; id <= 6; id++) {
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(id));
        pose[id - 1] = doubleSpinBox->value();
    }
    return pose;
}

/*!
 * \brief SixJoints::onTCPValueChanged              由spinBox触发后，更改slider的值，并将TCP发射出去
 */
void SixJoints::onTCPValueChangedFromSpinBox()
{
    //spinBox的变化引起slider的变化
    QObject *doubleSpinBox = sender();
    if (!doubleSpinBox) {
        return;
    }
    bool ok = false;
    int index = doubleSpinBox->property(indexPropertyName).toInt(&ok);
    if (!ok) {
        return;
    }
    QDoubleSpinBox *temp = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(index));
    QSlider *slider = findChild<QSlider *>(QString("verticalSliderTCP%1").arg(index));
    slider->setValue((int)(temp->value()));

    //发射TCP变化的信号给串口
    QVector<qreal> pos(6,0);
    for (int id = 1; id <= 6; id++) {
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(id));
        pos[id - 1] = doubleSpinBox->value();
    }
    SetPos(pos);
    emit TCPChanged(pos);

}
/*!
 * \brief SixJoints::onTCPValueChanged1             由slider触发后，更改spinbox的值
 */
void SixJoints::onTCPValueChangedFromSlider()
{
    //slider的变化引起spinbox的变化
    QObject *sliderSender = sender();
    if (!sliderSender) {
        return;
    }
    bool ok = false;
    int index = sliderSender->property(indexPropertyName).toInt(&ok);
    if (!ok) {
        return;
    }
    QSlider *slider = findChild<QSlider *>(QString("verticalSliderTCP%1").arg(index));
    QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(index));
    doubleSpinBox->setValue((qreal)(slider->value()));

    QVector<double> pos(6);
    for (int id = 1; id <=6; ++id) {
        QDoubleSpinBox *spinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(id));
        pos[id - 1] = spinBox->value();
    }
    SetPos(pos);
}

void SixJoints::setSpinBoxPos(QVector<qreal> pos)
{
//    qDebug() << "Pose: " << pos;
    for (int id = 1; id <=6; ++id) {
        QDoubleSpinBox *spinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(id));
        spinBox->setValue(pos[id - 1]);
    }
}
/*!
 * \brief SixJoints::on_pushButtonReset_clicked     响应“复位”按键
 */
void SixJoints::on_pushButtonReset_clicked()
{
    for (int index = 1; index <= 6; index++) {
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(index));
        doubleSpinBox->setValue(stewart->range[index - 1][1]);
    }
}

void SixJoints::SetPos(double xp, double yp, double zp, double ap, double bp, double cp)
{
    stewart->SetPos(xp, yp, zp, ap, bp, cp,Platform::ClassicType);
    QVector<double> pos(6);
    pos << xp << yp << zp << ap << bp << cp;
    SetPos(pos);
}

void SixJoints::SetPos(QVector<double> pos)
{
    poseGoal = pos;
    if (stewartType == Platform::LinkType) {
        stewart->SetPos(pos, Platform::LinkType);
        QVector<double> joints(6);
        if (stewart->GetJoints(joints)) {
            for (int index = 1; index <= 6; ++index) {
                QLabel *label = findChild<QLabel *>(QString("joint%1").arg(index));
                label->setText( QString("%1").arg(joints.at(index - 1)));
            }
            joints[0] = 90 + joints[0];
            joints[1] = 90 - joints[1];
            joints[2] = 90 + joints[2];
            joints[3] = 90 - joints[3];
            joints[4] = 90 + joints[4];
            joints[5] = 90 - joints[5];
            emit SendDataBySerial(joints);                                //通过串口向小平台发送位置
        }
    } else if (stewartType == Platform::ClassicType) {
        stewart->SetPos(pos, Platform::ClassicType);
        QVector<double> joints(6);
        if (stewart->GetJoints(joints)) {
            emit SendDataBySerial(joints);
            for (int index = 1; index <= 6; ++index) {
                QLabel *label = findChild<QLabel *>(QString("joint%1").arg(index));
                label->setText( QString("%1").arg(joints.at(index - 1)));
            }
        }
    }
}

/*!
 * \brief SixJoints::UpdatePose 大平发报前先做平滑处理
 */
void SixJoints::UpdatePose()
{
    if (!enable) {
        return;
    }
    const qreal kp = 0.5, ki = 0, kd = 5.65;
    error_last = error;
    error = QVector<qreal>(6);
    qreal dPos = 0;
    for (int i = 0; i < 6; ++i) {
        error[i] += poseGoal[i] - poseCurrent[i];
        dPos += qAbs(error[i]);
        integrate[i] += error[i];
        qreal acc = kp * error[i] + ki * integrate[i] + kd * (error[i] - error_last[i]);
        acc = qBound<qreal>(-0.2, acc, 0.2);
        velocity[i] += acc;
        if (qFuzzyCompare(error[i], 0)) {
            continue;
        }
        poseCurrent[i] += velocity[i];
    }
    if (dPos < 0.1) {
        return;
    }
//    QFile pid(QString("PID.csv"));

//    if (!pid.open(QIODevice::Append)) {
//        return;
//    }
//    QTextStream csvPid(&pid);

//    csvPid << poseCurrent[0] << ',' << poseCurrent[1] << ',' << poseCurrent[2] << ','
//           << poseCurrent[3] << ',' << poseCurrent[4] << ',' << poseCurrent[5] << '\n';

    if (stewartType == Platform::LinkType) {
        stewart->SetPos(poseCurrent, Platform::LinkType);
        QVector<double> joints(6);
        if (stewart->GetJoints(joints)) {
            QVector<qreal> jointsToSend = joints;
            for (int index = 0; index < 6; ++index) {
                if (joints[index] < -5) {                                   //如果角度小于-5度，则无操作
                    return;
                }
            }
            //根据电机的实际安装位置，确实报文发送的序号
            jointsToSend[0] = joints[3] * driveRate;
            jointsToSend[1] = joints[1] * driveRate;
            jointsToSend[2] = joints[5] * driveRate;
            jointsToSend[3] = joints[4] * driveRate;
            jointsToSend[4] = joints[2] * driveRate;
            jointsToSend[5] = joints[0] * driveRate;
            emit SendMessage(jointsToSend);                                     //通过UDP向大平台发送位置
        }
    } else if (stewartType == Platform::ClassicType) {
        stewart->SetPos(poseCurrent, Platform::ClassicType);
        QVector<double> joints(6);
        if (stewart->GetJoints(joints)) {
            QVector<qreal> jointsToSend = joints;
            for (int index = 1; index <= 6; ++index) {
                jointsToSend[index - 1] = joints[index - 1] * driveRate;
            }
            emit SendMessage(jointsToSend);
        }
    }
}


void SixJoints::on_start_clicked()
{
    static bool isStartMove = true;
    if (isStartMove) {
        ui->start->setText(tr("停止播放"));
        isStartMove = false;
        timerForStart->start(1000 / sampleRate);                                              //! 播放一个姿态的时间
        connect(timerForStart, SIGNAL(timeout()), this, SLOT(Start()));
        ui->record->setEnabled(false);
        ui->pushButtonReset->setEnabled(false);
        ui->groupBoxTCP->setEnabled(false);
    } else {
        ui->start->setText(tr("循环播放"));
        isStartMove = true;
        timerForStart->stop();
        ui->record->setEnabled(true);
        ui->pushButtonReset->setEnabled(true);
        ui->groupBoxTCP->setEnabled(true);
    }
}

void SixJoints::on_record_clicked()
{
    static bool isStartRecode = true;
    if (isStartRecode) {
        ui->record->setText(tr("停止记录"));
        isStartRecode = false;
        timerForRecode->start(1000 / sampleRate);                                             //! 录制一个姿态的时间
        connect(timerForRecode, SIGNAL(timeout()), this, SLOT(Record()));
        ui->start->setEnabled(false);
    } else {
        ui->record->setText(tr("开始记录"));
        isStartRecode = true;
        timerForRecode->stop();
        ui->start->setEnabled(true);
    }
}
void SixJoints::Record()
{
    QFile fileRecordByPose(QString("RecordByPose.csv"));
    QFile fileRecordByJoints(QString("RecordByJoints.csv"));

    if (!fileRecordByPose.open(QIODevice::Append)) {
        return;
    }
    if (!fileRecordByJoints.open(QIODevice::Append)) {
        return;
    }
    QTextStream csvByPose(&fileRecordByPose);
    QTextStream csvByJoints(&fileRecordByJoints);
    csvByPose << poseGoal[0] << ',' << poseGoal[1] << ',' << poseGoal[2] << ','
              << poseGoal[3] << ',' << poseGoal[4] << ',' << poseGoal[5] << '\n';
    if (stewartType == Platform::LinkType) {
        for (int index = 1; index <= 6; ++index) {
            QLabel *label = findChild<QLabel *>(QString("joint%1").arg(index));
            switch (index) {
            case 1:
                csvByJoints << 90 + int(label->text().toDouble()) << ',';
                break;
            case 2:
                csvByJoints << 90 - int(label->text().toDouble()) << ',';
                break;

            case 3:
                csvByJoints << 90 + int(label->text().toDouble()) << ',';
                break;
            case 4:
                csvByJoints << 90 - int(label->text().toDouble()) << ',';
                break;
            case 5:
                csvByJoints << 90 + int(label->text().toDouble()) << ',';
                break;
            case 6:
                csvByJoints << 90 - int(label->text().toDouble()) << '\n';
                break;
            }
        }
    } else if (stewartType == Platform::ClassicType) {
        for (int index = 1; index <= 6; ++index) {
            QLabel *label = findChild<QLabel *>(QString("joint%1").arg(index));
            if (index == 6) {
                csvByJoints << int(label->text().toDouble()) << '\n';
            } else {
                csvByJoints << int(label->text().toDouble()) << ',';
            }
        }
    }

}

void SixJoints::Start()
{
    QFile fileRecode(QString("RecordByPose.csv"));
    if (!fileRecode.isOpen() && !fileRecode.open(QIODevice::ReadOnly)) {
        return;
    }
    static QTextStream csv(&fileRecode);
    QString line;
    csv >> line;
    if (line.isEmpty()) {
        return;
    }
    QStringList strList = line.split(",");
    for(int index = 0; index < 6; index++) {
        poseGoal[index] = strList[index].toDouble();
    }
    for (int index = 1; index <= 6; index++) {
        QSlider *slider = findChild<QSlider *>(QString("verticalSliderTCP%1").arg(index));
        slider->setValue(poseGoal[index - 1]);
    }
}


void SixJoints::on_enableCG27_clicked(bool checked)
{
    isCg27Enable = checked;
}
