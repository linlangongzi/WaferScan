#pragma once

#include "CoreGlobal.h"
#include <QFileSystemModel>
#include <QObject>
#include <Core/DocumentInfo.h>

class IDocument;

class CORE_EXPORT DocumentManager : public QObject
{
    Q_OBJECT

public:
    explicit DocumentManager(QObject *parent = nullptr);
    ~DocumentManager();

    static DocumentManager *instance();

    static bool hasCurrentDocument();
    static IDocument *currentDocument();
    static QDir currentDocumentDir();
    static QFileSystemModel *currentDocumentModel();

    static void setCurrentDocument(const QString &path);
    static bool createDocument(const QString &path, DocumentInfo info = DocumentInfo(), QString *errorString = nullptr);
    static bool isValidDocument(const QString &path);

signals:
    void currentDocumentChanged();

private:
    QFileSystemModel docModel;
    IDocument *document;
};
