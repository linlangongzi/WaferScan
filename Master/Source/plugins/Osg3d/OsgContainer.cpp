#include "Machine.h"
#include "MachinePart.h"
#include "OsgContainer.h"
#include "OsgManipulator.h"
#include "PointMt.h"
#include <osg/MatrixTransform>
#include <osg/Notify>
#include <osgDB/ReadFile>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <QApplication>
#include <QInputEvent>
#include <QMenu>
#include <QVector3D>
#include <osgFX/Outline>
#include "qtosgUtils.h"

using namespace osg;

class SelectedObserver : public osg::Observer
{
public:
    OsgContainer *container = NULL;

    void objectDeleted(void *item)
    {
        if (item != static_cast<Referenced *>(container->itemSelected)) {
            qWarning() << "Selected item mismatch" << item << container->itemSelected;
            return;
        }
        if (!container->itemSelected || container->itemSelected->getNumParents() == 0) {
            qWarning() << "Selected item parent mismatch";
            return;
        }
        Group *highlight = container->itemSelected->getParent(0);
        highlight->getParent(0)->removeChild(highlight);
        container->itemSelected = NULL;
        container->itemSelectedObserver = NULL;
    }
};

class HoverObserver : public osg::Observer
{
public:
    OsgContainer *container = NULL;

    void objectDeleted(void *item)
    {
        if (item != static_cast<Referenced *>(container->itemHovered)) {
            qWarning() << "Hovered item mismatch" << item << container->itemHovered;
            return;
        }
        if (!container->itemHovered || container->itemHovered->getNumParents() == 0) {
            qWarning() << "Hovered item parent mismatch";
            return;
        }
        Group *highlight = container->itemHovered->getParent(0);
        highlight->getParent(0)->removeChild(highlight);
        container->itemHovered = NULL;
        container->itemHoveredObserver = NULL;                                  /* 这个Observer即将被析构 */
    }
};

OsgContainer::OsgContainer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setNotifyLevel(FATAL);
    init3D();
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    manipulator = new OsgManipulator(this);
    setCameraManipulator(manipulator);
    connect(manipulator, &OsgManipulator::ContextMenu, this, &OsgContainer::ContextMenu);
    root->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::ON);
    //创建光照
    LightConfig light;
    light.color = Qt::white;
    light.ambient = 0;
    light.brightness = 0.84;
    light.specularity = 0.47;
    light.longitude = -32.42;
    light.latitude = 54.01;
    createLight(light);

    light.color = Qt::white;
    light.ambient = 0;
    light.brightness = 0.23;
    light.specularity = 0.59;
    light.longitude = -79.22;
    light.latitude = -28.81;
    createLight(light);

    light.color = QColor(241, 191, 249);
    light.ambient = 0;
    light.brightness = 0.28;
    light.specularity = 0.36;
    light.longitude = 93.62;
    light.latitude = -18;
    createLight(light);

    light.color = Qt::white;
    light.ambient = 0;
    light.brightness = 0.25;
    light.specularity = 0;
    light.longitude = 78.69;
    light.latitude = 55.95;
    createLight(light);
}

OsgContainer::~OsgContainer()
{

}

