#pragma once

#include <QObject>
#include <QJsonObject>

class Platform : public QObject
{
    Q_OBJECT

public:
    void Init();                                                                /* 平台初始化 */

public:
    /* 单例管理、析构管理和包装 */
    explicit Platform(QObject *parent = nullptr);
    ~Platform() override;

    /*! 获取 JsonObject 表中的一个值 */
    QJsonValue Get(const QString &key) const;

    /* 导入和导出 */
    QJsonObject ToJson() const;

public slots:
    /* 设置JsonObject表中的一个值 */
    void Set(const QString &key, const QJsonValue &value);

signals:
    void ValueChanged(const QString &key, const QJsonValue &value);

public:
    /* 从Json导入 */
    void FromJson(const QJsonObject &source);

private:
    QJsonObject data;
};

/* 全局获取platform方法，为GetInstance缩写 */
Platform *platform();
