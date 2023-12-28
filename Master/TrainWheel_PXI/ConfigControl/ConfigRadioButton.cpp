#include "ConfigRadioButton.h"
#include <QDebug>

ConfigRadioButton::ConfigRadioButton(QWidget *parent) :
    QRadioButton(parent)
{
    Connect();
}

void ConfigRadioButton::Connect()
{
    connect(this, SIGNAL(toggled(bool)), this, SLOT(OnValueChanged(bool)));
}

void ConfigRadioButton::ConnectTo(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigRadioButton::ConnectTo: null pointer");
    }
    connect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ValueChangedSetup(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
    emit RequestUpdate(m_key);
}

void ConfigRadioButton::Disconnect(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigRadioButton::Disconnect: null pointer");
    }
    disconnect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ValueChangedSetup(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
}

void ConfigRadioButton::ActionValueChanged(const QString &key, const QJsonValue &value)
{
    if (key == m_key && value == m_base)
    {
        setChecked(true);
        emit ParameterValueChanged(m_key, m_base);
    }
}

void ConfigRadioButton::SetParameterKey(const QString &key, const QJsonValue &value)
{
    m_key = key;
    m_base = value;
    emit RequestUpdate(m_key);
}

void ConfigRadioButton::OnValueChanged(bool bChecked)
{
    if (bChecked)
    {
        emit ParameterValueChanged(m_key, m_base);
    }
}