bool OsgContainer::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchUpdate: {
        QList<QTouchEvent::TouchPoint> touchPoints = static_cast<QTouchEvent *>(event)->touchPoints();
        unsigned int id = 0;
        unsigned int tapCount = touchPoints.size();

        ref_ptr<osgGA::GUIEventAdapter> osgEvent(NULL);
        osgGA::GUIEventAdapter::TouchPhase phase = osgGA::GUIEventAdapter::TOUCH_UNKNOWN;
        foreach (const QTouchEvent::TouchPoint& touchPoint, touchPoints) {
            if (!osgEvent) {
                if (event->type() == QEvent::TouchBegin) {
                    phase = osgGA::GUIEventAdapter::TOUCH_BEGAN;
                    osgEvent = window->getEventQueue()->touchBegan(id, osgGA::GUIEventAdapter::TOUCH_BEGAN, touchPoint.pos().x(), touchPoint.pos().y());
                } else if (event->type() == QEvent::TouchEnd) {
                    phase = osgGA::GUIEventAdapter::TOUCH_ENDED;
                    osgEvent = window->getEventQueue()->touchEnded(id, osgGA::GUIEventAdapter::TOUCH_ENDED, touchPoint.pos().x(), touchPoint.pos().y(), tapCount);
                } else if (event->type() == QEvent::TouchUpdate) {
                    phase = osgGA::GUIEventAdapter::TOUCH_MOVED;
                    osgEvent = window->getEventQueue()->touchMoved(id, osgGA::GUIEventAdapter::TOUCH_MOVED, touchPoint.pos().x(), touchPoint.pos().y());
                }
            } else {
                osgEvent->addTouchPoint(id, osgGA::GUIEventAdapter::TOUCH_ENDED, touchPoint.pos().x(), touchPoint.pos().y());
                osgEvent->addTouchPoint(id, phase, touchPoint.pos().x(), touchPoint.pos().y());
            }
            id++;
        }
        break;
    }
    default:
        break;
    }
    return QOpenGLWidget::event(event);
}

void OsgContainer::setKeyboardModifiers(QInputEvent *event)
{
    int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
    unsigned int mask = 0;
    if (modkey & Qt::ShiftModifier) {
        mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    }
    if (modkey & Qt::ControlModifier) {
        mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    }
    if (modkey & Qt::AltModifier) {
        mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    }

    window->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
    update();
}

void OsgContainer::keyPressEvent(QKeyEvent *event)
{
    setKeyboardModifiers(event);
    window->getEventQueue()->keyPress(event->key());
    QOpenGLWidget::keyPressEvent(event);
    update();
}

void OsgContainer::keyReleaseEvent(QKeyEvent *event)
{
    setKeyboardModifiers(event);
    window->getEventQueue()->keyRelease(event->key());
    QOpenGLWidget::keyReleaseEvent(event);
    update();
}

void OsgContainer::mousePressEvent(QMouseEvent *event)
{
    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton: button = 1; break;
    case Qt::MidButton: button = 2; break;
    case Qt::RightButton: button = 3; break;
    case Qt::NoButton: button = 0; break;
    default: button = 0; break;
    }
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
    update();
}

void OsgContainer::mouseReleaseEvent(QMouseEvent *event)
{
    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton: button = 1; break;
    case Qt::MidButton: button = 2; break;
    case Qt::RightButton: button = 3; break;
    case Qt::NoButton: button = 0; break;
    default: button = 0; break;
    }
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);

    QOpenGLWidget::mouseReleaseEvent(event);
    update();
}

void OsgContainer::mouseDoubleClickEvent(QMouseEvent *event)
{
    int button = 0;
    switch (event->button()) {
    case Qt::LeftButton: button = 1; break;
    case Qt::MidButton: button = 2; break;
    case Qt::RightButton: button = 3; break;
    case Qt::NoButton: button = 0; break;
    default: button = 0; break;
    }
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);

    QOpenGLWidget::mouseDoubleClickEvent(event);
    update();
}

void OsgContainer::mouseMoveEvent(QMouseEvent *event)
{
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseMotion(event->x(), event->y());
    QOpenGLWidget::mouseMoveEvent(event);
    update();
}

