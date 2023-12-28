#include "ConfigCombox.h"
#include <QDebug>

ConfigCombox::ConfigCombox(QWidget *parent) :
    QComboBox(parent)
{
    Connect();
}

void ConfigCombox::SetParameterKey(const QString &key)
{
    m_key = key;
    emit RequestUpdate(m_key);
}

void ConfigCombox::ConnectTo(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigCombox::ConnectTo: null pointer");
    }
    connect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ValueChanged(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ParameterLimit(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
    emit RequestUpdate(m_key);
}

void ConfigCombox::Disconnect(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigCombox::Disconnect: null pointer");
    }
    disconnect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ValueChanged(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ParameterLimit(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
}

void ConfigCombox::Connect()
{
    connect(this, SIGNAL(activated(int)), this, SLOT(OnValueChanged(int)));
}

void ConfigCombox::ActionValueChanged(const QString &key, const QJsonValue &value)
{
    if (key == m_key && !hasFocus())
    {
        setCurrentIndex(value.toInt());
    }
}

void ConfigCombox::OnValueChanged(int value)
{
    emit ParameterValueChanged(m_key, value);
}
