#include "ColorPalette.h"
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <functional>
#include <allrising/Util.h>

Q_DECL_UNUSED static const int typeId = qRegisterMetaType<ColorPalette>();

ColorPalette::ColorPalette(ColorPalette::DefaultGradient type)
{
    QLinearGradient g;
    switch (type) {
    case Gray:
        g.setColorAt(0, Qt::white);
        g.setColorAt(1, Qt::black);
        break;
    case Uit:
        g.setColorAt(0.00, Qt::black);
        g.setColorAt(0.25, 0x0000FF);
        g.setColorAt(0.50, 0x00FF00);
        g.setColorAt(0.75, 0xFF0000);
        g.setColorAt(1.00, 0xFFFF00);
        break;
    case Jet:
        g.setColorAt(0.0000, 0x00008F);
        g.setColorAt(0.1094, 0x0000FF);
        g.setColorAt(0.3594, 0x00FFFF);
        g.setColorAt(0.6094, 0xFFFF00);
        g.setColorAt(0.8594, 0xFF0000);
        g.setColorAt(1.0000, 0x800000);
        break;
    case Rainbow:
        g.setColorAt(1.0 / 15 * 0, QColor(255, 255, 255));
        g.setColorAt(1.0 / 15 * 1, QColor(183, 209, 236));
        g.setColorAt(1.0 / 15 * 2, QColor(116, 165, 215));
        g.setColorAt(1.0 / 15 * 3, QColor(68, 105, 178));
        g.setColorAt(1.0 / 15 * 4, QColor(43, 53, 134));
        g.setColorAt(1.0 / 15 * 5, QColor(42, 76, 126));
        g.setColorAt(1.0 / 15 * 6, QColor(66, 138, 120));
        g.setColorAt(1.0 / 15 * 7, QColor(129, 182, 120));
        g.setColorAt(1.0 / 15 * 8, QColor(203, 215, 69));
        g.setColorAt(1.0 / 15 * 9, QColor(232, 205, 66));
        g.setColorAt(1.0 / 15 * 10, QColor(216, 156, 87));
        g.setColorAt(1.0 / 15 * 11, QColor(204, 132, 92));
        g.setColorAt(1.0 / 15 * 12, QColor(201, 119, 62));
        g.setColorAt(1.0 / 15 * 13, QColor(189, 102, 44));
        g.setColorAt(1.0 / 15 * 14, QColor(163, 60, 44));
        g.setColorAt(1.0 / 15 * 15, QColor(142, 35, 46));
        break;
#ifdef PIPE_DETECT
    case Step:
        g.setColorAt(0.5, Qt::green);
        g.setColorAt(0.8, Qt::blue);
        g.setColorAt(1.0, Qt::red);
        m_type = StepGradient;
        break;
#endif
    }
    m_gradient = g;
    m_emptyColor = Qt::lightGray;
}

QColor ColorPalette::colorAt(double position, const QGradientStop &prev, const QGradientStop &next) const
{
    GradientType type = gradientType();
    if (type == LinearGradient) {
        qreal diff = qMax(next.first - prev.first, 1e-6);                    /* prev与next相同时，diff为0，将导致ratio为1.#INF */
        qreal ratio = (position - prev.first) / diff;
        qreal r = ratio * (next.second.red() - prev.second.red()) + prev.second.red();
        qreal g = ratio * (next.second.green() - prev.second.green()) + prev.second.green();
        qreal b = ratio * (next.second.blue() - prev.second.blue()) + prev.second.blue();
        qreal a = ratio * (next.second.alpha() - prev.second.alpha()) + prev.second.alpha();
        return QColor(qRound(r), qRound(g), qRound(b), qRound(a));
    } else if (type == StepGradient) {
        return next.second;
    }
    return QColor();
}

