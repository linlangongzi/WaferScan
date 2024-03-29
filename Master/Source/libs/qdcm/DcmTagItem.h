#pragma once

#include "DcmCoreApi.h"
#include "DcmTag.h"
#include "DcmTagList.h"

class DcmTagGroup;

/*! Item tag.
 * DICOM item tag is basically a list of tags.
 * Item is a part of sequence (SQ) tag. Item tag has a reserved
 * key (fffe, e000). Item tag does not have value representation.
 */
class DCM_CORE_API DcmTagItem: public DcmTag
{
public:

    /**
     * Default constructor.
     */
    DcmTagItem();

    /**
     * Copy constructor.
     * \param item Item to be copied.
     */
    DcmTagItem(const DcmTagItem &item);

    /**
     * Assignment operator.
     * \param item Item to be set.
     * \return This object.
     */
    DcmTagItem& operator =(const DcmTagItem &item);

    /**
     * Clone this item object.
     * \return Pointer to cloned item.
     */
    DcmTag *clone() const;

    /**
     * Destructor.
     */
    ~DcmTagItem();

    /**
     * Returns raw content size of this item.
     * \param transferSyntax Transfer syntax to encode tags.
     * \return Content size in bytes.
     */
    DcmSize contentSize(const DcmTransferSyntax &transferSyntax) const;

    /**
     * Returns size of the item tag including all contained tags.
     * \param transferSyntax Transfer syntax to encode tags.
     * \return Total size of this item tag.
     */
    DcmSize size(const DcmTransferSyntax &transferSyntax) const;

    /**
     * Insert tag to this item.
     * If tag already exists it will be replaced.
     * \param tag Tag to be inserted.
     */
    void insert(const DcmTag &tag);

    /**
     * Insert and retain the tag.
     * The tag will be deteted along with this item tag.
     * \param tagPtr Pointer to the tag to be inserted.
     */
    void insertAndRetain(DcmTag *tagPtr);

    /**
     * Returns a list of tag groups stored in this item.
     * \return Reference to tag groups list.
     */
    DcmTagList &tagGroups();

private:

    /**
     * Look for the group tag or create one if not found.
     * \param g Group number.
     * \return Pointer to found or created group tag.
     */
    DcmTagGroup *findOrCreateGroup(DcmUnsignedShort g);

    DcmTagList m_tagGroups; ///< List of tag group.
};
