#pragma once

#include "Item3d.h"
#include <QAction>

class MachinePart : public Item3d
{
    Q_OBJECT

    Q_PROPERTY(bool transparent READ isTransparent WRITE setTransparent)

public:
    MachinePart(osg::Node *node);
    virtual void ContextMenu(QMenu *menu, const QVector3D &p) override;

    bool isTransparent() const;

public slots:
    void setTransparent(bool transparent);

private:
    bool m_transparent = false;
};
