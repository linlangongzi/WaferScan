#pragma once

#include <QWidget>
#include "LicenseDevice.h"

namespace Ui {
class LicenseEditor;
}

class QTreeWidgetItem;

class LicenseEditor : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool readOnly READ IsReadOnly WRITE SetReadOnly)
    Q_PROPERTY(LicenseDevice device READ GetDevice WRITE SetDevice)

public:
    explicit LicenseEditor(QWidget *parent = 0);
    ~LicenseEditor();

    bool IsReadOnly() const;
    LicenseDevice GetDevice() const;
    LicenseLevel GetLevel(int index) const;

signals:
    void RequestRecharge(const QString &code);
    void RequestUpdate();
    void RequestgenerateRecharge(QString deviceId, QString targetPin, LicenseLevel level);

public slots:
    void SetReadOnly(bool newReadOnly);
    void SetDevice(const LicenseDevice &newDevice);

protected:
    virtual void timerEvent(QTimerEvent *);
    virtual bool eventFilter(QObject *watch, QEvent *event);

private slots:
    void on_addLevel_clicked();
    void on_removeLevel_clicked();
    void on_levels_itemChanged(QTreeWidgetItem *item, int column);
    void on_recharge_clicked();
    void on_update_clicked();
    void on_editConfig_clicked();
    void on_generateRecharge_clicked();

private:
    void UpdateTime();
    void UpdateReadonlyControls();

private:
    Ui::LicenseEditor *ui;
    bool readOnly = false;
    LicenseDevice device;

    bool isUpdating = false;
};
