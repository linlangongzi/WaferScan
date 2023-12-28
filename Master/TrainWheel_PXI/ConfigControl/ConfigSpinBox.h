#ifndef CONFIGSPINBOX_H
#define CONFIGSPINBOX_H

#include "ConfigControlInterface.h"
#include <QSpinBox>

class ConfigSpinBox : public QSpinBox, public ConfigControlInterface
{
    Q_OBJECT
public:
    explicit ConfigSpinBox(QWidget *parent = 0);

public:
    void SetParameterKey(const QString &key);
    void ConnectTo(QObject *obj);
    void Disconnect(QObject *obj);

private:
    void Connect();

public slots:
    void ActionValueChanged(const QString &key, const QJsonValue &value);

private slots:
    void OnValueChanged(int value);

signals:
    void ParameterValueChanged(const QString &key, const QJsonValue &value);
    void RequestUpdate(const QString &key);

private:
    QString m_key;

};

#endif // CONFIGSPINBOX_H
