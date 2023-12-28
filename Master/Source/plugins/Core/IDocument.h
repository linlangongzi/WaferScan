#pragma once

#include <QDir>
#include <QFile>
#include <QObject>
#include "DocumentInfo.h"
#include "IDocumentPart.h"

class CORE_EXPORT DocumentPartInfo
{
    Q_GADGET

    Q_PROPERTY(QString fileName MEMBER fileName)
    Q_PROPERTY(QString type MEMBER type)

public:
    QString type;                                                               /*!< 文件类型, 须在IDocumentPartFactory中注册 */
    QString fileName;                                                           /*!< 文件名, 相对于IDocument路径 */

private:
    IDocumentPart *part = nullptr;
    friend class IDocument;
};

class CORE_EXPORT IDocument : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DocumentInfo info READ info WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(bool modified READ isModified WRITE setModified NOTIFY modifiedChanged)

public:
    explicit IDocument(QObject *parent = nullptr);

    bool isValid() const;
    bool isWritable() const;
    QDir dir() const;

    DocumentInfo info() const;
    bool isModified() const;

    IDocumentPart *addPart(const QString &fileName, const QString &typeName);
    IDocumentPart *addPart(const QString &fileName, const QMetaObject *partType);
    template <typename T> T *addPart(const QString &fileName);
    void removePart(IDocumentPart *part);

    template <typename T> T *part(const QString &fileName = QString());
    IDocumentPart *part(const QString &fileName, const QMetaObject *meta = nullptr);
    QList<IDocumentPart *> parts(const QMetaObject *meta = nullptr);
    template <typename T> QList<T *> parts();

    static bool registerPartFactory(const QMetaObject *meta, const QString &typeName, std::function<IDocumentPart *()> create);

public slots:
    bool save();

    void setInfo(const DocumentInfo &info);
    void setModified(bool modified);

signals:
    void infoChanged(DocumentInfo info);
    void modifiedChanged(bool modified);
    void partChanged();
    void aboutToClose();

private:
    void tryOpenPart(DocumentPartInfo &part);
    bool tryOpen(const QString &path, QString *errorString);
    void writeInfo();
    void close();
    DocumentPartFactory findFactory(const QMetaObject *meta);

private:
    QDir m_dir;
    IDocumentPart *m_toc = nullptr;

    DocumentInfo m_info;
    QList<DocumentPartInfo> m_parts;
    bool m_modified = false;

    static QHash<QString, DocumentPartFactory> m_partTypes;

    friend class DocumentManager;
};

template<typename T>
T *IDocument::addPart(const QString &fileName)
{
    auto *part = addPart(fileName, &T::staticMetaObject);
    return qobject_cast<T *>(part);
}

template <typename T>
T *IDocument::part(const QString &fileName)
{
    return qobject_cast<T *>(part(fileName, &T::staticMetaObject));
}

template<typename T>
QList<T *> IDocument::parts()
{
    auto p = parts(&T::staticMetaObject);
    QList<T *> typedParts;
    for (auto *part: p) {
        T *typedPart = qobject_cast<T *>(part);
        if (typedPart) {
            typedParts << typedPart;
        }
    }
    return typedParts;
}
