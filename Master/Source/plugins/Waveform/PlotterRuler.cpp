#include "PlotterRuler.h"
#include <QFontMetrics>
#include <QVector2D>
#include <QtMath>
#include <functional>
#include <allrising/Util.h>

struct EdgeOption {
    Qt::Orientation orientation = Qt::Horizontal;
    Qt::Edge edge = Qt::TopEdge;
    QPointF origin(const QRectF &rect)
    {
        if (edge == Qt::TopEdge || edge == Qt::LeftEdge) {
            return rect.topLeft();
        } else {
            return rect.bottomLeft() - QPointF(0, 1);
        }
    }

    QPointF unit;
    QPointF tick;
};

const QMap<Qt::Edge, EdgeOption> edgeOptions = []{
    using namespace Qt;
    QMap<Qt::Edge, EdgeOption> d;
    d[TopEdge].unit = QPointF(1, 0);                                            /* 正方向向左 */
    d[TopEdge].tick = QPointF(0, 1);                                            /* 刻度向下 */

    d[BottomEdge].unit = QPointF(1, 0);                                         /* 正方向向右 */
    d[BottomEdge].tick = QPointF(0, -1);                                        /* 刻度向上 */

    d[LeftEdge].unit = QPointF(1, 0);                                           /* 正方向向下 */
    d[LeftEdge].tick = QPointF(0, 1);                                           /* 刻度向右 */

    d[RightEdge].unit = QPointF(1, 0);                                          /* 正方向向下 */
    d[RightEdge].tick = QPointF(0, -1);                                         /* 刻度向左 */
    return d;
}();

const int MAJOR_TICK_LENGTH = 4;
const int MINOR_TICK_LENGTH = 2;
const int MINOR_TICK_DIVISION = 10;
const int MAJOR_TICK_MIN_SPACE = 50;

const qreal MAJOR_TICK_RATIO = 0.1;
const qreal MINOR_TICK_RATIO = 0.05;
const qreal AVOID_RECT_RATIO = 1;

QRectF GetTextRect(QPainter *painter, const QString &text, const QRectF &boundingRect, const QPointF &anchor)
{
    const QRectF textRect = painter->boundingRect(QRectF(anchor, QSize()), text);
    return Range::constrainRect(textRect, boundingRect);
}

bool RectIntersects(const QRectF &rect, const QList<QRectF> &avoids)
{
    for (const QRectF &r: avoids) {
        if (rect.intersects(r)) {
            return true;
        }
    }
    return false;
}

QRectF DrawMajorTick(QPainter *painter, const EdgeOption &option, int height, qreal pos, const QPointF &start, const QRectF &boundingRect, const QList<QRectF> &avoids, const QBrush &fill = QBrush())
{
    const qreal tickLength = qMax<qreal>(height * MAJOR_TICK_RATIO, MAJOR_TICK_LENGTH);
    const QPointF end = start + option.tick * tickLength;                /*!< 刻度线的终点 */
    const int fontSize = height - tickLength - 1;
    QFont font;
    font.setPixelSize(fontSize);
    painter->setFont(font);

    const QString text = QString::number(pos);
    const int textPixel = painter->fontMetrics().width(text);
    QRectF textRect = GetTextRect(painter, text, boundingRect, start + option.tick * height - option.unit * textPixel / 2);  /*!< 文字区域 */

    painter->drawLine(start, end);
    if (RectIntersects(textRect, avoids)) {
        return QRectF();
    }
    painter->fillRect(textRect, fill);
    painter->drawText(textRect, Qt::TextDontClip | Qt::AlignCenter | Qt::AlignVCenter, text);

    const int space = AVOID_RECT_RATIO * height;
    textRect.setWidth(textRect.width() + space);
    return textRect;
}

/*! 计算理想的主刻度间隔 */
qreal GetInterval(qreal unitPerPixel)
{
    int sign = arsSignF(unitPerPixel);
    unitPerPixel = qAbs(unitPerPixel);
    qreal minInterval = MAJOR_TICK_MIN_SPACE * unitPerPixel;                    /* 在逻辑坐标系中的最小间隔，例如150 */
    int exponent = qCeil(log10(minInterval));                                   /* 最小间隔向上取10的幂次，例如log10(150) ~= 3 */
    qreal interval = qPow(10, exponent);                                        /* 理想间隔，例如1000 */

    if (interval / 5 > minInterval) {                                           /* 如果理想间隔划成5等份也满足条件，那么5等分 */
        interval /= 5;
    } else if (interval / 2 > minInterval) {                                    /* 5等分太小，尝试2等分 */
        interval /= 2;
    }
    return interval * sign;
}

/*! 计算下一个刻度位置 */
inline int GetNextIndex(qreal pos, qreal interval)
{
    return qFloor(pos / interval) + 1;
}

/*! 绘画标尺的主接口 */
PlotterRuler::PlotterRuler() : backgroundColor(Qt::white),
    majorColor(Qt::black),
    minorColor(Qt::gray),
    unitColor(Qt::black),
    unitName("")
{

}

