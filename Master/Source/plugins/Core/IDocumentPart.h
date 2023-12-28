#pragma once

#include "CoreGlobal.h"
#include "DocumentInfo.h"
#include <functional>
#include <QFile>
#include <QDir>

class CORE_EXPORT IDocumentPart : public QFile
{
    Q_OBJECT

public:
    IDocumentPart(QObject *parent = nullptr);
    IDocumentPart(const QString &name, QObject *parent);

    QJsonValue readJson();
    void writeJson(const QJsonValue &json);

    QDir dir() const;

    virtual bool save();
};

struct DocumentPartFactory
{
    const QMetaObject *meta = &IDocumentPart::staticMetaObject;
    QString typeName = "Part";
    std::function<IDocumentPart *()> create;
};
