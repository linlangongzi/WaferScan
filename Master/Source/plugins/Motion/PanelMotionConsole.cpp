#include "MotionManager.h"
#include "PanelMotionConsole.h"
#include "ui_PanelMotionConsole.h"
#include "Motion/TrioDriver.h"
#include "Motion/TrioStream.h"
#include <QKeyEvent>
#include <QMenu>
#include <QToolButton>

using Motion::TrioDriver;

PanelMotionConsole::PanelMotionConsole(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelMotionConsole)
{
    ui->setupUi(this);

    ui->command->installEventFilter(this);
    connect(MotionManager::driver(), &TrioDriver::channelDataArrived, this, &PanelMotionConsole::OnDataArrived);

    /* 创建过滤器菜单 */
    auto *filterMenu = new QMenu(ui->filter);
    for (int index = 0; index < Motion::ChannelCount + 1; ++index) {
        QString name = QChar(Motion::channelNames[index]);
        ui->channel->addItem(name);
        QAction *action = filterMenu->addAction(name);
        action->setCheckable(true);
        action->setChecked(index != Motion::ChannelProject);                    /* 默认启用除管理通道之外的所有消息 */
        filters << action;
    }
    filters.last()->setText("Matlab");
    ui->filter->setMenu(filterMenu);
}

PanelMotionConsole::~PanelMotionConsole()
{
    delete ui;
}

bool PanelMotionConsole::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != ui->command || event->type() != QEvent::KeyPress) {
        return false;
    }
    auto *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() != Qt::Key_Return) {
        return false;
    }
    QString command = ui->command->currentText();
    if (!command.isEmpty()) {
        if (ui->channel->currentText() == "Matlab") {
//            const QString ret = platform()->Matlab()->DoString(command);
//            ui->history->appendPlainText(">> " + command);
//            ui->history->appendPlainText(ret);
        } else if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
            MotionManager::driver()->sendRaw(command.trimmed());
        } else {
            MotionManager::driver()->execute(command.trimmed(), ui->channel->currentIndex());
        }
        int index = ui->command->findText(command);
        if (index != -1) {
            ui->command->removeItem(index);
        }
        ui->command->addItem(command);
        ui->command->clearEditText();
    }
    return true;
}

void PanelMotionConsole::OnDataArrived(int channel, const QString &data)
{
//    qDebug() << "Console: " << channel << data;
    QAction *action = filters.value(channel);
    if (action && action->isChecked()) {
        ui->history->appendPlainText(action->text() + ": " + data.trimmed());
    }
}

void PanelMotionConsole::on_clear_clicked()
{
    ui->history->clear();
    ui->command->clear();
}

void PanelMotionConsole::on_sensorDebug_toggled(bool checked)
{
    Motion::TrioDriver *driver = MotionManager::driver();
    Motion::ControllerConfig config = driver->config();
    if (10 < config.ioConfig.size()) {                                          // 10 为w轴传感器序号
        config.ioConfig[10].inverted = checked;
        driver->setConfig(config);
    }
}
