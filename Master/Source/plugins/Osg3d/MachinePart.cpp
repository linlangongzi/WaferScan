#include "MachinePart.h"
#include "Machine.h"
#include <osg/StateSet>
#include <QMenu>
#include <algorithm>
#include "qtosgUtils.h"

using namespace osg;

MachinePart::MachinePart(Node *node)
{
    addChild(node);
}

void MachinePart::ContextMenu(QMenu *menu, const QVector3D &p)
{
    QAction *transparent = menu->addAction(tr("透明"), this, &MachinePart::setTransparent);
    transparent->setCheckable(true);
    transparent->setChecked(isTransparent());

    return Item3d::ContextMenu(menu, p);
}

bool MachinePart::isTransparent() const
{
    return m_transparent;
}

void MachinePart::setTransparent(bool transparent)
{
    m_transparent = transparent;
//    setAlpha(transparent ? 0.1 : 1);
//    setOverrideColor(transparent ? QColor(Qt::red) : QColor());
}
