#pragma once

#include <osgViewer/Viewer>
#include <QOpenGLWidget>
#include "Item3d.h"

class ManipulateOptions
{
    Q_GADGET

    Q_PROPERTY(Drag dragLeft MEMBER dragLeft)
    Q_PROPERTY(Drag dragMiddle MEMBER dragMiddle)
    Q_PROPERTY(Drag dragRight MEMBER dragRight)
    Q_PROPERTY(Click clickLeft MEMBER clickLeft)
    Q_PROPERTY(Click clickMiddle MEMBER clickMiddle)
    Q_PROPERTY(Click clickRight MEMBER clickRight)
    Q_PROPERTY(Click doubleLeft MEMBER doubleLeft)
    Q_PROPERTY(Click doubleMiddle MEMBER doubleMiddle)
    Q_PROPERTY(Click doubleRight MEMBER doubleRight)

public:
    enum Drag {
        DragNone,
        DragRotate,
        DragPan,
        DragZoom,
        DragSelect,
    };
    Q_ENUM(Drag)

    enum Click {
        ClickNone,
        ClickSelect,
        ClickContextMenu,
        ClickHome,
        ClickHomeItem,
    };
    Q_ENUM(Click)

    Drag dragLeft = DragNone;
    Drag dragMiddle = DragRotate;
    Drag dragRight = DragPan;
    Click clickLeft = ClickSelect;
    Click clickMiddle = ClickNone;
    Click clickRight = ClickContextMenu;
    Click doubleLeft = ClickNone;
    Click doubleMiddle = ClickHome;
    Click doubleRight = ClickNone;
};

class QInputEvent;
class OsgManipulator;

class LightConfig
{
    Q_GADGET

    Q_PROPERTY(QColor color MEMBER color)
    Q_PROPERTY(qreal ambient MEMBER ambient)
    Q_PROPERTY(qreal brightness MEMBER brightness)
    Q_PROPERTY(qreal specularity MEMBER specularity)
    Q_PROPERTY(bool isRelative MEMBER isRelative)
    Q_PROPERTY(qreal longitude MEMBER longitude)
    Q_PROPERTY(qreal latitude MEMBER latitude)

public:
    QColor color;
    qreal ambient = 0;
    qreal brightness = 0;
    qreal specularity = 0;

    bool enabled = true;
    bool isRelative = false;
    qreal longitude = 0;
    qreal latitude = 0;
};

class OsgContainer : public QOpenGLWidget, public osgViewer::Viewer
{
    Q_OBJECT

public:
    OsgContainer(QWidget *parent = 0);
    ~OsgContainer();

    bool event(QEvent* event);

    void setKeyboardModifiers(QInputEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent* event);
    void timerEvent(QTimerEvent *);
    void setSelectedItem(Item3d *item);
    void setHoverItem(Item3d *item);

    osgViewer::Viewer *getOSGViewer() { return this; }
    osg::Group *getRoot(){ return root.get(); }

protected:
    virtual void paintGL();

protected slots:
    void ContextMenu(const QVector3D &intersection, Item3d *item);              //处理右键菜单
    void ZoomFit();

private:
    void init3D();
    osg::ref_ptr<osg::Camera> createCamera(int x, int y, int w, int h);
    void createLight(const LightConfig &lightConfig);

private:
    friend class SelectedObserver;
    friend class HoverObserver;

    osg::ref_ptr<osg::Group> root;
    osg::ref_ptr<osgViewer::GraphicsWindow> window;
    OsgManipulator *manipulator;
    Item3d *itemSelected = NULL;
    Item3d *itemHovered = NULL;
    SelectedObserver *itemSelectedObserver = NULL;
    HoverObserver *itemHoveredObserver = NULL;
};
