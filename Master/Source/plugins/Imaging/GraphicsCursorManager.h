#pragma once

#include "GraphicsCursorItem.h"

class GraphicsCursorManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QPointF cursorPosition READ cursorPosition WRITE setCursorPosition)
    Q_PROPERTY(QColor cursorColor READ cursorColor WRITE setCursorColor)
    Q_PROPERTY(bool isMenuEnable READ isMenuEnable WRITE setMenuEnable)
    Q_PROPERTY(bool isCursorVisible READ isCursorVisible WRITE setCursorVisible)
    Q_PROPERTY(bool isMovable READ isMovable WRITE setMovable)

public:
    GraphicsCursorManager(QObject *parent = nullptr);
    ~GraphicsCursorManager() override;
    void createItems(QGraphicsItem *cScan, QGraphicsItem *sliceX, QGraphicsItem *sliceY);

public:
    QPointF cursorPosition() const;
    QColor cursorColor() const;
    bool isMenuEnable() const;
    bool isCursorVisible() const;
    bool isMovable() const;

public slots:
    void setCursorColor(const QColor &color);
    void setCursorPosition(const QPointF &cursorPos);
    void setMenuEnable(bool enable);
    void setCursorVisible(bool isSliceVisible);
    void setMovable(bool isMovable);

signals:
    void referCursorChanged();

private:
    bool m_isMenuEnable;
    bool m_isSliceVisible;
    bool m_isMovable;
    QColor m_cursorColor;
    QPointF m_cursorPosition = {};

    GraphicsCursorItem *m_cursorCScan = nullptr;
    GraphicsCursorItem *m_cursorSliceX = nullptr;
    GraphicsCursorItem *m_cursorSliceY = nullptr;
};

