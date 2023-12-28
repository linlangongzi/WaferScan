#include "DcmTagUT.h"

DcmTagUT::DcmTagUT()
    : DcmTagString(DcmTagKey(), DcmVr::UT)
{
}

DcmTagUT::DcmTagUT(const DcmTagKey &tagKey)
    : DcmTagString(tagKey, DcmVr::UT)
{
}

DcmTagUT::DcmTagUT(const DcmTagUT &tag)
    : DcmTagString(tag)
{
}

DcmTagUT& DcmTagUT::operator =(const DcmTagUT &tag)
{
    if (this != &tag) {
        DcmTagString::operator =(tag);
    }
    return *this;
}

DcmTag *DcmTagUT::clone() const
{
    return new DcmTagUT(*this);
}

DcmTagUT::~DcmTagUT()
{
}

DcmTagUT& DcmTagUT::operator =(const QVariant &v)
{
    setValue(v);
    return *this;
}

DcmSize DcmTagUT::size(const DcmTransferSyntax &transferSyntax) const
{
    if (transferSyntax.encoding() == DcmEncoding_Explicit) {
        return contentSize(transferSyntax) + 12;
    }
    return contentSize(transferSyntax) + 8; // Implicit encoding.
}