void OsgContainer::wheelEvent(QWheelEvent *event)
{
    setKeyboardModifiers(event);
    window->getEventQueue()->mouseScroll(
                event->orientation() == Qt::Vertical ?
                    (event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
                    (event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT));
    QOpenGLWidget::wheelEvent(event);
    update();
}

void OsgContainer::resizeEvent(QResizeEvent *event)
{
    const QSize &size = event->size();
    window->resized(x(), y(), size.width(), size.height());
    window->getEventQueue()->windowResize(x(), y(), size.width(), size.height());
    window->requestRedraw();

//    const QSize& oldSize = event->oldSize();
//    int oldWidth = oldSize.width();
//    int oldHeight = oldSize.height();

//    int newWidth = size.width();
//    int newHeight = size.height();

//    double widthChangeRatio = double(newWidth) / double(oldWidth);
//    double heigtChangeRatio = double(newHeight) / double(oldHeight);
//    double aspectRatioChange = widthChangeRatio / heigtChangeRatio;
    QOpenGLWidget::resizeEvent(event);
}

void OsgContainer::moveEvent(QMoveEvent *event)
{
    const QPoint& pos = event->pos();
    window->resized(pos.x(), pos.y(), width(), height());
    window->getEventQueue()->windowResize(pos.x(), pos.y(), width(), height());

    QOpenGLWidget::moveEvent(event);
}

void OsgContainer::timerEvent(QTimerEvent *)
{
    update();
}

void OsgContainer::setSelectedItem(Item3d *item)
{
    if (itemSelected == item) {
        return;
    }
    ref_ptr<Group> highlight;
    ref_ptr<Item3d> i = item;
    ref_ptr<Item3d> prevSelected = itemSelected;
    Q_UNUSED(prevSelected);
    if (itemSelected) {
        if (itemSelected->getNumParents() > 0) {
            highlight = itemSelected->getParent(0);
            highlight->removeChild(itemSelected);
            ref_ptr<Group> parent = highlight->getParent(0);
            parent->replaceChild(highlight, itemSelected);
        }
        itemSelected->removeObserver(itemSelectedObserver);
    }
    if (item) {
        if (!highlight) {
            osgFX::Outline *outline = new osgFX::Outline;
            outline->setName("SelectProxy");
            outline->setColor(toVec4(Qt::green));
            outline->setWidth(5);
            outline->setEnabled(true);
            highlight = outline;
        }
        ref_ptr<Group> parent = i->getParent(0);
        if (!parent) {
            return;
        }
        parent->replaceChild(i, highlight);
        highlight->addChild(i);
        if (!itemSelectedObserver) {
            itemSelectedObserver = new SelectedObserver;
            itemSelectedObserver->container = this;
        }
        item->addObserver(itemSelectedObserver);
    }
    itemSelected = item;
}

void OsgContainer::setHoverItem(Item3d *item)
{
    if (itemHovered == item) {
        return;
    }
    ref_ptr<Group> highlight;
    ref_ptr<Item3d> i = item;
    ref_ptr<Item3d> prevHovered = itemHovered;
    Q_UNUSED(prevHovered);
    if (itemHovered) {
        if (itemHovered->getNumParents() > 0) {
            highlight = itemHovered->getParent(0);
            highlight->removeChildren(0, highlight->getNumChildren());
            if (highlight->getNumParents() > 0) {
                ref_ptr<Group> parent = highlight->getParent(0);
                parent->replaceChild(highlight, itemHovered);
            }
        }
        itemHovered->removeObserver(itemHoveredObserver);
    }
    if (item) {
        if (!highlight) {
            osgFX::Outline *outline = new osgFX::Outline;
            outline->setName("HoverProxy");
            outline->setColor(toVec4(Qt::blue));
            outline->setWidth(5);
            outline->setEnabled(true);
            highlight = outline;
        }
        ref_ptr<Group> parent = i->getParent(0);
        if (!parent) {
            return;
        }
        parent->replaceChild(i, highlight);
        highlight->addChild(i);
        if (!itemHoveredObserver) {
            itemHoveredObserver = new HoverObserver;
            itemHoveredObserver->container = this;
        }
        item->addObserver(itemHoveredObserver);
    }
    itemHovered = item;
}

void OsgContainer::paintGL()
{
    if (isVisibleTo(QApplication::activeWindow())) {
        frame();
    }
}

void OsgContainer::ContextMenu(const QVector3D &intersection, Item3d *item)
{
    QMenu menu;

    menu.addAction(tr("重置视图"), [this] () {
        home();
    });

    if (item) {
        itemSelected = item;
        QAction *actionZoomFit = new QAction(tr("放大到该零件"), this);
        connect(actionZoomFit, &QAction::triggered, this, &OsgContainer::ZoomFit); //该ZoomFit涉及到ItemSelected，因此需要在itemSelected更新后调用
        menu.addAction(actionZoomFit);                                             //添加ZoomFit动作，对PointMt来说，放大到其父节点

        item->ContextMenu(&menu, intersection);                                 //根据item的类型，自动添加右键菜单
    }
    menu.exec(QCursor::pos());
}

void OsgContainer::ZoomFit()
{
    if (!itemSelected) {
        return;
    }
    getCameraManipulator()->setNode(getSceneData());                            //重置视口节点

    getCameraManipulator()->setNode(itemSelected);                              //此三条命令可以将视口指向该节点
    getCameraManipulator()->computeHomePosition();
    home();

    getCameraManipulator()->setNode(getSceneData());                            //重置视口节点
    getCameraManipulator()->computeHomePosition();
}

void OsgContainer::init3D()
{
    root = new Group;
    root->setName("Root");

    osg::ref_ptr<Camera> camera = createCamera(0, 0, width(), height());
    setCamera(camera);
    ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
    setCameraManipulator(manipulator);
    addEventHandler(new osgViewer::StatsHandler);
    addEventHandler(new osgViewer::ThreadingHandler());
    addEventHandler(new osgViewer::HelpHandler);
    addEventHandler(new osgGA::StateSetManipulator(camera->getOrCreateStateSet()));
    setThreadingModel(osgViewer::Viewer::SingleThreaded);

    root->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::ON);
    root->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, StateAttribute::ON);
    setSceneData(root);

