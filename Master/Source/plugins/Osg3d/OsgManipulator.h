#pragma once

#include "Item3d.h"

#include <osgGA/OrbitManipulator>
#include <QDebug>
#include <QObject>
#include <osgViewer/Viewer>
#include "OsgContainer.h"

class OsgManipulator : public QObject, public osgGA::OrbitManipulator
{
    Q_OBJECT

    typedef osgGA::OrbitManipulator inherited;

public:
    OsgManipulator(OsgContainer *viewer);

    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) override;                               //其它事件

protected:
    virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy) override;
    virtual bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy) override;     //旋转
    virtual bool performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy) override;      //平移
    bool handleMouseMove(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) override;
    bool handleMousePush(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) override;
    bool handleMouseRelease(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us) override;

    void doDrag(ManipulateOptions::Drag drag, double eventTimeDelta, double dx, double dy);
    void doClick(ManipulateOptions::Click click, const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);

    QPair<Item3d *, QVector3D> Pick(const osgGA::GUIEventAdapter &ea);                                             //点选一个Item3d对象

signals:
    void ContextMenu(const QVector3D &intersection, Item3d *item);                   //右键菜单事件

protected:
    ManipulateOptions options;
    float posX;                                                                 //鼠标位置
    float posY;
    OsgContainer *container;
//    Item3d *lastMoveItem = nullptr;
//    Item3d *lastSelectItem = nullptr;
};
