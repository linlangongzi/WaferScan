#pragma once

#include <QWidget>
#include <Ultrasonic/UTSetup.h>

class UTSetup;
class ConfigLabel;

namespace Ui {
class UTSetupSelector;
}

class UTSetupSelector : public QWidget
{
    Q_OBJECT

public:
    explicit UTSetupSelector(QWidget *parent = 0);
    ~UTSetupSelector();

    UTSetup *GetUTSetup() const;
    ConfigID GetUTSetupID() const;
    QJsonValue GetUTSetupValue(const QString &key) const;

    void SetUTSetup(UTSetup *setup);
    void SetUTSetup(ConfigID id);

    void AddDisplayKey(const QString &key, const QString &name);

private slots:
    void RefreshList();
    void RefreshSelection();
    void OnValueChanged(const QString &key, const QJsonValue &value);
    void on_selectUTSetup_currentIndexChanged(int index);

signals:
    void UTSetupValueChanged(const QString &key, const QJsonValue &value);

private:
    QList<ConfigLabel *> labels;
    QPointer<UTSetup> bindUTSetup;
    Ui::UTSetupSelector *ui;
};
