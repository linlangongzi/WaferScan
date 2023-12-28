#include "SixJoints.h"
#include "ui_SixJoints.h"
#include "QDoubleSpinBox"
#include "QSlider"

#include <QFile>
const char indexPropertyName[] = "index";

SixJoints::SixJoints(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SixJoints)
{
    ui->setupUi(this);

    //轴控制模式下为每一个slider设置量程及零点
    for (int index = 1; index <= 6; index++) {
        QSlider *slider = findChild<QSlider *>(QString("verticalSliderJ%1").arg(index));
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxJ%1").arg(index));

        slider->setMaximum(sysParas.getMaxJoint()[index - 1]);
        slider->setMinimum(sysParas.getMinJoint()[index - 1]);
        slider->setValue(sysParas.getOriJoint()[index - 1]);

        doubleSpinBox->setMaximum(sysParas.getMaxJoint()[index - 1]);
        doubleSpinBox->setMinimum(sysParas.getMinJoint()[index - 1]);
        doubleSpinBox->setValue(sysParas.getOriJoint()[index - 1]);

        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onJointValueChanged0(double)));
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onJointValueChanged1(int)));
        doubleSpinBox->setProperty(indexPropertyName, index);
        slider->setProperty(indexPropertyName, index);
    }
    //TCP模式下，为每一个slider设置量程及零点
    for (int index = 1; index <= 6; index++) {
        QSlider *slider = findChild<QSlider *>(QString("verticalSliderTCP%1").arg(index));
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(index));

        slider->setMaximum(sysParas.getMaxTCP()[index - 1]);
        slider->setMinimum(sysParas.getMinTCP()[index - 1]);
        slider->setValue(sysParas.getOriTCP()[index - 1]);

        doubleSpinBox->setMaximum(sysParas.getMaxTCP()[index - 1]);
        doubleSpinBox->setMinimum(sysParas.getMinTCP()[index - 1]);
        doubleSpinBox->setValue(sysParas.getOriTCP()[index - 1]);

        connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onTCPValueChanged0()));
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onTCPValueChanged1()));
        doubleSpinBox->setProperty(indexPropertyName, index);
        slider->setProperty(indexPropertyName, index);
    }
    //设置TCP模式不可用
    ui->groupBoxTCP->setEnabled(1);

    pose = QVector<double>(6);
    timerForRecode = new QTimer(this);
    timerForStart = new QTimer(this);

}

SixJoints::~SixJoints()
{
    delete ui;
}
/*!
 * \brief SixJoints::onJointValueChanged        doubleSpinBox引发的轴参数改变。
 *                                              轴参数变化引发的slot，用来手动发射JOINT参数
 * \param value
 */
void SixJoints::onJointValueChanged0(qreal value)
{
    QObject *doubleSpinBox = sender();
    if (!doubleSpinBox) {
        return;
    }
    bool ok = false;
    int index = doubleSpinBox->property(indexPropertyName).toInt(&ok);
    if (!ok) {
        return;
    }
    //同时改变slider的值
    QSlider *slider = findChild<QSlider *>(QString("verticalSliderJ%1").arg(index));
    slider->setValue((int)value);

    QVector<qreal> joints(6);
    char sum = 0;
    for (int index = 1; index <= 6; ++index) {
        QSlider *slider = findChild<QSlider *>(QString("verticalSliderJ%1").arg(index));
        joints[index - 1] = slider->value();
        sum += char(joints[index - 1] + 90);
    }
    pose = joints;
//    qDebug() << "joint pose:" << joints;
    char serialData[] = {0xFF, 0XFE,
                         char(90 + joints[0]), 90 + char(joints[1]),            /*! 下发电机的位置，将-90~90转换到0~180度 */
                         char(90 + joints[2]), 90 + char(joints[3]),            /*! 若电机旋转方向与实际方向不同，将其修改为90-joints[i]即可 */
                         char(90 + joints[4]), 90 + char(joints[5]),
                         char(sum)};
//    qDebug() << "joints:"  << joints << "sum:" << int(sum);
    emit SendDataBySerial(serialData);
}
/*!
 * \brief SixJoints::onJointValueChanged1       slider引发的轴参数改变
 * \param value
 */
