#include "MachinePart.h"
#include "OsgManipulator.h"
#include "PointMt.h"
#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/Viewer>
#include <QVector3D>
#include <QVector4D>
#include "qtosgUtils.h"

using namespace osg;
using namespace osgGA;

OsgManipulator::OsgManipulator(OsgContainer *viewer) :
    container(viewer)
{
}

bool OsgManipulator::performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    doDrag(options.dragLeft, eventTimeDelta, dx, dy);
    return false;
}

/*! 重载鼠标中键拖动事件，重载为旋转 */
bool OsgManipulator::performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    doDrag(options.dragMiddle, eventTimeDelta, dx, dy);
    return false;                                                               /*!< 返回false可禁用拖动完成后的惯性效果 */
}

/*! 重载鼠标右键拖动事件，重载为平移 */
bool OsgManipulator::performMovementRightMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
    doDrag(options.dragRight, eventTimeDelta, dx, dy);
    return false;                                                               /*!< 返回false可禁用拖动完成后的惯性效果 */
}

bool OsgManipulator::handleMouseMove(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
    auto item = Pick(ea);
    container->setHoverItem(item.first);
    return inherited::handleMouseMove(ea, us);
}

bool OsgManipulator::handleMousePush(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
    if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON
        || ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
        posX = ea.getX();
        posY = ea.getY();
    }
    return inherited::handleMousePush(ea, us);
}

bool OsgManipulator::handleMouseRelease(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
    if (ea.getX() == posX && ea.getY() == posY) {
        if (ea.getButton() == GUIEventAdapter::LEFT_MOUSE_BUTTON) {
            doClick(options.clickLeft, ea, us);
        } else if (ea.getButton() == GUIEventAdapter::MIDDLE_MOUSE_BUTTON) {
            doClick(options.clickMiddle, ea, us);
        } else if (ea.getButton() == GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
            doClick(options.clickRight, ea, us);
        }
    }
    return inherited::handleMouseRelease(ea, us);
}

void OsgManipulator::doDrag(ManipulateOptions::Drag drag, double eventTimeDelta, double dx, double dy)
{
    switch (drag) {
    case ManipulateOptions::DragPan:
        inherited::performMovementMiddleMouseButton(eventTimeDelta, dx, dy);
        break;
    case ManipulateOptions::DragRotate:
        inherited::performMovementLeftMouseButton(eventTimeDelta, dx, dy);
        break;
    case ManipulateOptions::DragZoom:
        inherited::performMovementRightMouseButton(eventTimeDelta, dx, dy);
        break;
    }
}

void OsgManipulator::doClick(ManipulateOptions::Click click, const GUIEventAdapter &ea, GUIActionAdapter &us)
{
    Q_UNUSED(us);
    switch (click) {
    case ManipulateOptions::ClickSelect: {
        auto item = Pick(ea);
        container->setSelectedItem(item.first);
        break;
    }
    case ManipulateOptions::ClickContextMenu: {
        auto item = Pick(ea);
        emit ContextMenu(item.second, item.first);    //item可以是空指针,右键菜单信号
        break;
    }
    case ManipulateOptions::ClickHome: {
        container->home();
        break;
    }
    case ManipulateOptions::ClickHomeItem: {
        break;
    }
    }
}

bool OsgManipulator::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
    if (ea.getEventType() == osgGA::GUIEventAdapter::DOUBLECLICK) {
        if (ea.getButton() == GUIEventAdapter::LEFT_MOUSE_BUTTON) {
            doClick(options.doubleLeft, ea, us);
        } else if (ea.getButton() == GUIEventAdapter::MIDDLE_MOUSE_BUTTON) {
            doClick(options.doubleMiddle, ea, us);
        } else if (ea.getButton() == GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
            doClick(options.doubleRight, ea, us);
        }
    }
    return inherited::handle(ea, us);
}

QPair<Item3d*, QVector3D> OsgManipulator::Pick(const osgGA::GUIEventAdapter &ea)
{
    osgUtil::LineSegmentIntersector::Intersections intersections;

    if (container->computeIntersections(ea, intersections)) {
        for (const auto &intersection: intersections) {
            if (intersection.nodePath.empty()) {
                continue;
            }
            for (int i = intersection.nodePath.size() - 1; i >= 0; --i) {
                Item3d *item = dynamic_cast<Item3d *>(intersection.nodePath[i]);
                if (item) {
                    return QPair<Item3d*, QVector3D>(item, toVector3D(intersection.localIntersectionPoint));
                }
            }
        }
    }

    return QPair<Item3d*, QVector3D>();
}
