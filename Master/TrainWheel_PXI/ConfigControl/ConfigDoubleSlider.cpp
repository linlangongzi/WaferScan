#include "ConfigDoubleSlider.h"

ConfigDoubleSlider::ConfigDoubleSlider(QWidget *parent) :
    QSlider(parent)
{
    Connect();
}

void ConfigDoubleSlider::SetParameterKey(const QString &key)
{
    m_key = key;
    emit RequestUpdate(m_key);
}

void ConfigDoubleSlider::ConnectTo(QObject *obj)
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

void ConfigDoubleSlider::Disconnect(QObject *obj)
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

void ConfigDoubleSlider::Connect()
{
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(OnValueChanged(int)));
}

void ConfigDoubleSlider::ActionValueChanged(const QString &key, const QJsonValue &value)
{
    if (key == m_key && !hasFocus())
    {
        setValue(static_cast<int>(value.toDouble() * 100));
    }
}

void ConfigDoubleSlider::OnValueChanged(int value)
{
    double dValue = static_cast<double>(value * 0.01);
    QString strValue = QString::number(dValue, '.', 2);
    emit ParameterValueChanged(m_key, strValue.toDouble());
}
