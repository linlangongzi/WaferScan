#include "AlarmGroupFrame.h"
#include "PlotAlarmGroupEdit.h"
#include "ui_AlarmGroupFrame.h"

AlarmGroupFrame::AlarmGroupFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AlarmGroupFrame)
{
    ui->setupUi(this);
    ui->name->installEventFilter(this);
}

AlarmGroupFrame::~AlarmGroupFrame()
{
    delete ui;
}

TestResultType AlarmGroupFrame::JudgeResult()
{
    for (auto label : m_labels) {
        if (label->JudgeResult() == TestResultType::Reject) {
            return TestResultType::Reject;
        }
    }

    return TestResultType::Accept;
}

PlotAlarmGroup AlarmGroupFrame::option() const
{
    return m_option;
}

void AlarmGroupFrame::ConnectTo(QObject *obj)
{
    for (AlarmLabel *label : m_labels) {
        if (obj) {
            connect(label, SIGNAL(CurrentChannelIndex(qint32)), obj, SLOT(OnCurChannelChanged(qint32)));
        }
    }
}

void AlarmGroupFrame::setOption(const PlotAlarmGroup &option)
{
    m_option = option;
    ui->name->setText(option.name);

    resizeContainer(option.indicators.count(), m_labels, [=]() {
        auto *label = new AlarmLabel(this);
        ui->mainLayout->addWidget(label);
        return label;
    }, [](AlarmLabel *label) {
        delete label;
    });

    for (int index = 0; index < m_labels.count(); ++index) {
        AlarmLabel *label = m_labels[index];
        label->setOption(option.indicators.at(index));
        label->setColor(option.inactiveColor, option.activeColor);
    }

    emit optionChanged(option);
}

void AlarmGroupFrame::setAlarms(const QList<PlotAlarmItem> &alarms)
{
    QList<PlotAlarmItem> filtered;
    if (m_option.featureIndex == -1) {
        filtered = alarms;
    } else {
        for (const auto &alarm: alarms) {
            if (alarm.featureIndex == alarm.featureIndex) {
                filtered << alarm;
            }
        }
    }
    for (auto *label: m_labels) {
        label->setAlarms(filtered);
    }
}

bool AlarmGroupFrame::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == ui->name && e->type() == QEvent::MouseButtonDblClick) {
        PlotAlarmGroupEdit dialog(this);
        dialog.setOption(m_option);
        if (dialog.exec() == QDialog::Accepted) {
            setOption(dialog.option());
        }
    }
    return QFrame::eventFilter(obj, e);
}