//    setRunFrameScheme(ON_DEMAND);

    startTimer(10);
}

ref_ptr<Camera> OsgContainer::createCamera(int x, int y, int w, int h)
{
    w = qMax(w, 100);
    h = qMax(w, 100);
    window = new osgViewer::GraphicsWindowEmbedded(x, y, w, h);
//    DisplaySettings* ds = DisplaySettings::instance().get();
    ref_ptr<GraphicsContext::Traits> traits = new GraphicsContext::Traits;
    traits->windowDecoration = true;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;

    ref_ptr<Camera> camera = new Camera;
    camera->setGraphicsContext(window);
    camera->setViewport(new Viewport(0, 0, traits->width, traits->height));
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->setProjectionMatrixAsPerspective(
                30.0f, double(traits->width) / double(traits->height), 1.0f, 10000.0f);
    camera->setAllowEventFocus(false);
    camera->setReferenceFrame(Transform::ABSOLUTE_RF);
    camera->setRenderOrder(Camera::POST_RENDER);
    camera->setClearColor(toVec4(QColor(224, 226, 237)));

    return camera.release();
}

void OsgContainer::createLight(const LightConfig &lightConfig)
{
    LightSource *lightSource = new LightSource;
    Light *light = lightSource->getLight();

    QColor c = lightConfig.color;
    light->setLightNum(0);
    light->setAmbient(Vec4(c.redF(), c.greenF(), c.blueF(), c.alphaF() * lightConfig.ambient));
    light->setDiffuse(Vec4(c.redF(), c.greenF(), c.blueF(), c.alphaF() * lightConfig.brightness));
    light->setSpecular(Vec4(c.redF(), c.greenF(), c.blueF(), c.alphaF() * lightConfig.specularity));

    qreal x = qCos(qDegreesToRadians(lightConfig.latitude)) * qSin(qDegreesToRadians(lightConfig.longitude));
    qreal y = -qCos(qDegreesToRadians(lightConfig.latitude)) * qCos(qDegreesToRadians(lightConfig.longitude));
    qreal z = qSin(qDegreesToRadians(lightConfig.longitude));
    light->setDirection(Vec3(x, y, z));

    lightSource->setLocalStateSetModes(lightConfig.enabled ? StateAttribute::ON : StateAttribute::OFF);
    lightSource->setReferenceFrame(lightConfig.isRelative ? LightSource::RELATIVE_RF : LightSource::ABSOLUTE_RF);
    getCamera()->addChild(lightSource);
}
