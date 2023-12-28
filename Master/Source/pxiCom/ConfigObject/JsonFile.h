#pragma once

#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QDebug>
#include "ConfigObject/Config.h"

class JsonFile : public Config
{
    Q_OBJECT
public:
    explicit JsonFile(QObject *parent = 0);

    void setFileName(const QString &name);
    bool open();
    void close();

public slots:
//    void WriteJson(QJsonArray array);
    void WriteJson(QJsonArray array);

protected:
    virtual ConfigObject *New(QObject* parent) const;

private:
    QString fileName;
};
