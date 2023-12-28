#include "ConfigSpinBox.h"
#include <QDebug>

ConfigSpinBox::ConfigSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
    Connect();
}

void ConfigSpinBox::Connect()
{
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(OnValueChanged(int)));
}

void ConfigSpinBox::ConnectTo(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigSpinBox::ConnectTo: null pointer");
    }
    connect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ValueChanged(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ParameterLimit(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
    emit RequestUpdate(m_key);
}

void ConfigSpinBox::Disconnect(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigSpinBox::Disconnect: null pointer");
    }
    disconnect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ValueChanged(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ParameterLimit(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
}

void ConfigSpinBox::ActionValueChanged(const QString &key, const QJsonValue &value)
{
    if (key == m_key)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  if (key == m_key)
    {
        setValue(value.toInt());       
    }
}

void ConfigSpinBox::SetParameterKey(const QString &key)
{
    m_key = key;
    emit RequestUpdate(m_key);
}

void ConfigSpinBox::OnValueChanged(int value)
{
    emit ParameterValueChanged(m_key, value);
}
