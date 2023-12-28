#pragma once

#include <QGraphicsScene>
#include "WaveformGlobal.h"

class QAction;

class WAVEFORM_EXPORT UTGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    UTGraphicsScene(QObject *parent = nullptr);

signals:
    void contextMenuRequest();          /*! \todo 移除 */
    void contextMenuTriggered(QList<QAction *> actionList);

public slots:
    void onManagerContextMenuTriggered(const QList<QAction *> &actionList);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QList<QAction *> actions;
};
