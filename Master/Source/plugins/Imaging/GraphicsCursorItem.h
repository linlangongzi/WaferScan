#pragma once

#include <QGraphicsItemGroup>

template<typename T>
class CursorPosition;
//class LineItem;
//class PointItem;

class GraphicsCursorItem : public QGraphicsItemGroup
{
    Q_GADGET

    Q_PROPERTY(QPointF cursorPosition READ cursorPosition WRITE setCursorPosition)
    Q_PROPERTY(QColor cursorColor READ cursorColor WRITE setCursorColor)
    Q_PROPERTY(Qt::Orientations orientations READ orientations WRITE setOrientations)

public:
    explicit GraphicsCursorItem(QGraphicsItem *parent = nullptr);
    ~GraphicsCursorItem() override;
    void setMenuCallback(const std::function<void ()> &callback);
    void setPositionCallback(const std::function<void (QPointF)> &callback);
    void setMovable(bool isMovable);

public:
    QPointF cursorPosition() const;
    Qt::Orientations orientations() const;
    QColor cursorColor() const;

public slots:
    void setCursorPosition(const QPointF &pos);
    void setOrientations(Qt::Orientations orientations);
    void setCursorColor(const QColor &color);

private:
    void updateVisibility();

protected:
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event) override;

private:
    QColor m_cursorColor;
    QPointF m_cursorPosition = {};
    Qt::Orientations m_orientations;

    std::function<void()> m_menuCallback;
    std::function<void(QPointF)> m_positionCallback;
    CursorPosition<QGraphicsLineItem> *m_cursorHorizonal = nullptr;
    CursorPosition<QGraphicsLineItem> *m_cursorVertical = nullptr;
    CursorPosition<QGraphicsRectItem> *m_cursorCrossPoint = nullptr;
};
