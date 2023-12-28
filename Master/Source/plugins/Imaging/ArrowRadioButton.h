#pragma once

#include <QRadioButton>
#include <QVector2D>
#include "ColorPalette.h"

/*!
 * \brief The ArrowRadioButton class
 * 调色板编辑器使用的控件，外观为小三角形，额外提供鼠标拖动和双击等消息
 */
class ArrowRadioButton : public QRadioButton
{
    Q_OBJECT

    Q_PROPERTY(qreal position READ position WRITE setPosition)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    explicit ArrowRadioButton(QWidget *parent = 0);
    qreal position() const;
    QColor color() const;

public slots:
    void setPosition(qreal pos);
    void setColor(const QColor &color);

private:
    bool event(QEvent *e) override;
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

    void UpdatePosition();

private:
    qreal m_position = 0;
    QColor m_color;
};
