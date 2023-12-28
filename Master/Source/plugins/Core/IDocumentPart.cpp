#include "IDocument.h"
#include "IDocumentPart.h"

IDocumentPart::IDocumentPart(QObject *parent) :
    QFile(parent)
{
}

IDocumentPart::IDocumentPart(const QString &name, QObject *parent) :
    QFile(name, parent)
{
}

bool IDocumentPart::save()
{
    return true;
}

QJsonValue IDocumentPart::readJson()
{
    reset();
    return ByteArrayToJson(readAll());
}

void IDocumentPart::writeJson(const QJsonValue &json)
{
    resize(0);
    reset();
    write(JsonToByteArray(json));
}

QDir IDocumentPart::dir() const
{
    auto *doc = qobject_cast<IDocument *>(parent());
    if (doc) {
        return doc->dir();
    }
    return QDir();
}
