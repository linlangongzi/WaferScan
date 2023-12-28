#ifndef MATERIAL_H
#define MATERIAL_H

#include <QObject>
#include "ConfigObject/Config.h"

class Material : public Config
{
    Q_OBJECT
    CONFIG_OBJECT

protected:
    explicit Material(QObject *parent = 0);
    virtual ConfigObject *New(QObject *parent) const;

signals:
    void RequestUpdateAnswer(const QString &key, const QJsonValue &value);

public slots:
    void SetParameter(const QString &key, const QJsonValue &value);
    void RequestUpdate(const QString &key);

};

#endif // MATERIAL_H
