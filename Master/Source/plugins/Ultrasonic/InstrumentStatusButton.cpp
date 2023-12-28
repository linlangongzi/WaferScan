#include "InstrumentStatusButton.h"
#include "DialogConnect.h"
#include "UltrasonicManager.h"
#include <Core/LicenseManager.h>

InstrumentStatusButton::InstrumentStatusButton(QWidget *parent) :
    QToolButton(parent)
{
    setAutoRaise(true);
    connect(this, &QToolButton::clicked, this, &InstrumentStatusButton::onClicked);

    connect(UltrasonicManager::instance(), &UltrasonicManager::instrumentsChanged, this, &InstrumentStatusButton::refresh);
    refresh();
}

void InstrumentStatusButton::refresh()
{
    int connected = 0;
    const auto instruments = UltrasonicManager::instruments();
    for (UTInstrument *instrument: instruments) {
        if (instrument->instrumentStatus().state == InstrumentStatus::Connected) {
            connected++;
        }
        connect(instrument, &UTInstrument::instrumentStatusChanged, this, &InstrumentStatusButton::refresh, Qt::UniqueConnection);
    }
    const int total = instruments.count();
    QPalette palette;
    palette.setColor(QPalette::ButtonText, (connected >= total) ? Qt::darkGreen : LicenseManager::instance()->hasProductLicense()? Qt::red : Qt::darkGray);
    setText(tr("超声仪器: %1 / %2").arg(connected).arg(total));
    setEnabled(LicenseManager::instance()->hasProductLicense());
    setPalette(palette);
}

void InstrumentStatusButton::onClicked()
{
    DialogConnect dialog(this);
    dialog.exec();
}
