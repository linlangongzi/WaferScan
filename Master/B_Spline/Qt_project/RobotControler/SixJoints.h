#ifndef SIXJOINTS_H
#define SIXJOINTS_H

#include <QDialog>
#include <QTimer>

#include "SysParas.h"
#include "ForwardSolver.h"


namespace Ui {
class SixJoints;
}

class SixJoints : public QDialog
{
    Q_OBJECT

public:
    explicit SixJoints(QWidget *parent = 0);
    ~SixJoints();

signals:
    void SendDataBySerial(const char *data);                             //调用正解器，计算TCP位置

private slots:
    void onJointValueChanged0(qreal value);                                     //轴位置参数被改变
    void onJointValueChanged1(int value);
    void onTCPValueChanged0();                                                  //TCP位置改变
    void onTCPValueChanged1();

    void on_pushButtonReset_clicked();
//    void on_pushButtonOri_clicked();

    void on_start_clicked();
    void on_record_clicked();
    void Record();
    void Start();

private:
    Ui::SixJoints *ui;
    SysParas sysParas;
    ForwardSolver forwardSolver;                                                //正解器

    QTimer *timerForRecode;
    QTimer *timerForStart;
    QVector<double> pose;
    const int sampleRate = 20;                                                  //采样率，单位Hz

};

#endif // SIXJOINTS_H
