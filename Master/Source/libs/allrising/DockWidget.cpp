#include "DockWidget.h"
#include <QAction>
#include <QBoxLayout>
#include <QJsonObject>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyleOptionGraphicsItem>
#include <QStylePainter>
#include <QtMath>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "allrising.dock");

DockWidgetTitleButton::DockWidgetTitleButton(QWidget *parent)
    : QAbstractButton(parent)
{
    setFocusPolicy(Qt::NoFocus);
}

QSize DockWidgetTitleButton::sizeHint() const
{
    ensurePolished();

    int size = 2 * style()->pixelMetric(QStyle::PM_DockWidgetTitleBarButtonMargin, nullptr, this);
    if (!icon().isNull()) {
        int iconSize = style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr, this);
        QSize sz = icon().actualSize(QSize(iconSize, iconSize));
        size += qMax(sz.width(), sz.height());
    }

    return {size, size};
}

QSize DockWidgetTitleButton::minimumSizeHint() const
{
    return sizeHint();
}

void DockWidgetTitleButton::enterEvent(QEvent *event)
{
    if (isEnabled()) {
        update();
    }
    QAbstractButton::enterEvent(event);
}

void DockWidgetTitleButton::leaveEvent(QEvent *event)
{
    if (isEnabled()) {
        update();
    }
    QAbstractButton::leaveEvent(event);
}

void DockWidgetTitleButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QStyleOptionToolButton opt;
    opt.init(this);
    opt.state |= QStyle::State_AutoRaise;

    if (style()->styleHint(QStyle::SH_DockWidget_ButtonsHaveFrame, nullptr, this)) {
        if (isEnabled() && underMouse() && !isChecked() && !isDown()) {
            opt.state |= QStyle::State_Raised;
        }
        if (isChecked()) {
            opt.state |= QStyle::State_On;
        }
        if (isDown()) {
            opt.state |= QStyle::State_Sunken;
        }
        style()->drawPrimitive(QStyle::PE_PanelButtonTool, &opt, &p, this);
    }

    opt.icon = icon();
    opt.subControls = nullptr;
    opt.activeSubControls = nullptr;
    opt.features = QStyleOptionToolButton::None;
    opt.arrowType = Qt::NoArrow;
    int size = style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr, this);
    opt.iconSize = QSize(size, size);
    style()->drawComplexControl(QStyle::CC_ToolButton, &opt, &p, this);
}

DockWidgetTitle::DockWidgetTitle(QWidget *parent) :
    QWidget(parent)
{
    auto close = new DockWidgetTitleButton(this);
    close->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    connect(close, SIGNAL(clicked(bool)), parent, SLOT(close()));

    auto floating = new DockWidgetTitleButton(this);
    connect(floating, SIGNAL(clicked(bool)), this, SLOT(ToggleFloating()));
    floating->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton));

    auto *l = new QBoxLayout(QBoxLayout::LeftToRight, this);

    l->addStretch();
    l->setMargin(2);
    l->setSpacing(0);
    setLayout(l);

    AddButton(close);
    AddButton(floating);

    isVertical = false;
}

void DockWidgetTitle::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    auto *dock = qobject_cast<QDockWidget *>(parentWidget());
    // ### Add PixelMetric to change spacers, so style may show border
    // when not floating.
//    if (dock->isFloating()) {
//        QStyleOptionFrame framOpt;
//        framOpt.init(this);
//        p.drawPrimitive(QStyle::PE_FrameDockWidget, framOpt);
//    }

    // Title must be painted after the frame, since the areas overlap, and
    // the title may wish to extend out to all sides (eg. XP style)
    QStyleOptionDockWidget opt;
    opt.initFrom(this);
    opt.closable = dock->features() & QDockWidget::DockWidgetClosable;
    opt.title = dock->windowTitle();
    opt.floatable = dock->features() & QDockWidget::DockWidgetFloatable;
    opt.movable = dock->features() & QDockWidget::DockWidgetMovable;
    opt.verticalTitleBar = isVertical;

    int darkness = qRound(100 + 200 * qAbs(qSin(highlight * 2 * M_PI)));
    QColor color = opt.palette.background().color().darker(darkness);
    opt.palette.setColor(QPalette::Background, color);

    p.drawControl(QStyle::CE_DockWidgetTitle, opt);
}

void DockWidgetTitle::resizeEvent(QResizeEvent *e)
{
    auto *dock = qobject_cast<QDockWidget *>(parentWidget());
    if (dock) {
        bool vertical = dock->features() & QDockWidget::DockWidgetVerticalTitleBar;
        SetOrientation(vertical);
    }
    QWidget::resizeEvent(e);
}

QSize DockWidgetTitle::sizeHint() const
{
    int maxSize = 0;
    for (auto button: findChildren<QWidget *>()) {
        QSize size = button->sizeHint();
        maxSize = qMax(maxSize, size.height());
        maxSize = qMax(maxSize, size.width());
    }
    return {maxSize + 2, maxSize + 2};
}

QSize DockWidgetTitle::minimumSizeHint() const
{
    return sizeHint();
}

void DockWidgetTitle::AddButton(QAbstractButton *button)
{
    button->setParent(this);
    auto *l = qobject_cast<QBoxLayout *>(layout());
    if (l) {
        l->insertWidget(1, button);
    }
}

