#ifndef UTIMAGEPALETTE_H
#define UTIMAGEPALETTE_H

#include <QVector>
#include <QRgb>
#include "arsCore/UTData.h"

class UTImagePalette
{
public:
    typedef QVector<QRgb> ColorTable;

    ColorTable colorTable;
    CDataValue min;
    CDataValue max;

    enum
    {
        colorTableLength = UCHAR_MAX + 1
    };

public:
    bool operator ==(const UTImagePalette &other) const
    {
        return colorTable == other.colorTable && min == other.min && max == other.max;
    }

    bool operator !=(const UTImagePalette &other) const
    {
        return !(*this == other);
    }
};

Q_DECLARE_METATYPE(UTImagePalette)

#endif                                                                          // UTIMAGEPALETTE_H