QColor ColorPalette::colorAt(double position) const
{
    const QGradientStops stops = m_gradient.stops();
    int index = 0;
    while (index < stops.count() && stops.at(index).first < position) {
        ++index;
    }

    if (index <= 0) {
        return colorAt(position, stops[0], stops[0]);
    } else if (index == stops.count()) {
        return colorAt(position, stops[index - 1], stops[index - 1]);
    } else {
        return colorAt(position, stops[index - 1], stops[index]);
    }
}

QColor ColorPalette::ToColor(const QJsonValue &json)
{
    return QColor(json.toString());
}

QJsonValue ColorPalette::FromColor(const QColor &color)
{
    return QJsonValue(color.name(QColor::HexArgb));
}

UTImagePalette ColorPalette::GetColorPalette(bool ignoreEmpty) const
{
    UTImagePalette palette;

    QGradientStops stops = m_gradient.stops();
    palette.colorTable.resize(256);
    qreal ratio = qreal(1) / palette.colorTable.size();
    int colorIndex = 0;                                                         /* index指向position之前的ColorItem */
    QGradientStop prev = stops.value(0);                                        /* index指向的item值 */
    QGradientStop next = prev;                                                  /* 下一个item值，初始化时默认按下溢处理 */
    for (int index = 0; index < palette.colorTable.count(); ++index) {
        double position = ratio * index;
        while ( position > next.first && colorIndex <= stops.count() ) {     /* 向后搜索position两边的ColorItem */
            colorIndex++;
            prev = next;
            next = stops[qMin(colorIndex, stops.count() - 1)];                /* colorIndex不会向下越界，但可能向上越界到colors.count() */
        }
        palette.colorTable[index] = qPremultiply(colorAt(position, prev, next).rgba());
    }

    if (!ignoreEmpty) {
        palette.colorTable[0] = qPremultiply(m_emptyColor.rgba());
    }
    palette.min = m_min;
    palette.max = m_max;

    return palette;
}

QLinearGradient ColorPalette::gradient() const
{
    return m_gradient;
}

void ColorPalette::setGradient(QLinearGradient gradient)
{
    m_gradient = gradient;
}

ColorPalette::GradientType ColorPalette::gradientType() const
{
    return m_type;
}

QColor ColorPalette::emptyColor() const
{
    return m_emptyColor;
}

qreal ColorPalette::minValue() const
{
    return m_min;
}

qreal ColorPalette::maxValue() const
{
    return m_max;
}

bool ColorPalette::isAutoMax() const
{
    return m_autoMax;
}

bool ColorPalette::isAutoMin() const
{
    return m_autoMin;
}

void ColorPalette::setGradientType(ColorPalette::GradientType newType)
{
    m_type = newType;
}

void ColorPalette::setEmptyColor(const QColor &emptyColor)
{
    m_emptyColor = emptyColor;
}

void ColorPalette::setMinValue(qreal min)
{
    m_min = min;
}

void ColorPalette::setMaxValue(qreal max)
{
    m_max = max;
}

void ColorPalette::setAutoMax(bool upperLimit)
{
    m_autoMax = upperLimit;
}

void ColorPalette::setAutoMin(bool lowerLimit)
{
    m_autoMin = lowerLimit;
}

void ColorPalette::removeStop(int index)
{
    QGradientStops stops = m_gradient.stops();
    if (uint(index) < uint(stops.count())) {
        stops.remove(index);
        m_gradient.setStops(stops);
    }
}

void ColorPalette::setStop(int index, qreal position, const QColor &color)
{
    QGradientStops stops = m_gradient.stops();
    if (uint(index) < uint(stops.count())) {
        stops[index] = qMakePair(position, color);
        m_gradient.setStops(stops);
    }
}

