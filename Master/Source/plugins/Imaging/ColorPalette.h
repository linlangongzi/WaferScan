#pragma once

#include "UTImagePalette.h"
#include <QJsonValue>
#include <QLinearGradient>
#include <QVector>

class QPainter;

/*!
 * ColorPalette 是一个渐变调色板，并可根据渐变颜色点，提供插值好的颜色查询表。
 */
class ColorPalette
{
    Q_GADGET

    Q_PROPERTY(QLinearGradient gradient READ gradient WRITE setGradient)
    Q_PROPERTY(GradientType gradientType READ gradientType WRITE setGradientType)
    Q_PROPERTY(QColor emptyColor READ emptyColor WRITE setEmptyColor)
    Q_PROPERTY(qreal minValue READ minValue WRITE setMinValue)
    Q_PROPERTY(qreal maxValue READ maxValue WRITE setMaxValue)
    Q_PROPERTY(bool isAutoMax READ isAutoMax WRITE setAutoMax)
    Q_PROPERTY(bool isAutoMin READ isAutoMin WRITE setAutoMin)

public:
    enum GradientType {
        LinearGradient,
        StepGradient
    };
    Q_ENUM(GradientType)

    enum DefaultGradient {
        Gray,
        Uit,
        Jet,
        Rainbow,
#ifdef PIPE_DETECT
        Step,
#endif
    };
    Q_ENUM(DefaultGradient)

public:
#ifdef PIPE_DETECT
    ColorPalette(DefaultGradient m_type = Step);
#else
    ColorPalette(DefaultGradient m_type = Rainbow);
#endif

    QColor colorAt(double position) const;
    UTImagePalette GetColorPalette(bool ignoreEmpty = false) const;

    QLinearGradient gradient() const;
    GradientType gradientType() const;
    QColor emptyColor() const;
    qreal minValue() const;
    qreal maxValue() const;
    bool isAutoMax() const;
    bool isAutoMin() const;

    void setGradient(QLinearGradient gradient);
    void setGradientType(GradientType newType);
    void setEmptyColor(const QColor &emptyColor);
    void setMinValue(qreal minValue);
    void setMaxValue(qreal maxValue);
    void setAutoMax(bool isAutoMax);
    void setAutoMin(bool isAutoMin);

    qreal range() const {return m_max - m_min;}
    qreal mapToRange(qreal pos) const {return pos * range() + m_min;}
    qreal mapFromRange(qreal pos) const {return (pos - m_min) / range();}
    void removeStop(int index);
    void setStop(int index, qreal position, const QColor &color);
    void paint(QPainter *painter, const QRect &rect, Qt::Orientation orientation = Qt::Horizontal, bool isReversed = false);

    QJsonObject toJson() const;
    static ColorPalette fromJson(const QJsonValue &json);

    bool operator ==(const ColorPalette &other) const;
    bool operator !=(const ColorPalette &other) const;

private:
    static QColor ToColor(const QJsonValue &json);
    static QJsonValue FromColor(const QColor &color);
    QColor colorAt(double position, const QGradientStop &prev, const QGradientStop &next) const;

    QLinearGradient m_gradient;
    GradientType m_type = LinearGradient;
    QColor m_emptyColor;
    qreal m_min = 0;
    qreal m_max = 100;
    bool m_autoMax = false;
    bool m_autoMin = false;
};