void PlotterRuler::drawRuler(QPainter *painter, const Range &range, const QRect &rect, QRect boundingRect, Qt::Edge edge, bool isReversed, qreal cursorPos)
{
    drawRuler(painter, range.min(), range.max(), rect, boundingRect, edge, isReversed, cursorPos);
}

void PlotterRuler::drawRuler(QPainter *painter, qreal min, qreal max, QRect rect, QRect boundingRect, Qt::Edge edge, bool isReversed, qreal cursorPos)
{
    if (!edgeOptions.contains(edge)) {
        return;
    }

    painter->save();
    if (edge == Qt::LeftEdge || edge == Qt::RightEdge) {
        painter->translate(rect.topLeft());
        painter->rotate(270);
        painter->translate(-rect.height(), 0);
        painter->translate(-rect.topLeft());
        isReversed = !isReversed;

        rect = QRect(rect.topLeft(), QSize(rect.height(), rect.width()));
        boundingRect = QRect(boundingRect.topLeft(), QSize(boundingRect.height(), boundingRect.width()));
    }

    EdgeOption option = edgeOptions[edge];                                      /*!< 该标尺区域的绘图选项 */
    option.edge = edge;
    option.orientation = option.edge & (Qt::BottomEdge | Qt::TopEdge) ? Qt::Horizontal : Qt::Vertical;
    const qreal rangeLength = max - min;
    const int length = rect.width();                                            /*!< 标尺区域的像素宽度 */
    const qreal ratio = rangeLength / length;                                   /*!< 单位长度的像素数 */
    const qreal height = rect.height();
    QPointF origin = option.origin(rect);                                       /*!< 标尺区域的原点 */
    QPointF unit = option.unit / ratio;                                         /*!< 标尺正方向向量 */
    if (isReversed) {
        origin = origin + rangeLength * unit;
        unit = -unit;
    }
    // 绘制两端文字
    QList<QRectF> avoids;

    painter->setPen(majorColor);
    avoids << DrawMajorTick(painter, option, height, min, origin, boundingRect, avoids);
    const QRectF endTextRect = DrawMajorTick(painter, option, height, max, origin + unit * rangeLength, boundingRect, avoids);
    avoids << endTextRect;
    /* 绘制单位文字 */
    if (!unitName.isEmpty()) {
        const qreal height = endTextRect.width();
        int textLength = painter->fontMetrics().width(unitName);
        const QPointF textOrigin = isReversed ? option.origin(endTextRect) + option.tick * height - unit * (height + 1) * ratio : option.origin(endTextRect) + option.tick * height - (textLength + 1) * option.unit;
        QRectF textRect = GetTextRect(painter, unitName, boundingRect, textOrigin);

        if (!RectIntersects(textRect, avoids)) {
            painter->drawText(textRect, unitName);
            textRect.setLeft(textRect.left() - textRect.width() / 2);
            avoids << textRect;
        }
    }

    const qreal interval = GetInterval(ratio) / MINOR_TICK_DIVISION;
    if (qFuzzyIsNull(interval)) {                                               /* 防止 interval 为0，导致死循环 */
        qDebug() << "Skipping ruler: interval is 0";
    } else {
        qreal tickLength = qMax<qreal>(MINOR_TICK_LENGTH, height * MINOR_TICK_RATIO);
        int index = qFloor(min / interval) + 1;
        for (qreal pos = interval * index; pos <= max; ++index, pos = interval * index) {
            const QPointF pixelPos = origin + (pos - min) * unit;
            if (index % MINOR_TICK_DIVISION == 0) {                                 /* 主刻度 */
                painter->setPen(majorColor);
                avoids << DrawMajorTick(painter, option, rect.height(), pos, pixelPos, boundingRect, avoids);
            } else {                                                                /* 副刻度 */
                painter->setPen(minorColor);
                painter->drawLine(pixelPos, pixelPos + option.tick * tickLength);
            }
        }

        painter->setPen(majorColor);
        painter->drawLine(origin, origin + rangeLength * unit);                     /* 绘制底线 */
        // 绘制光标位置
        if (Range(min, max).isInside(cursorPos)) {
            DrawMajorTick(painter, option, rect.height(), cursorPos, origin + unit * (cursorPos - min), boundingRect, QList<QRectF>(), Qt::lightGray);
        }
    }
    painter->restore();
}

void PlotterRuler::DrawGrid(QPainter *painter, const QRectF &rect) const
{
    const QTransform &transform = painter->transform();

    qreal interval = GetInterval(1 / transform.m11());
    int start = GetNextIndex(rect.left(), interval);
    int end = GetNextIndex(rect.right(), interval);
    for (int index = start; index < end; ++index) {
        qreal pos = interval * index;
        painter->drawLine(QLineF(pos, rect.top(), pos, rect.bottom()));         /* 此处若改为drawLine(x1, y1, x2, y2)，则参数将被转为int，导致绘图错误 */
    }

    interval = GetInterval(1 / transform.m22());
    start = GetNextIndex(rect.top(), interval);
    end = GetNextIndex(rect.bottom(), interval);
    for (int index = start; index < end; ++index) {
        qreal pos = interval * index;
        painter->drawLine(QLineF(rect.left(), pos, rect.right(), pos));
    }
}
