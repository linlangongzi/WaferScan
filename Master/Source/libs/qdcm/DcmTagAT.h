#pragma once

#include "DcmCoreApi.h"
#include "DcmTag.h"

/*! DICOM Attribute Tag (AT) tag.
 *
 * Ordered pair of 16-bit unsigned integers
 * representing (group, element) tag key.
 */
class DCM_CORE_API DcmTagAT: public DcmTag
{
public:

    /**
     * Default constructor.
     */
    DcmTagAT();

    /**
     * Construct AT tag.
     * \param tagKey Tag key.
     */
    DcmTagAT(const DcmTagKey &tagKey);

    /**
     * Copy constructor.
     * \param tag Tag to be copied.
     */
    DcmTagAT(const DcmTagAT &tag);

    /**
     * Assignment operator.
     * \param tag Tag to be assigned.
     */
    DcmTagAT& operator =(const DcmTagAT &tag);

    /**
     * Clone this tag.
     * \return This tag's clone.
     */
    DcmTag *clone() const;

    /**
     * Destructor.
     */
    ~DcmTagAT();

    /**
     * Returns value of this tag.
     * The (group, element) key is returned as tag key hash
     * 32-bit unsigned integer.
     * \return Tag value.
     */
    QVariant value() const;

    /**
     * Returns tag's values.
     * Each value is a 32-bit unsigned integer, representing
     * tag key hash.
     * \return List of tag values.
     */
    QVariantList values() const;

    /**
     * Assign tag value.
     * This method handles unsigned int value that
     * is interpreted as tag key hash.
     * \param v Value to be set.
     */
    void setValue(const QVariant &v);

    /**
     * Appent tag value.
     * This method handles unsigned int value that
     * is interpreted as tag key hash.
     * \param v Value to be added.
     */
    void appendValue(const QVariant &v);

    /**
     * Assign value.
     * \param v Value to be set.
     * \return This tag.
     */
    DcmTagAT& operator =(const QVariant &v);

    /**
     * Returns tag multiplicity.
     * \return Multiplicity.
     */
    int multiplicity() const;

    /**
     * Returns tag value as tag key object.
     * \return Tag's value.
     */
    DcmTagKey asDcmTagKey() const;

    /**
     * Returns list of tag values.
     * \return List of tag's values.
     */
    QList<DcmTagKey> asDcmTagKeyList() const;

    /**
     * Assign tag value as tag key object.
     * \param v Value to be assigned.
     */
    void setDcmTagKey(const DcmTagKey &v);

    /**
     * Append tag key value to this tag.
     * \param v Value to be added.
     */
    void appendDcmTagKey(const DcmTagKey &v);

    /**
     * Returns content size of this tag.
     * For attribute tag the content size is always 4 * multiplicity.
     * \param transferSyntax Transfer syntax to encode the tag.
     * \return Tag's content size in bytes.
     */
    DcmSize contentSize(const DcmTransferSyntax &transferSyntax) const;

private:

    QList<DcmTagKey> m_values;  ///< List of values.
};
