#pragma once

#include <QTime>
#include <QList>
#include "DcmCoreApi.h"
#include "DcmTagString.h"

/*! DICOM Time (TM) tag.
 *
 * This tag holds a time value in string format:
 * HHMMSS.FRAC,
 * Fractional part can go as small as 1 microsecond.
 */
class DCM_CORE_API DcmTagTM: public DcmTagString
{
public:

    /**
     * Default constructor.
     */
    DcmTagTM();

    /**
     * Construct a TM tag.
     * \param tagKey Tag key.
     */
    DcmTagTM(const DcmTagKey &tagKey);

    /**
     * Copy constructor.
     * \param tag Tag to be copied.
     */
    DcmTagTM(const DcmTagTM &tag);

    /**
     * Assignment operator.
     * \param tag Tag to be assigned.
     * \return This tag.
     */
    DcmTagTM& operator =(const DcmTagTM &tag);

    /**
     * Clone this tag.
     * \return Clone of this tag.
     */
    DcmTag *clone() const;

    /**
     * Destructor.
     */
    ~DcmTagTM();

    /**
     * Assign value.
     * This method handles string or time types.
     * \param v Value to be assigned.
     */
    void setValue(const QVariant &v);

    /**
     * Append value.
     * This method handles string or time types.
     * \param v Value to be added.
     */
    void appendValue(const QVariant &v);

    /**
     * Assign tag value.
     * \param v Value to be set.
     * \return This tag.
     */
    DcmTagTM& operator =(const QVariant &v);

    /**
     * Returns tag value in time format.
     * As Qt time type is presice up to millisecond,
     * this may result in precision loss.
     * For higher precision this tag has to be manipulated
     * in string format.
     */
    QTime asTime() const;

    /**
     * Retuns a list of time values.
     * \return List of time values.
     */
    QList<QTime> asTimeList() const;

    /**
     * Assign tag value in time type.
     * \param time Time to be set.
     */
    void setTime(const QTime &time);

    /**
     * Append time value.
     * \param time Time to be added.
     */
    void appendTime(const QTime &time);
};
