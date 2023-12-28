#ifndef PANELSENSORSTATUS_H
#define PANELSENSORSTATUS_H

#include <QWidget>
#include "UTInstrumentDriver/PXI100/DataDef.h"
#include <QCheckBox>
#include <QFile>
#include <QLabel>

namespace Ui {
class PanelSensorStatus;
}

class PanelSensorStatus : public QWidget
{
    Q_OBJECT

public:
    explicit PanelSensorStatus(QWidget *parent = 0);
    ~PanelSensorStatus();
    void SetRadioButtonList(UTSharedDataSimulation simulation, bool isSet);

public slots:
    void onRawMessageArrived(int type, const QVariant &data);

private slots:
    void updateBitmap();
    void on_recordOpen_clicked();

private:
    void InitUI();
    void Connect();
    void AddButtonToList(QWidget *widget);

private:
    typedef QList<QCheckBox *> CheckBoxList;
    typedef QList<QLabel *> LableList;

    Ui::PanelSensorStatus *ui;
    CheckBoxList m_checkBoxList;
    LableList m_labelList;
    NS_PXI100::SIMULATION_DATA_t lastEyesData;
    QByteArray data;
    QFile sensorLog;
    bool openLog;

};

#endif // PANELSENSORSTATUS_H
