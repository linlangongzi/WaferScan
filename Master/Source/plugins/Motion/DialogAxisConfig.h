#pragma once

#include <QDialog>
#include <QPointer>
//class QLabel;

//class DigitalIndicator : public QObject
//{
//    Q_OBJECT
//public:
//    void DigitalIndicator(QLabel *label);

//public slots:
//    void UpdateStatus(int index, bool enabled);
//    void SetIndex(int newIndex);

//private:
//    int index;
//};

namespace Ui {
class DialogAxisConfig;
}
class QAbstractButton;
class Axis;
namespace Motion{
class ControllerStatus;
}

class DialogAxisConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAxisConfig(QWidget *parent = 0);
    ~DialogAxisConfig();
    void loadAxis(Axis *newAxis);

public slots:
    virtual int exec();

private slots:
    void addSensor(const QString &name, int id);
    void onLicenseUpdated();
    void on_buttonBox_accepted();
    void on_forwardKeyReset_clicked();
    void on_reverseKeyReset_clicked();
    void on_stop_clicked();
    void on_forward_clicked();
    void on_reverse_clicked();
    void on_moveabsGo_clicked();
    void on_moveReverse_clicked();
    void on_moveForward_clicked();
    void on_buttonBox_rejected();
    void apply();

    void on_speedMax_editingFinished();
    void on_accelMax_editingFinished();
    void on_decelMax_editingFinished();
    void on_forwardLimit_editingFinished();
    void on_sensorAdd_clicked();
    void on_sensorDelete_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_licenseManualRobotics_clicked();

private:
    void initUi(const Motion::ControllerStatus &status);

private:
    Ui::DialogAxisConfig *ui;
    QPointer<Axis> axis;
};
