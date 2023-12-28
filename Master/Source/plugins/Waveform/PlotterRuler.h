#pragma once

#include "WaveformGlobal.h"
#include <allrising/Range.h>
#include <QPainter>

struct RulerStyleOption
{
    Q_GADGET
    Q_PROPERTY(QColor backgroundColor MEMBER backgroundColor)
    Q_PROPERTY(QColor majorColor MEMBER majorColor)
    Q_PROPERTY(QColor minorColor MEMBER minorColor)
    Q_PROPERTY(QColor gridColor MEMBER gridColor)
    Q_PROPERTY(QColor unitColor MEMBER unitColor)
    Q_PROPERTY(QString unitName MEMBER unitName)
    Q_PROPERTY(bool reverse MEMBER reverse)

    QColor backgroundColor = Qt::transparent;
    QColor majorColor = Qt::black;
    QColor minorColor = Qt::gray;
    QColor gridColor = Qt::darkGray;
    QColor unitColor = Qt::black;
    QString unitName;
    bool rangeText = false;
    bool reverse = false;
};

class WAVEFORM_EXPORT PlotterRuler
{
public:
    PlotterRuler();
    void setBackgroundColor(const QColor &color) {backgroundColor = color;}
    void SetMajorColor(const QColor &color) {majorColor = color;}
    void SetMinorColor(const QColor &color) {minorColor = color;}
    void SetUnitColor(const QColor &color) {unitColor = color;}
    void setUnitName(const QString &name) {unitName = name;}
    void drawRuler(QPainter *painter, const Range &range, const QRect &rect, QRect boundingRect, Qt::Edge edge, bool isReversed, qreal cursorPos = Q_INFINITY);
    void drawRuler(QPainter *painter, qreal min, qreal max, QRect rect, QRect boundingRect, Qt::Edge edge, bool isReversed, qreal cursorPos = Q_INFINITY);
    void DrawGrid(QPainter *painter, const QRectF &rect) const;

private:
    QColor backgroundColor;
    QColor majorColor;
    QColor minorColor;
    QColor unitColor;
    QString unitName;
};
