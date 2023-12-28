#ifndef SIXJOINTS_H
#define SIXJOINTS_H

#include <QDialog>
#include "Platform.h"
#include <QFile>


namespace Ui {
class SixJoints;
}

class SixJoints : public QDialog
{
    Q_OBJECT

public:
    explicit SixJoints(QWidget *parent, Platform::Type type);
    ~SixJoints();
    bool GetJoints(QVector<double> &joints);
    QVector<double> GetPose();
    bool getCg27Status() {return isCg27Enable;}
    void setEnabled(bool value) {enable = value;}

signals:
    void TCPChanged(QVector<qreal> pos);
    void SendDataBySerial(QVector<qreal> joints);
    void SendMessage(QVector<qreal> joints);


public slots:
    void SetPos(double xp, double yp, double zp, double ap, double bp, double cp);
    void SetPos(QVector<double> pos);
    void setSpinBoxPos(QVector<qreal> pos);

private slots:
    void on_pushButtonReset_clicked();
    void onTCPValueChangedFromSpinBox();
    void onTCPValueChangedFromSlider();

    void on_start_clicked();
    void on_record_clicked();
    void Record();
    void Start();

    void on_enableCG27_clicked(bool checked);
    void UpdatePose();

private:
    Ui::SixJoints *ui;
    Platform *stewart;
    Platform::Type stewartType;

    QTimer *timerForRecode;
    QTimer *timerForStart;
    QTimer *timerForInterpolation;
    bool isCg27Enable = false;

    QVector<qreal> poseGoal = QVector<qreal>(6);
    QVector<qreal> poseCurrent = QVector<qreal>(6);
    QVector<qreal> error = QVector<qreal>(6);
    QVector<qreal> error_last = QVector<qreal>(6);
    QVector<qreal> integrate = QVector<qreal>(6);
    QVector<qreal> velocity= QVector<qreal>(6);

    int driveRate = 10000;
    const int sampleRate = 20;                                                  //采样率，单位Hz
    bool enable = false;

};

#endif // SIXJOINTS_H
