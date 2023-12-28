#ifndef CONFIGDOUBLESLIDER_H
#define CONFIGDOUBLESLIDER_H

#include "ConfigControlInterface.h"
#include <QWidget>
#include <QSlider>
#include <QDebug>
#include <QJsonValue>

class ConfigDoubleSlider : public QSlider, public ConfigControlInterface
{
    Q_OBJECT
public:
    explicit ConfigDoubleSlider(QWidget *parent = 0);

public:
    void SetParameterKey(const QString &key);  // 初始化时由外部定义key，和初始value
    void ConnectTo(QObject *obj);   // 用于与外部通信的绑定函数，标签格式为“key， QJsonValue”
    void Disconnect(QObject *obj);  // 用于与外部通信解除绑定的函数，标签格式为“key， QJsonValue”
private:
    void Connect(); // 自身控件发出信号，绑定我们自己的槽函数，实现发自己格式的信号

public slots:
    void ActionValueChanged(const QString &key, const QJsonValue &value);   // 接收外部JsonValue格式的数据槽

private slots:
    void OnValueChanged(int value); // 自身用来发自定义格式信号的槽

signals:
    void ParameterValueChanged(const QString &key, const QJsonValue &value);    // 给外部发送JsonValue格式的数据信号
    void RequestUpdate(const QString &key);

private:
    QString m_key;  // 存储当前控件匹配数据的key

};

#endif // CONFIGDOUBLESLIDER_H
