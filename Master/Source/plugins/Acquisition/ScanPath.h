#pragma once

#include "arsCore/UTData.h"
#include <QVector>
#include <QVector3D>
#include <QJsonObject>
#include <QtGlobal>
#include <QPainter>

class ScanPlan;
class Range;

/* ScanPath是一个基于QVector<QVector3D>实现的路径，提供三维坐标列的存储，并提供路径总长度的计算。 */
class ScanPath : public QVector<UTPose>
{
    enum Type
    {
        NONE = 0,
        LOCATE = 1,
        LINE = 2,
        ARC = 3,
        RECTANGLE = 4,
        DISK = 5,
        CYLINDER = 6,
    };

public:
    enum PathError
    {
        ERR_NONE = 0,
        ERR_VALUE_NOT_FOUND,
        ERR_TYPE_INVALID,
        ERR_NOT_IMPLEMENTED,
        ERR_AREA_INVALID,
        ERR_VALUE_INVALID,
        ERR_INVALID_AXIS_INDEX
    };

public:
    float GetLengthTotal();
    float GetLength(int index = 0) const;

    PathError SetPath(const QString &type, const QJsonObject &info);
    void Paint(QPainter &painter, int axisX, int axisY);
    QRectF GetBoundingRect(int axisX, int axisY) const;
    Range GetRange(int axisNum) const;

    void SetParent(ScanPlan *plan);
    ScanPlan *GetParent();

private:
    PathError SetPathNone(const QJsonObject &info);
    PathError SetPathLocate(const QJsonObject &info);
    PathError SetPathLine(const QJsonObject &info);
    PathError SetPathArc(const QJsonObject &info);
    PathError SetPathRectangle(const QJsonObject &info);
    PathError SetPathDisk(const QJsonObject &info);
    PathError SetPathCylinder(const QJsonObject &info);
    PathError SetPathCoordList(const QJsonObject &info);

    ScanPlan *parent;
    UTPose pathStart;
    UTPose pathEnd;

    // bool lengthValid;
    // float lengthTotal;
    static const float RECTANGLE_MIN_WIDTH;
    typedef PathError (ScanPath:: *SetPathPointer)(const QJsonObject &);
    typedef QMap<QString, SetPathPointer> SetPathPointerMap;
    static const SetPathPointerMap pathFuncMap;
    static const SetPathPointerMap InitializeFuncMap();
};
