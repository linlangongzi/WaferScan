#pragma once

#include "Item3d.h"
#include <QAction>
#include <QVector3D>

class PointMt : public Item3d
{
    Q_OBJECT
public:
    PointMt(const QVector3D &p, Node *node, qreal pointSize = 5);
    QVector3D GetPosition();
    void setPosition(const QVector3D &pos);
    bool PositionChanged();

    virtual void ContextMenu(QMenu *menu, const QVector3D &p);

private slots:
    void DraggerTranslate();

private:
    QAction *draggerTranslate;
    QVector3D postionPrevious;
};
