#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <Waveform/AbstractGraphicsItemManager.h>

class UTGraphicsScene;

class CursorManager : public AbstractGraphicsItemManager
{
    Q_OBJECT

public:
    CursorManager(QGraphicsItem *rootItem, QObject *parent);

    void SetCursorOrientations(Qt::Orientations arg);
    Qt::Orientations GetOrientations() const {return orientations;}

public slots:
    void SetCursorPos(const QPointF &scenePos);

protected slots:
    void updateVisibility();

private:
    QGraphicsLineItem *verticalLine;
    QGraphicsLineItem *horizontalLine;
    Qt::Orientations orientations;
};
