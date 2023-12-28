#include "DialogConnect.h"
#include "ui_DialogConnect.h"
#include "InstrumentConnection.h"
#include <Ultrasonic/UltrasonicManager.h>

DialogConnect::DialogConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConnect)
{
    ui->setupUi(this);

    connect(UltrasonicManager::instance(), &UltrasonicManager::instrumentsChanged, this, &DialogConnect::RefreshInstrument);
    RefreshInstrument();
}

DialogConnect::~DialogConnect()
{
    delete ui;
}

/*! 初始化 */
void DialogConnect::RefreshInstrument()
{
    qDeleteAll(findChildren<InstrumentConnection *>());
    // 仪器
    for (UTInstrument *instrument: UltrasonicManager::instruments()) {
        auto *instrumentConnect = new InstrumentConnection(instrument, this);
        int index = ui->InstrumentLayout->count();
        ui->InstrumentLayout->insertWidget(index - 1, instrumentConnect);
    }
}
