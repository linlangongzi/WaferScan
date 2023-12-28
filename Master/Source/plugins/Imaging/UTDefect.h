#pragma once

#include <QVector>
#include <QPolygonF>
#include <QVector2D>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <QJsonValue>
#include <QPixmap>
#include <allrising/MetaType/MetaType.h>

class UTDefect;

using UTDefectList =  QList<UTDefect>;
/*! UTDefect 是一个在超声C扫描图像中的缺陷，包含缺陷的位置、深度、尺寸、图像等信息 */
class UTDefect
{
    Q_GADGET

    Q_PROPERTY(QPolygonF minRect MEMBER minRect)
    Q_PROPERTY(float averageDepth MEMBER averageDepth)
    Q_PROPERTY(float minDepth MEMBER minDepth)
    Q_PROPERTY(float maxDepth MEMBER maxDepth)
    Q_PROPERTY(float bottomDepth MEMBER bottomDepth)
    Q_PROPERTY(float minAmp MEMBER minAmp)
    Q_PROPERTY(float maxAmp MEMBER maxAmp)
    Q_PROPERTY(float area MEMBER area)
    Q_PROPERTY(QRect markRect MEMBER markRect)
    Q_PROPERTY(QImage image MEMBER image)
    Q_PROPERTY(int pixelCount MEMBER pixelCount)
    Q_PROPERTY(QString comment MEMBER comment)
    Q_PROPERTY(QList<QImage> m_screenshots MEMBER m_screenshots)
    Q_PROPERTY(qreal percentage MEMBER percentage)

public:
    DECLARE_COMPARE_OPERATORS(UTDefect)

    UTDefect() {}

    QPolygonF GetOuterBoundary() const;

    double GetArea() const;
    QPolygonF GetMinRect() const;
    double GetLength() const;
    double GetWidth() const;
    double GetCharacteristicSize() const;
    QVector2D GetCenter() const;

    UTDefectList GetChildren() const;

    bool IsOutside() const;
    float GetAverageDepth() const {return averageDepth;}
    float GetBottomDepth() const {return bottomDepth;}
    float GetMaxAmp() const {return maxAmp;}
    QList<QImage> screenshots() const;
    void setScreenshots(const QList<QImage> &newImages);

    QJsonValue toJson() const;
    static UTDefect fromJson(const QJsonValue &value);
    QVariantMap ToVariantMap(const QPointF &origin = QPointF()) const;

public:
    QPolygonF minRect;                                                          /* 特征矩形 */
    float averageDepth = 0;
    float minDepth = Q_INFINITY;
    float maxDepth = -Q_INFINITY;
    float bottomDepth = 0;
    float minAmp = Q_INFINITY;
    float maxAmp = -Q_INFINITY;
    float area = 0;
    QList<QImage> m_screenshots = {};
    QRect markRect;
    QImage image;
    int pixelCount = 0;
    QString comment;
    qreal percentage = 0;
};
