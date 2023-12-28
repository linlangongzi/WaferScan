#include "ConfigSlider.h"
#include <QDebug>

ConfigSlider::ConfigSlider(QWidget *parent) :
    QSlider(parent)
{
    Connect();
}

void ConfigSlider::Connect()
{
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(OnValueChanged(int)));
}

void ConfigSlider::ConnectTo(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigSlider::ConnectTo: null pointer");
    }
    connect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ValueChanged(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ParameterLimit(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
    emit RequestUpdate(m_key);
}

void ConfigSlider::Disconnect(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigSlider::Disconnect: null pointer");
    }
    disconnect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ValueChanged(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ParameterLimit(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
}

void ConfigSlider::ActionValueChanged(const QString &key, const QJsonValue &value)
{
    if (key == m_key && !hasFocus())
    {
        setValue(value.toInt());
    }
}

void ConfigSlider::SetParameterKey(const QString &key)
{
    m_key = key;
    emit RequestUpdate(m_key);
}

void ConfigSlider::OnValueChanged(int value)
{
    emit ParameterValueChanged(m_key, value);
}