void ColorPalette::paint(QPainter *painter, const QRect &rect, Qt::Orientation orientation, bool isReversed)
{
    if (m_type == StepGradient) {
        const QGradientStops stops = m_gradient.stops();
        if (orientation == Qt::Horizontal) {
            int length = rect.width();
            int lastEnd = 0;
            for (const QGradientStop &stop: stops) {
                int pos = length * stop.first;
                QRect ladderRect(lastEnd, rect.top(), pos - lastEnd, rect.height());
                lastEnd = pos;
                painter->setBrush(stop.second);
                painter->drawRect(ladderRect);
            }
            painter->drawRect(lastEnd, rect.top(), rect.width() - lastEnd, rect.height());
        } else {
            int top = rect.top();
            int bottom = rect.bottom();
            int lastEnd = bottom + 1;
            for (const QGradientStop &stop: stops) {
                int pos = rect.height() * (1 - stop.first);
                QRect ladderRect(rect.left(), pos + top, rect.width(), lastEnd - (pos + top));
                lastEnd = ladderRect.top();
                painter->setBrush(stop.second);
                painter->drawRect(ladderRect);
            }
            painter->drawRect(rect.left(), rect.top(), rect.width(), lastEnd - rect.top());
        }
    } else {
        QLinearGradient gradient = m_gradient;
        QPoint start = rect.topLeft();
        QPoint stop = (orientation == Qt::Vertical) ? rect.bottomLeft() : rect.topRight();
        if (isReversed) {
            qSwap(start, stop);
        }
        gradient.setStart(start);
        gradient.setFinalStop(stop);

        gradient.setCoordinateMode(QGradient::LogicalMode);
        painter->fillRect(rect, gradient);
    }
}

QJsonObject ColorPalette::toJson() const
{
    QJsonObject obj;
    QJsonArray array;
    for (const QGradientStop &stop: m_gradient.stops()) {
        QJsonObject color;
        color["Color"] = FromColor(stop.second);
        color["Position"] = stop.first * 100;
        array << color;
    }
    obj["ColorPalette"] = array;
    obj["MinLimit"] = m_min;
    obj["MaxLimit"] = m_max;
    obj["AutoMax"] = m_autoMax;
    obj["AutoMin"] = m_autoMin;
    obj["EmptyColor"] = FromColor(m_emptyColor);
    obj["GradientType"] = EnumToKey(m_type);
    return obj;
}

ColorPalette ColorPalette::fromJson(const QJsonValue &json)
{
    QJsonObject palette = json.toObject();
    if (palette.isEmpty()) {
        return ColorPalette();
    }

    ColorPalette colorPalette;
    QJsonArray array = palette["ColorPalette"].toArray();
    QLinearGradient gradient;
    for (const QJsonValue &value: array) {
        QJsonObject tapes = value.toObject();
        gradient.setColorAt(tapes["Position"].toDouble() / 100, ToColor(tapes["Color"]));
        colorPalette.m_gradient = gradient;
    }

    colorPalette.m_min = palette["MinLimit"].toDouble();
    colorPalette.m_max = palette["MaxLimit"].toDouble();
    colorPalette.m_autoMax = palette["AutoMax"].toBool();
    colorPalette.m_autoMin = palette["AutoMin"].toBool();
    colorPalette.m_emptyColor = ToColor(palette["EmptyColor"]);
    colorPalette.m_type = KeyToEnum(palette["GradientType"].toString(), LinearGradient);

    return colorPalette;
}

bool ColorPalette::operator ==(const ColorPalette &other) const
{
    return m_gradient == other.m_gradient
            && m_type == other.m_type
            && m_emptyColor == other.m_emptyColor
            && m_autoMax == other.m_autoMax
            && m_autoMin == other.m_autoMin
            && qFuzzyCompare(m_min, other.m_min)
            && qFuzzyCompare(m_max, other.m_max);
}

bool ColorPalette::operator !=(const ColorPalette &other) const
{
    return m_gradient != other.m_gradient
            || m_type != other.m_type
            || m_emptyColor != other.m_emptyColor
            || m_autoMax != other.m_autoMax
            || m_autoMin != other.m_autoMin
            || !qFuzzyCompare(m_min, other.m_min)
            || !qFuzzyCompare(m_max, other.m_max);
}
