#pragma once

#include <QObject>
#include <QSettings>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    using Callback = std::function<void()>;

    explicit SettingsManager(QObject *parent = 0);

    void beginGroup(const QString &prefix);
    void endGroup();

    void beginReadArray(const QString &prefix);
    void beginWriteArray(const QString &prefix, int size = -1);
    void endArray();

    bool contains(const QString &key) const;
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    template <typename T>
    T value(const QString &key, const T &defaultValue = T()) const;

    void subscribeValueChange(QObject *obj, const QString &key, Callback func);
    template <typename Obj, typename Func>
    void subscribeGroupChange(Obj *obj, const QString &group, Func func);
    void subscribeGroupChange(QObject *obj, const QString &group, Callback func);

    void setSettings(QSettings *settings);

signals:
    void valueChanged(const QString &key);

private slots:
    void objectDestroyed(QObject *obj);

private:
    struct Subscription
    {
        QObject *owner = Q_NULLPTR;
        QString key;
        Callback func;
    };
    QSettings *m_settings = Q_NULLPTR;
    QList<Subscription> m_keySubscription;
    QList<Subscription> m_groupSubscription;
//    QSet<Subscription> m_pendingSubscription;
};

//uint qHash(const SettingsManager::Subscription &sub);

template <typename T>
T SettingsManager::value(const QString &key, const T &defaultValue) const
{
    return value(key, QVariant::fromValue(defaultValue)).template value<T>();
}
