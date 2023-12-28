#pragma once

#include <QVector>
#include <QRgb>
#include <Ultrasonic/UTData.h>

class UTImagePalette
{
public:
    using ColorTable = QVector<QRgb>;

    ColorTable colorTable;
    CDataValue min;
    CDataValue max;

    enum
    {
        colorTableLength = UCHAR_MAX + 1
    };

public:
    UTImagePalette() : colorTable(colorTableLength) {}

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
