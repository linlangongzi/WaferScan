#pragma once

#include <Core/Id.h>
#include <QIcon>
#include <QObject>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QIcon;
class QWidget;
QT_END_NAMESPACE

namespace Core {

class CORE_EXPORT IOptionsPage : public QObject
{
    Q_OBJECT

public:
    IOptionsPage(QObject *parent = nullptr, bool registerGlobally = true);
    ~IOptionsPage() override;

    static const QList<IOptionsPage *> allOptionsPages();

    Id id() const { return m_id; }
    QString displayName() const { return m_displayName; }
    Id category() const { return m_category; }
    QString displayCategory() const { return m_displayCategory; }
    QIcon categoryIcon() const;

    virtual bool matches(const QString &searchKeyWord) const;
    virtual QWidget *widget() = 0;
    virtual void apply() = 0;
    virtual void finish() = 0;

protected:
    void setId(Id id) { m_id = id; }
    void setDisplayName(const QString &displayName) { m_displayName = displayName; }
    void setCategory(Id category) { m_category = category; }
    void setDisplayCategory(const QString &displayCategory) { m_displayCategory = displayCategory; }
    void setCategoryIcon(const QIcon &categoryIcon) { m_categoryIcon = categoryIcon; }

    Id m_id;
    Id m_category;
    QString m_displayName;
    QString m_displayCategory;
    QIcon m_categoryIcon;

    mutable bool m_keywordsInitialized = false;
    mutable QStringList m_keywords;
};

/*
    Alternative way for providing option pages instead of adding IOptionsPage
    objects into the plugin manager pool. Should only be used if creation of the
    actual option pages is not possible or too expensive at Qt Creator startup.
    (Like the designer integration, which needs to initialize designer plugins
    before the options pages get available.)
*/

class CORE_EXPORT IOptionsPageProvider : public QObject
{
    Q_OBJECT

public:
    IOptionsPageProvider(QObject *parent = nullptr);
    ~IOptionsPageProvider() override;

    static const QList<IOptionsPageProvider *> allOptionsPagesProviders();

    Id category() const { return m_category; }
    QString displayCategory() const { return m_displayCategory; }
    QIcon categoryIcon() const;

    virtual QList<IOptionsPage *> pages() const = 0;
    virtual bool matches(const QString & /* searchKeyWord*/) const = 0;

protected:
    void setCategory(Id category) { m_category = category; }
    void setDisplayCategory(const QString &displayCategory) { m_displayCategory = displayCategory; }
    void setCategoryIcon(const QIcon &categoryIcon) { m_categoryIcon = categoryIcon; }

    Id m_category;
    QString m_displayCategory;
    QIcon m_categoryIcon;
};

} // namespace Core
