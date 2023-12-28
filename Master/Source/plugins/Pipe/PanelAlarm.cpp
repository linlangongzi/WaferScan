#include "PanelAlarm.h"
#include "ui_PanelAlarm.h"
#include "AlarmGroupFrame.h"
#include "PlotAlarmGroupEdit.h"
#include <Ars1000/PipeDataDef.h>
#include <Core/Platform.h>
#include <QComboBox>
#include <QMessageBox>
#include <Ultrasonic/UTChannel.h>

PanelAlarm::PanelAlarm(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PanelAlarm)
{
    ui->setupUi(this);
}

PanelAlarm::~PanelAlarm()
{
    delete ui;
}

void PanelAlarm::OnCurChannelChanged(qint32 index)
{
    qint32 ascanIndex = ui->comboBoxChannelList->currentIndex();
    emit CurChannelChanged(ascanIndex, index);
}

void PanelAlarm::setAlarmGroups(const QList<PlotAlarmGroup> &alarmGroups)
{
    m_alarmGroups = alarmGroups;
    refreshControls();
}

/*!
 * \brief PanelAlarm::AddAScan
 * \param name:A扫名称
 * 在报警列表中添加A扫列表选择条目
 */
void PanelAlarm::AddAScan(QString name)
{
    ui->comboBoxChannelList->addItem(name);
}

void PanelAlarm::RemoveAScan(int index)
{
    ui->comboBoxChannelList->removeItem(index);
}

/*!
 * \brief PanelAlarm::GetCurAScanIndex
 * \return 获取当前时第几个A扫 0开始
 */
qint32 PanelAlarm::GetCurAScanIndex() const
{
    return ui->comboBoxChannelList->currentIndex();
}

qint32 PanelAlarm::GetAScanCount() const
{
    return ui->comboBoxChannelList->count();
}

void PanelAlarm::ComboBoxClear()
{
    ui->comboBoxChannelList->clear();
}

void PanelAlarm::setAlarms(const QList<PlotAlarmItem> &alarms)
{
    for (auto *frame: m_frames) {
        frame->setAlarms(alarms);
    }
}

QList<PlotAlarmGroup> PanelAlarm::alarmGroups() const
{
    return m_alarmGroups;
}

TestResultType PanelAlarm::JudgeResult()
{
    for (auto frame : m_frames) {
        if (frame->JudgeResult() == TestResultType::Reject) {
            return TestResultType::Reject;
        }
    }

    return TestResultType::Accept;
}

void PanelAlarm::refreshControls()
{
    resizeContainer(m_alarmGroups.count(), m_frames, [=]() {
        auto *frame = new AlarmGroupFrame(this);
        ui->mainLayout->addWidget(frame);
        connect(frame, &AlarmGroupFrame::optionChanged, this, [=](PlotAlarmGroup group) {
            int index = m_frames.indexOf(frame);
            m_alarmGroups[index] = group;
        });
        return frame;
    }, [](AlarmGroupFrame *frame) {
        delete frame;
    });

    for (int index = 0; index < m_alarmGroups.count(); ++index) {
        m_frames[index]->setOption(m_alarmGroups[index]);
        m_frames[index]->ConnectTo(this);
    }
}

void PanelAlarm::on_add_clicked()
{
    PlotAlarmGroupEdit dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        m_alarmGroups << dialog.option();
        refreshControls();
    }
}
