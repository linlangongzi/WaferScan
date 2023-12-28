#include "ConfigCheckBox.h"

ConfigCheckBox::ConfigCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    Connect();
}

void ConfigCheckBox::SetParameterKey(const QString &key)
{
    m_key = key;
    emit RequestUpdate(m_key);
}

void ConfigCheckBox::ConnectTo(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigCheckBox::ConnectTo: null pointer");
    }
    connect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ValueChanged(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));

    connect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
    emit RequestUpdate(m_key);
}

void ConfigCheckBox::Disconnect(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigCheckBox::Disconnect: null pointer");
    }
    disconnect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ValueChanged(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
}

void ConfigCheckBox::Connect()
{
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(OnValueChanged(int)));
}

void ConfigCheckBox::ActionValueChanged(const QString &key, const QJsonValue &value)
{
    bool checkState = value.toBool();
    if ("DACEnable" == key)
    {
        if (checkState)
        {
            setCheckState(Qt::Checked);
        }
        else
        {
            setCheckState(Qt::Unchecked);
        }
        emit ParameterValueChanged(key, checkState);
    }
}

void ConfigCheckBox::OnValueChanged(int nCheck)
{
    int checkState = nCheck;
    switch(checkState)
    {
    case 0:
        emit ParameterValueChanged(m_key, false);
        break;
    case 1:
        emit ParameterValueChanged(m_key, true);
        break;
    case 2:
        emit ParameterValueChanged(m_key, true);
        break;
    default:
        break;
    }

}
