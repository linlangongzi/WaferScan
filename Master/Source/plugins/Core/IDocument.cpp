#include "IDocument.h"
#include <allrising/Util.h>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "core.doc");

QHash<QString, DocumentPartFactory> IDocument::m_partTypes;
static const char TOC_FILENAME[] = "toc";

IDocument::IDocument(QObject *parent) :
    QObject(parent)
{

}

bool IDocument::isValid() const
{
    return m_toc;
}

bool IDocument::isWritable() const
{
    return isValid() && m_toc->isWritable();
}

QDir IDocument::dir() const
{
    return m_dir;
}

DocumentInfo IDocument::info() const
{
    return m_info;
}

bool IDocument::isModified() const
{
    return m_modified;
}

IDocumentPart *IDocument::addPart(const QString &fileName, const QString &typeName)
{
    if (!isWritable()) {
        return nullptr;
    }
    if (!m_partTypes.contains(typeName)) {
        qCWarning(debug) << "Failed adding part: type" << typeName << "of file" << fileName << "not supported";
        return nullptr;
    }
    DocumentPartInfo part;
    part.type = typeName;
    part.fileName = fileName;
    tryOpenPart(part);
    if (part.part) {
        qCInfo(debug) << "Added part" << fileName << "with type" << typeName;
        m_parts << part;
        writeInfo();
        emit partChanged();
        return part.part;
    }
    return nullptr;
}

IDocumentPart *IDocument::addPart(const QString &fileName, const QMetaObject *partMeta)
{
    if (!isWritable()) {
        qCWarning(debug) << "Failed adding part: document" << dir().path() << "not writable" << m_toc->openMode();
        return nullptr;
    }
    for (const auto &partType: m_partTypes) {
        if (partType.meta == partMeta) {
            DocumentPartInfo part;
            part.type = partType.typeName;
            part.fileName = fileName;
            tryOpenPart(part);
            if (part.part) {
                qCInfo(debug) << "Added part" << fileName << "with type" << part.type << "(" << partMeta->className() << ")";
                m_parts << part;
                writeInfo();
                emit partChanged();
                return part.part;
            }
        }
    }
    qCWarning(debug) << "Failed adding part: unknown type class" << partMeta->className();
    return nullptr;
}

void IDocument::removePart(IDocumentPart *part)
{
    if (!isWritable()) {
        qCWarning(debug) << "Failed removing part: document is read-only";
        return;
    }
    if (part->parent() != this) {
        qCWarning(debug) << "Failed removing part: part" << part->objectName() << "not owned by document" << m_dir.path();
        return;
    }
    for (int index = m_parts.count() - 1; index >= 0; --index) {
        auto &p = m_parts[index];
        if (p.part == part) {
            m_parts.removeAt(index);
            qCInfo(debug) << "Removed part entry" << index << part->fileName() << p.type;
        }
    }
    emit partChanged();
    part->deleteLater();
    writeInfo();
}

IDocumentPart *IDocument::part(const QString &fileName, const QMetaObject *meta)
{
    if (!isValid()) {
        return nullptr;
    }
    for (auto &part: m_parts) {
        if (!fileName.isEmpty() && fileName != part.fileName) {                 /* 用户指定了文件名, 但不匹配 */
            continue;
        }
        const QMetaObject *partMeta = m_partTypes.value(part.type).meta;
        if (meta && !partMeta->inherits(meta)) {                                /* 用户指定了类型, 但该part未继承该类型 */
            continue;
        }
        /* 文件匹配所有条件, 尝试打开它 */
        tryOpenPart(part);
        return part.part;
    }
    return nullptr;
}

QList<IDocumentPart *> IDocument::parts(const QMetaObject *meta)
{
    if (!isValid()) {
        return QList<IDocumentPart *>();
    }
    QList<IDocumentPart *> parts;
    for (auto &part: m_parts) {
        const QMetaObject *partMeta = m_partTypes.value(part.type).meta;
        if (meta && !partMeta->inherits(meta)) {
            continue;
        }
        tryOpenPart(part);
        parts << part.part;
    }
    return parts;
}