void SixJoints::onJointValueChanged1(int value)
{
    QObject *slider = sender();
    if (!slider) {
        return;
    }
    bool ok = false;
    int index = slider->property(indexPropertyName).toInt(&ok);
    if (!ok) {
        return;
    }
    //同时改变slider的值
    QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxJ%1").arg(index));
    doubleSpinBox->setValue((qreal)value);

    //做一个机器人正解，实时刷新TCP的数据
    QVector<qreal> joints(6, 0.0);      //关节角度，从轴模式上读取
    QVector<qreal> TCP(6, 0.0);         //TCP值，经过正解器计算
    for (int index = 1; index <= 6; index++) {
        QSlider *slider = findChild<QSlider *>(QString("verticalSliderJ%1").arg(index));
        joints[index - 1] = slider->value();
    }
    TCP = forwardSolver.getTCP(joints);                                         /*! 调用正解器，计算TCP位置 */
    for (int index = 1; index <= 6; index++) {
        QSlider *slider = findChild<QSlider *>(QString("verticalSliderTCP%1").arg(index));
        slider->setValue(TCP[index - 1]);
    }

}


/*!
 * \brief SixJoints::onTCPValueChanged              由spinBox触发后，更改slider的值，并将TCP发射出去
 */
void SixJoints::onTCPValueChanged0()
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
    QVector<qreal> TCP(6, 0);
    for (int id = 1; id <= 6; id++) {
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(id));
        TCP[id - 1] = doubleSpinBox->value();
    }
}
/*!
 * \brief SixJoints::onTCPValueChanged1             由slider触发后，更改spinbox的值
 */
void SixJoints::onTCPValueChanged1()
{
    //slider的变化引起spinbox的变化
    QObject *slider = sender();
    if (!slider) {
        return;
    }
    bool ok = false;
    int index = slider->property(indexPropertyName).toInt(&ok);
    if (!ok) {
        return;
    }
    QSlider *temp = findChild<QSlider *>(QString("verticalSliderTCP%1").arg(index));
    QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxTCP%1").arg(index));
    doubleSpinBox->setValue((qreal)(temp->value()));
}
/*!
 * \brief SixJoints::on_pushButtonReset_clicked     响应“复位”按键
 */
void SixJoints::on_pushButtonReset_clicked()
{
    for (int index = 1; index <= 6; index++) {
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxJ%1").arg(index));
        doubleSpinBox->setValue(sysParas.getOriJoint()[index - 1]);
    }
}
/*!
 * \brief SixJoints::on_pushButtonOri_clicked       响应“标零”按键
 */
//void SixJoints::on_pushButtonOri_clicked()
//{
//    QVector<qreal> temp(6);
//    for (int index = 1; index <= 6; index++) {
//        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxJ%1").arg(index));
//        temp[index - 1] = doubleSpinBox->value();
//    }
//    sysParas.setOriJoint(temp);
//}


void SixJoints::on_start_clicked()
{
    static bool isStartMove = true;
    if (isStartMove) {
        ui->start->setText(QString("停止播放"));
        isStartMove = false;
        timerForStart->start(1000 / sampleRate);                                              //! 播放一个姿态的时间
        connect(timerForStart, SIGNAL(timeout()), this, SLOT(Start()));
        ui->record->setEnabled(false);
        ui->pushButtonReset->setEnabled(false);
        ui->groupBoxTCP->setEnabled(false);
    } else {
        ui->start->setText(QString("循环播放"));
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
        ui->record->setText(QString("停止记录"));
        isStartRecode = false;
        timerForRecode->start(1000 / sampleRate);                                             //! 录制一个姿态的时间
        connect(timerForRecode, SIGNAL(timeout()), this, SLOT(Record()));
        ui->start->setEnabled(false);
    } else {
        ui->record->setText(QString("开始记录"));
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
//    QTextStream csv(&recodeFile);
    csvByPose << pose[0] << ',' << pose[1] << ',' << pose[2] << ','
              << pose[3] << ',' << pose[4] << ',' << pose[5] << '\n';
    for (int index = 1; index <= 6; ++index) {
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxJ%1").arg(index));
        if (index == 6) {
            csvByJoints << doubleSpinBox->value() << '\n';
        } else {
            csvByJoints << doubleSpinBox->value() << ',';
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
        pose[index] = strList[index].toDouble();
    }
    for (int index = 1; index <= 6; index++) {
        QDoubleSpinBox *doubleSpinBox = findChild<QDoubleSpinBox *>(QString("doubleSpinBoxJ%1").arg(index));
        doubleSpinBox->setValue(pose[index - 1]);
    }

}

