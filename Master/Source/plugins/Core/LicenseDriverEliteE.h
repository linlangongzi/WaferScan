#pragma once

#include "LicenseDriver.h"

namespace LicenseDriverInternal {

typedef ulong Handle;
typedef QList<ulong> Pin;

struct DongleEliteE {
    DongleEliteE(Handle h) : handle(h) {}
    void refresh();
    QList<LicenseLevel> readLevels(bool isRtl);
    bool reopen(const QString &pin = {});
    bool tryChangePin(const QString &masterKey);

    quint32 deviceType() const;
    QString versionText() const;
    QString deviceDescription(quint32 type) const;
    bool isAuthentic() const;
    QByteArray readData(quint16 offset, quint16 size);
    bool extractUitDevice(const QByteArray &data);

    bool writeData(Handle handle, const QByteArray &data, quint16 offset);
    bool writeLevels(const QList<LicenseLevel> &levels);

    bool operator ==(const DongleEliteE &other) {return handle == other.handle;}

    Handle handle = 0;
    QString masterKey;
    LicenseDevice dev;

    Q_DECLARE_TR_FUNCTIONS(DongleEliteE)
};

}

class LicenseDriverEliteE : public LicenseDriver
{
    Q_OBJECT

public slots:
    virtual LicenseDeviceList enumerate() override;
    virtual QString update(const LicenseDevice &newDevice) override;
    virtual QString recharge(const QString &deviceId, const QString &code) override;
    virtual QString generateRecharge(const QString &deviceId, const LicenseLevel &level) override;

private:
    LicenseDriverInternal::DongleEliteE *findMaster();
    bool removeClosed();

    QList<LicenseDriverInternal::DongleEliteE> dongles;
};
