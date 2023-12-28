#include "DocumentManager.h"
#include "IDocument.h"

#include <QApplication>
#include <QHostInfo>
#include <QLoggingCategory>

static const char TOC_FILENAME[] = "toc";
static DocumentManager *m_instance = nullptr;
static IDocument dummyDocument;

static Q_LOGGING_CATEGORY(debug, "core.doc.manager");

DocumentManager::DocumentManager(QObject *parent) :
    QObject(parent),
    document(&dummyDocument)
{
    Q_ASSERT(m_instance == nullptr);
    m_instance = this;
}

DocumentManager::~DocumentManager()
{
    if (hasCurrentDocument()) {
        m_instance->document->close();
    }
    m_instance = nullptr;
}

DocumentManager *DocumentManager::instance()
{
    return m_instance;
}

bool DocumentManager::hasCurrentDocument()
{
    return m_instance->document != &dummyDocument;
}

IDocument *DocumentManager::currentDocument()
{
    return m_instance->document;
}

QDir DocumentManager::currentDocumentDir()
{
    return m_instance->document->dir();
}

QFileSystemModel *DocumentManager::currentDocumentModel()
{
    if (hasCurrentDocument()) {
        return &m_instance->docModel;
    }
    return nullptr;
}

void DocumentManager::setCurrentDocument(const QString &path)
{
    QDir newDir(path);
    if (currentDocumentDir() == newDir || !isValidDocument(path)) {
        return;
    }
    auto *oldDoc = m_instance->document;
    if (oldDoc->isValid()) {
        emit oldDoc->aboutToClose();
        oldDoc->close();
        oldDoc->deleteLater();
        m_instance->document = &dummyDocument;
    }
    auto *doc = new IDocument(m_instance);
    if (!doc->tryOpen(path, nullptr)) {
        delete doc;
    } else {
        m_instance->document = doc;
    }
    m_instance->docModel.setRootPath(newDir.path());

    emit m_instance->currentDocumentChanged();
}

bool DocumentManager::createDocument(const QString &path, DocumentInfo info, QString *errorString)
{
    static QString dummy;
    if (!errorString) {
        errorString = &dummy;
    }
    if (path.isEmpty()) {
        *errorString = tr("无效的路径");
        return false;
    }
    if (QFileInfo::exists(path)) {
        *errorString = tr("文件已存在");
        return false;
    }
    QDir dir(path);
    if (!QDir().mkpath(path)) {
        *errorString = tr("无法创建路径");
        return false;
    }
    qCInfo(debug) << "Creating new document at" << path;

    {
        IDocument doc;
        if (!doc.tryOpen(dir.path(), errorString)) {
            return false;
        }

        info.uuid = QUuid::createUuid();
        info.creationTool = QApplication::applicationName();
        info.creationToolVersion = QApplication::applicationVersion() + " " + qApp->property("Revision").toString();
        info.creationTime = QDateTime::currentDateTime();
        info.creationPath = dir.absolutePath();
        info.creationHost = QHostInfo::localHostName();

        doc.setInfo(info);
    }

    setCurrentDocument(path);

    return true;
}

bool DocumentManager::isValidDocument(const QString &path)
{
    QDir dir(path);
    return dir.exists(TOC_FILENAME);
}
