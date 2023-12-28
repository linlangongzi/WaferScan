#ifndef CONFIGDOUBLESPINBOX_H
#define CONFIGDOUBLESPINBOX_H

#include "ConfigControlInterface.h"
#include <QDoubleSpinBox>
#include <QFocusEvent>

class ConfigDoubleSpinBox : public QDoubleSpinBox, public ConfigControlInterface
{
    Q_OBJECT
public:
    ConfigDoubleSpinBox(QWidget *parent = 0);

public:
    void SetParameterKey(const QString &key);
    void ConnectTo(QObject *obj);
    void Disconnect(QObject *obj);

private:
    void Connect();
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
public slots:
    void ActionValueChanged(const QString &key, const QJsonValue &value);

private slots:
    void OnValueChanged(double value);

signals:
    void ParameterValueChanged(const QString &key, const QJsonValue &value);
    void RequestUpdate(const QString &key);
    void FocusLeave(const QString &key, const QJsonValue &value);

private:
    QString m_key;
    double m_value;
};

#endif // CONFIGDOUBLESPINBOX_H
