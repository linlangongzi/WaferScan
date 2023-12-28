#include "InstrumentConnection.h"
#include "ui_InstrumentConnection.h"

InstrumentConnection::InstrumentConnection(UTInstrument *instrument, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentConnection),
    instrument(instrument)
{
    ui->setupUi(this);

    OnConnectionChanged(instrument->instrumentStatus());
    connect(instrument, &UTInstrument::instrumentStatusChanged, this, &InstrumentConnection::OnConnectionChanged);
    ui->ip->setText(instrument->config().address);
}

InstrumentConnection::~InstrumentConnection()
{
    delete ui;
}

/*! 仪器连接状态改变的槽函数 */
void InstrumentConnection::OnConnectionChanged(InstrumentStatus status)
{
    switch (status.state) {
    case InstrumentStatus::Connected:       ui->connect->setText(tr("已连接")); break;
    case InstrumentStatus::Disconnected:    ui->connect->setText(tr("连接")); break;
//    case ConnectionStatus::SYNC_ERROR:      ui->connect->setText( tr("SyncError") );
    case InstrumentStatus::Initializing:    ui->connect->setText(tr("正在连接...")); break;
    default:    break;
    }
    ui->connect->setEnabled(status.state == InstrumentStatus::Disconnected);
}

void InstrumentConnection::on_connect_clicked()
{
    if (instrument) {
        instrument->Connect();
    }
}

void InstrumentConnection::on_disconnect_clicked()
{
    if (instrument) {
        instrument->Disconnect();
    }
}

void InstrumentConnection::on_reset_clicked()
{
    if (instrument) {
        instrument->resetPose();
    }
}
