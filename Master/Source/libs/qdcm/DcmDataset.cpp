#include "DcmDictionary.h"
#include "DcmTagDescription.h"
#include "DcmDataset.h"

DcmDataset::DcmDataset(QObject *parent) :
    QObject(parent)
{
}

void DcmDataset::insert(const DcmTag &tag)
{
    // Ignore group tags being inserted
    if (tag.tagKey().isGroupSize()) {
        return;
    }

    DcmTagGroup *group = findOrCreateGroup(tag.tagKey().group());
    group->insert(tag);
}

void DcmDataset::insertAndRetain(DcmTag *tagPtr)
{
    if (!tagPtr) {
        return;
    }

    // Ignore group tags being inserted
    if (tagPtr->tagKey().isGroupSize()) {
        return;
    }

    DcmTagGroup *group = findOrCreateGroup(tagPtr->tagKey().group());
    group->insertAndRetain(tagPtr);
}

DcmTag *DcmDataset::findTag(const DcmTagKey &tagKey) const
{
    DcmTag *groupTag = m_tagGroups.findFirst(DcmTagKey(tagKey.group(), 0x0000));
    if (!groupTag) {
        return nullptr;
    }
    DcmTagGroup *group = dynamic_cast<DcmTagGroup *>(groupTag);
    Q_ASSERT(group);

    return group->findTag(tagKey);
}

DcmTag *DcmDataset::findTag(const QString &tagName) const
{
    DcmTagDescription desc = DcmDictionary::getInstancePtr()->findByName(tagName);
    if (desc.isValid()) {
        return findTag(desc.tagKey());
    }
    return nullptr;
}

QVariant DcmDataset::tagValue(const DcmTagKey &tagKey) const
{
    DcmTag *tag = findTag(tagKey);
    if (tag) {
        return tag->value();
    }

    return QVariant();
}

QVariant DcmDataset::tagValue(const QString &tagName) const
{
    DcmTagDescription desc = DcmDictionary::getInstancePtr()->findByName(tagName);
    if (desc.isValid()) {
        return tagValue(desc.tagKey());
    }
    return QVariant();
}

QVariant DcmDataset::operator [](const QString &tagName) const
{
    return tagValue(tagName);
}

QVariantList DcmDataset::tagValues(const DcmTagKey &tagKey) const
{
    DcmTag *tag = findTag(tagKey);
    if (tag) {
        return tag->values();
    }

    return QVariantList();
}

QVariantList DcmDataset::tagValues(const QString &tagName) const
{
    DcmTagDescription desc = DcmDictionary::getInstancePtr()->findByName(tagName);
    if (desc.isValid()) {
        return tagValues(desc.tagKey());
    }
    return QVariantList();
}

void DcmDataset::setTagValue(const DcmTagKey &tagKey, const QVariant &value)
{
    DcmTag *tag = findTag(tagKey);
    if (tag) {
        tag->setValue(value);
    } else {
        tag = DcmTag::create(tagKey);
        tag->setValue(value);
        insertAndRetain(tag);
    }
}

void DcmDataset::setTagValue(const QString &tagName, const QVariant &value)
{
    DcmTagDescription desc = DcmDictionary::getInstancePtr()->findByName(tagName);
    if (desc.isValid()) {
        setTagValue(desc.tagKey(), value);
    }
}

DcmTagList &DcmDataset::tagGroups()
{
    return m_tagGroups;
}

DcmTagGroup *DcmDataset::findOrCreateGroup(DcmUnsignedShort g)
{
    DcmTag *groupTag = m_tagGroups.findFirst(DcmTagKey(g, 0x0000));
    DcmTagGroup *group = nullptr;
    if (groupTag) {
        group = dynamic_cast<DcmTagGroup *>(groupTag);
        Q_ASSERT(group);
    } else {
        group = new DcmTagGroup(g);
        m_tagGroups.insertAndRetain(group);
    }

    Q_ASSERT(group);
    return group;
}

void DcmDataset::attachModule(const DcmModule &module)
{
    DcmTagList tags = module.tagList();
    foreach (DcmTag *tagPtr, tags.list()) {
        this->insert(*tagPtr);
    }
}