void DockWidgetTitle::SetOrientation(bool vertical)
{
    if (vertical == isVertical) {
        return;
    }
    isVertical = vertical;
    auto *l = qobject_cast<QBoxLayout *>(layout());
    if (l) {
        l->setDirection(vertical ? QBoxLayout::BottomToTop : QBoxLayout::LeftToRight);
    }
}

void DockWidgetTitle::ToggleFloating()
{
    auto *dock = qobject_cast<QDockWidget *>(parentWidget());
    if (dock) {
        dock->setFloating(!dock->isFloating());
    }
}

void DockWidgetTitle::ToggleOrientation()
{
    auto *dock = qobject_cast<QDockWidget *>(parentWidget());
    if (dock) {
        dock->setFeatures(dock->features() ^ QDockWidget::DockWidgetVerticalTitleBar);
    }
}

void DockWidgetTitle::SetHighlightProgress(qreal progress)
{
    highlight = progress;
    update();
}

DockWidget::DockWidget(QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(parent, flags)
{
    actionVerticalTitle = new QAction(tr("垂直标题栏"), this);
    connect(actionVerticalTitle, &QAction::toggled, this, &DockWidget::SetTitleBarVertical);
    actionVerticalTitle->setCheckable(true);

    actionAlwaysFloating = new QAction(tr("禁止停靠"), this);
    connect(actionAlwaysFloating, &QAction::toggled, this, &DockWidget::SetAlwaysFloating);
    actionAlwaysFloating->setCheckable(true);

    actionFullscreen = new QAction(tr("全屏"), this);
    connect(actionFullscreen, &QAction::triggered, this, &DockWidget::fullscreen);

    title = new DockWidgetTitle(this);
    setTitleBarWidget(title);

    menuButton = new DockWidgetTitleButton(this);
    connect(menuButton, &QAbstractButton::clicked, this, &DockWidget::OpenMenu);
    menuButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton));
    title->AddButton(menuButton);

    auto updateTitle = [this](bool topLevel) {
        if (topLevel) {
            setTitleBarWidget(nullptr);
        } else {
            setTitleBarWidget(title);
            setWindowState(windowState() & ~Qt::WindowMaximized);
        }
    };
    updateTitle(isTopLevel());
    connect(this, &QDockWidget::topLevelChanged, updateTitle);
}

QJsonObject DockWidget::SaveLayout() const
{
    QJsonObject obj;
    obj["DockVertical"] = actionVerticalTitle->isChecked();
    obj["DockAlwaysFloating"] = actionAlwaysFloating->isChecked();
    return obj;
}

void DockWidget::LoadLayout(const QJsonObject &layout)
{
    actionVerticalTitle->setChecked(layout["DockVertical"].toBool());
    actionAlwaysFloating->setChecked(layout["DockAlwaysFloating"].toBool());
}

/*!
 * \brief DockWidget::Flash 悬浮窗口闪烁
 * \param count 闪烁次数
 *              -1 不停闪烁
 *              >=0 闪烁次数
 */
void DockWidget::Flash(int count)
{
    show();
    raise();
    QPropertyAnimation *animation = new QPropertyAnimation(titleBarWidget(), "backColorAlpha");
    animation->setDuration(500);
    animation->setLoopCount(count);
    animation->setKeyValueAt(0, 1);
    animation->setEndValue(0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void DockWidget::OpenMenu()
{
    QMenu menu;
    menu.addAction(actionVerticalTitle);
    menu.addAction(actionAlwaysFloating);
    menu.addAction(actionFullscreen);
//    menu.addSeparator();
//    menu.addAction(tr("停靠到左侧"), this, SLOT(DockLeft()));
//    menu.addAction(tr("停靠到右侧"), this, SLOT(DockRight()));
//    menu.addAction(tr("停靠到顶部"), this, SLOT(DockTop()));
//    menu.addAction(tr("停靠到底部"), this, SLOT(DockBottom()));

    menu.exec(mapToGlobal(menuButton->geometry().bottomLeft()));
}

void DockWidget::SetTitleBarVertical(bool isVertical)
{
    if (isVertical) {
        setFeatures(features() | DockWidgetVerticalTitleBar);
    } else {
        setFeatures(features() & (~DockWidgetVerticalTitleBar));
    }
}

void DockWidget::SetAlwaysFloating(bool enabled)
{
    if (enabled) {
        setAllowedAreas(Qt::NoDockWidgetArea);
        setFloating(true);
    } else {
        setAllowedAreas(Qt::AllDockWidgetAreas);
    }
}

void DockWidget::fullscreen()
{
    setFloating(true);
    setWindowState(windowState() | Qt::WindowMaximized);
}

void DockWidget::DockAt(Qt::DockWidgetArea area)
{
    auto *mw = qobject_cast<QMainWindow *>(parent());
    if (mw) {
        mw->removeDockWidget(this);
        mw->addDockWidget(area, this);
        show();
    }
}

void DockWidget::DockLeft()
{
    DockAt(Qt::LeftDockWidgetArea);
}

void DockWidget::DockRight()
{
    DockAt(Qt::RightDockWidgetArea);
}

void DockWidget::DockTop()
{
    DockAt(Qt::TopDockWidgetArea);
}

void DockWidget::DockBottom()
{
    DockAt(Qt::BottomDockWidgetArea);
}

void DockWidget::leaveEvent(QEvent *event)
{
    if (parentWidget() && parentWidget()->property("AutoHideDockTitle").toBool()) {
        titleBarWidget()->hide();
    }
    QDockWidget::leaveEvent(event);
}
