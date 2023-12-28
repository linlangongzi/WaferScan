#include "ArrowRadioButton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

const int arrowSize = 8;
const int arrowLength = 30;
const QPolygon arrowUp = QPolygon()
        << QPoint(arrowSize, 1)
        << QPoint(arrowSize * 2 - 1, arrowSize)
        << QPoint(arrowSize * 2 - 1, arrowLength)
        << QPoint(1, arrowLength)
        << QPoint(1, arrowSize);

ArrowRadioButton::ArrowRadioButton(QWidget *parent) :
    QRadioButton(parent)
{
    if (parent) {
        parent->installEventFilter(this);
    }
    setFixedSize(arrowUp.boundingRect().size());
    setMask(QRegion(arrowUp));                                                /* 使控件在三角形之外不接受鼠标事件 */
    setContextMenuPolicy(Qt::CustomContextMenu);                                /* 启用右键上下文菜单信号，将抛出customContextMenuRequested信号 */
    repaint();
}

qreal ArrowRadioButton::position() const
{
    return m_position;
}

QColor ArrowRadioButton::color() const
{
    return m_color;
}

void ArrowRadioButton::setPosition(qreal pos)
{
    m_position = pos;
    UpdatePosition();
}

void ArrowRadioButton::setColor(const QColor &color)
{
    if (m_color == color) {
        return;
    }
    m_color = color;
    update();
}

bool ArrowRadioButton::event(QEvent *e)
{
    if (e->type() == QEvent::ParentAboutToChange && parent()) {
        parent()->removeEventFilter(this);
    } else if (e->type() == QEvent::ParentChange && parent()) {
        parent()->installEventFilter(this);
    }
    return QRadioButton::event(e);
}

void ArrowRadioButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setBrush(QPixmap(":/TransparentBackground.png"));
    painter.drawPolygon(arrowUp);
    painter.setBrush(m_color);
    painter.drawPolygon(arrowUp);

    painter.setBrush(Qt::NoBrush);
//    painter.setRenderHint(QPainter::Antialiasing);
    if (isChecked()) {
        painter.setPen(QPen(Qt::white, 8));
        painter.drawPolygon(arrowUp);
        painter.setPen(QPen(Qt::black, 6));
        painter.drawPolygon(arrowUp);
    }
    painter.setPen(QPen(Qt::white, 4));
    painter.drawPolygon(arrowUp);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawPolygon(arrowUp);
}

bool ArrowRadioButton::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == parentWidget() && event->type() == QEvent::Resize) {
        UpdatePosition();
    }
    return QRadioButton::eventFilter(obj, event);
}

void ArrowRadioButton::UpdatePosition()
{
    const QRect rect = parentWidget()->contentsRect();
    move(rect.left() + rect.width() * m_position - arrowUp.value(0).x(), 0);
}
