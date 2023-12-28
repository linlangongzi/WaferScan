#include <Ultrasonic/UTData.h>
#include "PanelSensorStatus.h"
#include <arsCore/Platform.h>
#include "ui_PanelSensorStatus.h"
#include <Ultrasonic/UTInstrument.h>
#include <ProbeNoManager/ProbeNoManager.h>
#include <QBitmap>
#include <QFileDialog>
#include <Wheel/ControlPanel.h>
#include <allrising/ThrottleUpdate.h>

static const int BITMAP_WIDTH = 18;

PanelSensorStatus::PanelSensorStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelSensorStatus)
{
    ui->setupUi(this);

    sensorLog.setFileName("SensorLog.sensorlog");
    sensorLog.open(QIODevice::WriteOnly);

    InitUI();
    Connect();

    UTSharedDataSimulation simulation = UTSharedDataSimulation::create();
    for (int index = 0; index < 10; ++index) {
        simulation->sLightEye[index] = 1;
    }
    SetRadioButtonList(simulation, true);
}

PanelSensorStatus::~PanelSensorStatus()
{
    delete ui;
}

void PanelSensorStatus::InitUI()
{
    // 将128个手动传感器UI控件添加到List中便于操作
    AddButtonToList(ui->groupBox);
    AddButtonToList(ui->groupBox_2);
    AddButtonToList(ui->groupBox_3);
}

void PanelSensorStatus::onRawMessageArrived(int type, const QVariant &data)
{
    UTInstrument *instrument = qobject_cast<UTInstrument *>(sender());
    if (type == IInstrumentDriver::MessageTrainSensor && instrument) {
        UTSharedDataSimulation simulation = data.value<UTSharedDataSimulation>();

        bool isLeft = (instrument->GetID() == 2);
        if (ui->radioButtonLeft->isChecked()) {
            SetRadioButtonList(simulation, isLeft);
        } else {
            SetRadioButtonList(simulation, !isLeft);
        }

        ProbeNoManager *manager = ProbeNoManager::instance();
        if (ui->autoSwitch->isChecked()) {
            int channel = 0;
            int eyeIndex = -1;

            for (int i = 0; i < NS_PXI100::LIGHT_EYE_VAR_NUM; ++i) {
                quint16 eyesStatus = simulation->sLightEye[i];
                if (eyesStatus == 0) {
                    continue;
                }

                for (int j = 0; j <= 16; ++j) {
                    if ((1 << j) & eyesStatus) {
                        eyeIndex = i * 16 + j;
                        break;
                    }
                }

                if (eyesStatus >= 0) {
                    break;
                }
            }

            if (eyeIndex == -1) {
                return;
            }

            ProbeNoManager::ProbeNoList probeList = manager->EyeArea2ProbeNoList(eyeIndex);
            int index = ui->autoSwitchIndex->value();
            if (index < probeList.count()) {
                channel = probeList.value(index);
            } else {
                return;
            }

            if (channel == -1) {
                return;
            }

            ControlPanel::getInstance()->setCurrentChannel(channel);
        }
    }
}

void PanelSensorStatus::updateBitmap()
{
    QBitmap bitmap = QBitmap::fromData(QSize(BITMAP_WIDTH * 8, data.size() / BITMAP_WIDTH), reinterpret_cast<const uchar *>(data.constData()), QImage::Format_MonoLSB);
    ui->bitmap->setPixmap(bitmap);
}

void PanelSensorStatus::SetRadioButtonList(UTSharedDataSimulation simulation, bool isSet)
{
    if (!isSet) {
        return;
    }

    sensorLog.write(reinterpret_cast<const char *>(simulation->sLightEye), BITMAP_WIDTH);
    data.append(reinterpret_cast<const char *>(simulation->sLightEye), BITMAP_WIDTH);
    if (data.size() > ui->recordLength->value() * BITMAP_WIDTH) {
        data = data.right(ui->recordLength->value() * BITMAP_WIDTH);
    }
    ThrottleUpdate::call(this, &PanelSensorStatus::updateBitmap);

    /* 132个传感器 TCP传输的 LIGHT_EYE_VAR_NUM = 12, 但有用的为9个 */
    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 16; j++) {
            int index = i * 16 + j;
            bool isOn = simulation->sLightEye[i] >> j & 0x1;

            if (index < m_checkBoxList.size()) {
                m_checkBoxList.value(index)->setChecked(isOn);
            }
        }
    }

    bool diff = false;
    for (int i = 0; i < 9; ++i) {
        if (simulation->sLightEye[i] != lastEyesData.sLightEye[i]) {
            diff = true;
            lastEyesData.sLightEye[i] = simulation->sLightEye[i];
        }
    }

    if (diff) {
        QString log;
        for (int i = 0; i < 9; ++i) {                           // 9组有效
            quint16 mark = lastEyesData.sLightEye[i];
            for (int j = 0; j < 16; ++j) {                      // 16位 quint16
                log.append(((mark >> j) & 0x1) ? "1" : "0");    // 保证132位的 左到右 的显示方式
            }

            log.append(" ");
        }

        ui->logSensor->append(log);
    }
}

void PanelSensorStatus::Connect()
{
    for (auto obj: platform()->Instruments()->GetRegistry<UTInstrument>()) {
        connect(obj, &UTInstrument::rawMessageArrived, this, &PanelSensorStatus::onRawMessageArrived);
    }
}

void PanelSensorStatus::AddButtonToList(QWidget *widget)
{
    foreach (QObject *obj, widget->children()) {
        QCheckBox *box = qobject_cast<QCheckBox *>(obj);
        if (!box) {
            continue;
        }
        if (!box->isHidden()) {
            m_checkBoxList << box;
        }
    }
}

void PanelSensorStatus::on_recordOpen_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, QString(), QString(), "*.sensorlog");
    if (path.isEmpty()) {
        return;
    }
    data = ReadFileAsBinary(path);
    updateBitmap();
}
