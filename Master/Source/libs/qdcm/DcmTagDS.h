#pragma once

#include "DcmCoreApi.h"
#include "DcmTagString.h"

/*! DICOM Decimal String (DS) tag.
 *
 * A string of characters representing either a fixed point
 * or a floating point number.
 */
class DCM_CORE_API DcmTagDS: public DcmTagString
{
public:

    /**
     * Default constructor.
     */
    DcmTagDS();

    /**
     * Construct DS tag.
     * \param tagKey Tag key.
     */
    DcmTagDS(const DcmTagKey &tagKey);

    /**
     * Copy constructor.
     * \param tag Tag to be copied.
     */
    DcmTagDS(const DcmTagDS &tag);

    /**
     * Assignment operator.
     * \param tag Tag to be assigned.
     * \return This tag.
     */
    DcmTagDS& operator =(const DcmTagDS &tag);

    /**
     * Clone this tag.
     * \return Pointer to this tag's clone.
     */
    DcmTag *clone() const;

    /**
     * Destructor.
     */
    ~DcmTagDS();

    /**
     * Assign tag value.
     * This method takes Double, Int or String values.
     * \param v Value to be assigned.
     */
    void setValue(const QVariant &v);

    /**
     * Append value.
     * This method handles Double, Int or String values.
     * \param v Value to be added.
     */
    void appendValue(const QVariant &v);

    /**
     * Assign value.
     * \param v Value to be set.
     * \return This tag.
     */
    DcmTagDS& operator =(const QVariant &v);

    /**
     * Retuns tag's value as double.
     * \return Tag's value.
     */
    double asDouble() const;

    /**
     * Returns list of tag's values.
     * \return List of tag's values.
     */
    QList<double> asDoubleList() const;

    /**
     * Assign tag value.
     * \param v Value to be assigned.
     */
    void setDouble(double v);

    /**
     * Append tag value.
     * \param v Value to be added.
     */
    void appendDouble(double v);
};
