#include "GraphicsCursorManager.h"
#include "PanelCScan.h"
#include <QLoggingCategory>
#include <QMenu>

static Q_LOGGING_CATEGORY(debug, "image.cursorManager")

GraphicsCursorManager::GraphicsCursorManager(QObject *parent) :
    QObject(parent)
{
}

GraphicsCursorManager::~GraphicsCursorManager()
{
    delete m_cursorCScan;
    m_cursorCScan = nullptr;
    delete m_cursorSliceX;
    m_cursorSliceX = nullptr;
    delete m_cursorSliceY;
    m_cursorSliceY = nullptr;
}

void GraphicsCursorManager::createItems(QGraphicsItem *cScan, QGraphicsItem *sliceX, QGraphicsItem *sliceY)
{
    m_cursorCScan = new GraphicsCursorItem(cScan);
    m_cursorSliceX = new GraphicsCursorItem(sliceX);
    m_cursorSliceY = new GraphicsCursorItem(sliceY);
}

QPointF GraphicsCursorManager::cursorPosition() const
{
    return m_cursorPosition;
}

QColor GraphicsCursorManager::cursorColor() const
{
    return m_cursorColor;
}

bool GraphicsCursorManager::isMenuEnable() const
{
    return m_isMenuEnable;
}

bool GraphicsCursorManager::isCursorVisible() const
{
    return m_isSliceVisible;
}

bool GraphicsCursorManager::isMovable() const
{
    return m_isMovable;
}

void GraphicsCursorManager::setCursorPosition(const QPointF &cursorPos)
{
    QPointF pos = {cursorPos.x(), -cursorPos.y()};
    if (m_cursorPosition == pos) {
        return;
    }
    m_cursorPosition = pos;
    m_cursorCScan->setCursorPosition(m_cursorPosition);
    m_cursorSliceX->setCursorPosition(m_cursorPosition);
    m_cursorSliceY->setCursorPosition(m_cursorPosition);
}

void GraphicsCursorManager::setMenuEnable(bool enable)
{
    if (m_isMenuEnable == enable) {
        return;
    }
    m_isMenuEnable = enable;
    if (m_isMenuEnable) {
        m_cursorCScan->setMenuCallback([this]() {
            QMenu menu;
            menu.addAction(tr("删除"), this, [=]() {
                emit referCursorChanged();
            });
            menu.exec(QCursor::pos());
        });
        m_cursorSliceX->setMenuCallback([this]() {
            QMenu menu;
            menu.addAction(tr("删除"), this, [=]() {
                emit referCursorChanged();
            });
            menu.exec(QCursor::pos());
        });
        m_cursorSliceY->setMenuCallback([this]() {
            QMenu menu;
            menu.addAction(tr("删除"), this, [=]() {
                emit referCursorChanged();
            });
            menu.exec(QCursor::pos());
        });
    }
}

void GraphicsCursorManager::setCursorColor(const QColor &color)
{
    if (m_cursorColor == color) {
        return;
    }
    m_cursorColor = color;
    m_cursorCScan->setCursorColor(m_cursorColor);
    m_cursorSliceX->setCursorColor(m_cursorColor);
    m_cursorSliceY->setCursorColor(m_cursorColor);
}

void GraphicsCursorManager::setCursorVisible(bool isSliceVisible)
{
    if (m_isSliceVisible == isSliceVisible) {
        return;
    }
    m_isSliceVisible = isSliceVisible;
    if (!m_isSliceVisible) {
        m_cursorSliceX->setOrientations({});
        m_cursorSliceY->setOrientations({});
    } else {
        m_cursorSliceX->setOrientations(Qt::Vertical);
        m_cursorSliceY->setOrientations(Qt::Horizontal);
    }
    m_cursorCScan->setOrientations({Qt::Horizontal, Qt::Vertical});
}

void GraphicsCursorManager::setMovable(bool isMovable)
{
    if (m_isMovable == isMovable) {
        return;
    }
    m_isMovable = isMovable;
    m_cursorCScan->setMovable(isMovable);
    m_cursorSliceX->setMovable(isMovable);
    m_cursorSliceY->setMovable(isMovable);
    if (isMovable) {
        m_cursorCScan->setPositionCallback([=](const QPointF &newPos){
            m_cursorSliceX->setCursorPosition(newPos);
            m_cursorSliceY->setCursorPosition(newPos);
        });
        m_cursorSliceX->setPositionCallback([=](const QPointF &newPos){
            m_cursorCScan->setCursorPosition({newPos.x(), m_cursorCScan->cursorPosition().y()});
        });
        m_cursorSliceY->setPositionCallback([=](const QPointF &newPos){
            m_cursorCScan->setCursorPosition({m_cursorCScan->cursorPosition().x(), newPos.y()});
        });
    }
}
