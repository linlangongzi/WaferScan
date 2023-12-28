#include "AutoScaleLabel.h"
#include <QElapsedTimer>
#include <QLoggingCategory>
#include <QPainter>
#include <QStyle>

static Q_LOGGING_CATEGORY(debug, "motion.label", QtWarningMsg);

AutoScaleLabel::AutoScaleLabel(QWidget *parent) :
    QLabel(parent)
{

}

int AutoScaleLabel::minimumFontSize() const
{
    return m_minimumFontSize;
}

int AutoScaleLabel::maximumFontSize() const
{
    return m_maximumFontSize;
}

void AutoScaleLabel::setMinimumFontSize(int minimumFontSize)
{
    m_minimumFontSize = qMax(0, minimumFontSize);
    update();
}

void AutoScaleLabel::setMaximumFontSize(int maximumFontSize)
{
    m_maximumFontSize = qMax(0, maximumFontSize);
    update();
}

void AutoScaleLabel::paintEvent(QPaintEvent *)
{
    QElapsedTimer elapsed;
    elapsed.start();

    QPainter painter(this);
    QStyle *s = style();

    QFont font = this->font();
    m_prevFontSize = qBound(m_minimumFontSize, m_prevFontSize, m_maximumFontSize);
    font.setPixelSize(m_prevFontSize);

    QFontMetrics metrics(font, this);
    QString t = text();
    QSize fontSize = metrics.boundingRect(t).size();                            // 求出 text 在 font 字体下的 size
    QSize widgetSize = size();

    qCDebug(debug) << "Starting from" << m_prevFontSize;

    while (fontSize.boundedTo(widgetSize) == fontSize && font.pixelSize() < m_maximumFontSize) { // 当字符串的size小于控件size时对font的pixelSize加1再做判断
        qCDebug(debug) << fontSize << "==" << widgetSize << "-> font size +1";
        font.setPixelSize(font.pixelSize() + 1);
        metrics = QFontMetrics(font, this);
        fontSize = metrics.boundingRect(t).size();
    }
    while (fontSize.boundedTo(widgetSize) != fontSize && font.pixelSize() > m_minimumFontSize) { // 当字符串的size大于控件size时对font的pixelSize减1再做判断
        qCDebug(debug) << fontSize << "!=" << widgetSize << "-> font size -1";
        font.setPixelSize(font.pixelSize() - 1);
        metrics = QFontMetrics(font, this);
        fontSize = metrics.boundingRect(t).size();
    }

    m_prevFontSize = qBound(m_minimumFontSize, font.pixelSize(), m_maximumFontSize);

    qCDebug(debug) << "Finished at" << font.pixelSize() << "bound as" << m_prevFontSize;

    font.setPixelSize(m_prevFontSize);
    painter.setFont(font);
    s->drawItemText(&painter, contentsRect(), alignment(), palette(), isEnabled(), text(), foregroundRole());

    qCDebug(debug) << "Paint took" << elapsed.elapsed() << "ms";
}
