#include "ConfigDoubleSpinBox.h"
#include <QDebug>

ConfigDoubleSpinBox::ConfigDoubleSpinBox(QWidget *parent):
    QDoubleSpinBox(parent)
{
    Connect();
    this->setMouseTracking(true);
}

void ConfigDoubleSpinBox::Connect()
{
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));
}

void ConfigDoubleSpinBox::focusInEvent(QFocusEvent *event)
{
    QDoubleSpinBox::focusInEvent(event);
}

void ConfigDoubleSpinBox::focusOutEvent(QFocusEvent *event)
{
    emit FocusLeave(m_key, m_value);
    QDoubleSpinBox::focusOutEvent(event);
}

void ConfigDoubleSpinBox::ConnectTo(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigDoubleSpinBox::ConnectTo: null pointer");
    }
    connect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ValueChangedSetup(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(obj, SIGNAL(ParameterLimit(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    connect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
    connect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    emit RequestUpdate(m_key);
}

void ConfigDoubleSpinBox::Disconnect(QObject *obj)
{
    if (obj == NULL)
    {
        return qCritical("ConfigDoubleSpinBox::Disconnect: null pointer");
    }
    disconnect(this, SIGNAL(ParameterValueChanged(const QString &, const QJsonValue &)), obj, SLOT(SetParameter(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ValueChangedSetup(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(obj, SIGNAL(ParameterLimit(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
    disconnect(this, SIGNAL(RequestUpdate(QString)), obj, SLOT(RequestUpdate(QString)));
    disconnect(obj, SIGNAL(RequestUpdateAnswer(const QString &, const QJsonValue &)), this, SLOT(ActionValueChanged(const QString &, const QJsonValue &)));
}

void ConfigDoubleSpinBox::ActionValueChanged(const QString &key, const QJsonValue &value)
{
    if (key == m_key)
    {
        if (qFuzzyCompare(m_value, value.toDouble()))
        {
            return;
        }

        m_value = value.toDouble();
        setValue(value.toDouble());
    }
}

void ConfigDoubleSpinBox::SetParameterKey(const QString &key)
{
    m_key = key;
    emit RequestUpdate(m_key);
}

void ConfigDoubleSpinBox::OnValueChanged(double value)
{
    m_value = value;
    emit ParameterValueChanged(m_key, value);
}