bool IDocument::registerPartFactory(const QMetaObject *meta, const QString &typeName, std::function<IDocumentPart *()> create)
{
    DocumentPartFactory factory;
    factory.meta = meta;
    factory.typeName = typeName;
    factory.create = create;
    if (m_partTypes.contains(factory.typeName)) {
        qCCritical(debug) << "Failed registering document part factory:" << factory.typeName << "already exists with class" << m_partTypes[factory.typeName].meta->className();
        return false;
    }
    qCInfo(debug) << "Registering part with type name" << typeName;
    m_partTypes[factory.typeName] = factory;
    return true;
}

bool IDocument::save()
{
    if (!m_modified) {
        return true;
    }

    bool ok = true;
    for (auto &part: m_parts) {
        ok &= part.part->save();
    }
    if (ok) {
        setModified(false);
    }
    return ok;
}

void IDocument::setInfo(const DocumentInfo &info)
{
    if (!isValid()) {
        qCWarning(debug) << "Can't write toc to invalid file";
        return;
    }
    if (!isWritable()) {
        qCWarning(debug) << "Can't write toc to read-only file" << m_toc->fileName();
        return;
    }
    if (info == m_info) {
        return;
    }
    m_info = info;
    writeInfo();
    emit infoChanged(info);
}

void IDocument::setModified(bool modified)
{
    if (m_modified == modified) {
        return;
    }

    m_modified = modified;
    emit modifiedChanged(modified);
}

void IDocument::tryOpenPart(DocumentPartInfo &part)
{
    if (!isValid() || part.part) {
        return;
    }
    /* Part未打开, 创建并打开该Part. */
    if (m_partTypes.contains(part.type)) {
        const auto &factory = m_partTypes[part.type];
        IDocumentPart *p = factory.create();
        p->setParent(this);
        p->setFileName(m_dir.filePath(part.fileName));
        p->setObjectName(p->fileName());
        /* 用当前OpenMode打开 */
        if (!p->open(m_toc->openMode())) {
            qCWarning(debug) << "Failed opening part: can't open file" << part.fileName << "in" << m_toc->openMode() << "-" << p->errorString();
            p->deleteLater();
            p = nullptr;
        }
        part.part = p;
    } else {
        qCWarning(debug) << "Failed opening part: unsupported type" << part.type << "of file" << part.fileName;
    }
}

bool IDocument::tryOpen(const QString &path, QString *errorString)
{
    if (isValid()) {
        qCWarning(debug) << "Cannot open document" << path << ": already opened";
        return false;
    }

    QDir dir(path);
    qCDebug(debug) << "Trying to open document at" << path;

    auto *toc = new IDocumentPart;
    toc->setParent(this);
    toc->setObjectName(TOC_FILENAME);
    toc->setFileName(dir.filePath(TOC_FILENAME));
    qCDebug(debug) << "Opening document toc" << toc->fileName();
    bool ret = toc->open(QIODevice::ReadWrite) || toc->open(QIODevice::ReadOnly);
    if (!ret) {
        if (errorString) {
            *errorString = toc->errorString();
        }
        qCDebug(debug) << "Failed opening document toc:" << toc->errorString();
        delete toc;
        return false;
    }

    QJsonObject json = toc->readJson().toObject();
    m_info = convertFromJson<DocumentInfo>(json["info"]);
    m_parts = convertFromJson<QList<DocumentPartInfo> >(json["files"]);
    m_toc = toc;
    m_dir = dir;
    qCDebug(debug) << "Opened document" << m_dir.path() << m_toc->openMode() << "with" << m_parts.count() << "parts";
    emit infoChanged(m_info);

    return true;
}

void IDocument::writeInfo()
{
    if (!m_toc) {
        return;
    }
    QJsonObject json;
    json["files"] = convertToJson(m_parts);
    json["info"] = convertToJson(m_info);
    m_toc->writeJson(json);
}

void IDocument::close()
{
    if (!isValid()) {
        qCWarning(debug) << "Cannot close document: already closed";
        return;
    }
    qCDebug(debug) << "Closing document" << m_dir.path() << "with" << m_parts.count() << "parts";
    for (auto &part: m_parts) {
        part.part->close();
        part.part->deleteLater();
    }
    m_parts.clear();
    m_toc->deleteLater();
    m_toc = nullptr;
}
