#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CoreConstants.h"
#include "DocumentManager.h"
#include "ICore.h"
#include "IDocument.h"
#include "Id.h"
#include "LicenseManager.h"
#include "Platform.h"
#include "PluginDialog.h"
#include "SettingsDialog.h"
#include "actionmanager/actioncontainer.h"
#include "actionmanager/actionmanager.h"
#include <Core/FileOperateManager.h>
#include <Imaging/PanelCScan.h>
#include <Imaging/PanelSlice.h>
#include <Motion/MotionManager.h>
#include <QApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QUrl>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/DialogMaterial.h>
#include <Waveform/PanelAScan.h>
#include <allrising/DockWidget.h>
#include <allrising/ThrottleUpdate.h>
#include <extensionsystem/pluginmanager.h>
#include <utils/hostosinfo.h>

static Q_LOGGING_CATEGORY(debug, "core.mw");

using namespace Core;
using namespace Core::Internal;
using namespace ExtensionSystem;
using Utils::HostOsInfo;

MainWindow::MainWindow(QWidget *parent) :
    Utils::FancyMainWindow(parent),
    core(new ICore(this)),
    ui(new Ui::MainWindow),
    about(new DialogAbout(this))
{
    ui->setupUi(this);

    setDockNestingEnabled(true);
    createDefaultMenu();
    ActionManager::setContext(Context(Constants::C_GLOBAL));

    showCentralWidgetAction()->setVisible(false);
    centralWidget()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::showOptionsDialog(Id page, QWidget *parent)
{
//    emit core->optionsDialogRequested();
    return executeSettingsDialog(parent, page);
}

void MainWindow::insertMenu(QMenu *menu)
{
    ui->menubar->insertMenu(ui->menuHelp->menuAction(), menu);
}

IContext *MainWindow::contextObject(QWidget *widget)
{
    return m_contextWidgets.value(widget);
}

void MainWindow::addContextObject(IContext *context)
{
    if (!context) {
        return;
    }
    QWidget *widget = context->widget();
    if (m_contextWidgets.contains(widget)) {
        return;
    }

    m_contextWidgets.insert(widget, context);
}

void MainWindow::removeContextObject(IContext *context)
{
    if (!context) {
        return;
    }

    QWidget *widget = context->widget();
    if (!m_contextWidgets.contains(widget)) {
        return;
    }

    m_contextWidgets.remove(widget);
    if (m_activeContext.removeAll(context) > 0) {
        updateContextObject(m_activeContext);
    }
}

void MainWindow::updateFocusWidget(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)

    // Prevent changing the context object just because the menu or a menu item is activated
    if (qobject_cast<QMenuBar*>(now) || qobject_cast<QMenu*>(now))
        return;

    QList<IContext *> newContext;
    if (QWidget *p = qApp->focusWidget()) {
        IContext *context = nullptr;
        while (p) {
            context = m_contextWidgets.value(p);
            if (context)
                newContext.append(context);
            p = p->parentWidget();
        }
    }

    // ignore toplevels that define no context, like popups without parent
    if (!newContext.isEmpty() || qApp->focusWidget() == focusWidget()) {
        updateContextObject(newContext);
    }
}

void MainWindow::updateContextObject(const QList<IContext *> &context)
{
//    emit core->contextAboutToChange(context);
    m_activeContext = context;
    updateContext();
    qCDebug(debug) << "new context objects =" << context;
    foreach (IContext *c, context) {
        qCDebug(debug) << (c ? c->widget() : nullptr) << (c ? c->widget()->metaObject()->className() : nullptr);
    }
}

void MainWindow::aboutToShutdown()
{
    disconnect(qApp, &QApplication::focusChanged, this, &MainWindow::updateFocusWidget);
    m_activeContext.clear();
    hide();
}

void MainWindow::updateContext()
{
    Context contexts;

    foreach (IContext *context, m_activeContext) {
        contexts.add(context->context());
    }
    contexts.add(Context(Constants::C_GLOBAL));

    Context uniqueContexts;
    for (auto context : contexts) {
        const Id id = context;
        if (!uniqueContexts.contains(id)) {
            uniqueContexts.add(id);
        }
    }

    ActionManager::setContext(uniqueContexts);
    //    emit core->contextChanged(uniquecontexts);
}

void MainWindow::createDefaultMenu()
{
    using namespace Constants;

    ActionContainer *menubar = ActionManager::createMenuBar(MENU_BAR);
#ifndef Q_OS_MAC
    setMenuBar(menubar->menuBar());
#endif

    menubar->appendGroup(G_FILE);
//    menubar->appendGroup(G_EDIT);
    menubar->appendGroup(G_VIEW);
//    menubar->appendGroup(G_SCAN);
//    menubar->appendGroup(G_WINDOW);
    menubar->appendGroup(G_HELP);
    // === File Menu ===
    ActionContainer *fileMenu = ActionManager::createMenu(M_FILE);
    menubar->addMenu(fileMenu, G_FILE);
    fileMenu->menu()->setTitle(tr("文件"));
    actionManager = new FileOperateManager(this);
    Command *cmd = nullptr;
    QIcon icon;

    // Option Group
    fileMenu->appendGroup(G_FILE_OPTION);
    ui->actionOptions->setEnabled(LicenseManager::instance()->hasProductLicense());
    cmd = ActionManager::registerAction(ui->actionOptions, OPTIONS);
    fileMenu->addAction(cmd, G_FILE_OPTION);

//    ui->actionConnections->setEnabled(LicenseManager::instance()->hasProductLicense());
//    cmd = ActionManager::registerAction(ui->actionConnections, CONNECTIONS);
//    fileMenu->addAction(cmd, G_FILE_OPTION);
//    fileMenu->addSeparator(G_FILE_OPTION);

    // Exit Action

    fileMenu->appendGroup(G_FILE_OTHER);
    icon = QIcon::fromTheme(QLatin1String("application-exit"));
    ui->actionExit->setMenuRole(QAction::QuitRole);

    cmd = ActionManager::registerAction(ui->actionExit, EXIT);
    cmd->setDefaultKeySequence(QKeySequence(tr("Alt+F4")));
    fileMenu->addAction(cmd, G_FILE_OTHER);

    // === View Menu ===
    ActionContainer *viewMenu = ActionManager::createMenu(M_VIEW);
    menubar->addMenu(viewMenu, G_VIEW);
    viewMenu->menu()->setTitle(tr("视图"));

    viewMenu->appendGroup(G_VIEW_PANEL);

    viewMenu->appendGroup(G_VIEW_LAYOUT);
    viewMenu->addSeparator(G_VIEW_LAYOUT);

    cmd = ActionManager::registerAction(ui->actionImportLayout, IMPORT_LAYOUT);
    viewMenu->addAction(cmd, G_VIEW_LAYOUT);

    cmd = ActionManager::registerAction(ui->actionExportLayout, EXPORT_LAYOUT);
    viewMenu->addAction(cmd, G_VIEW_LAYOUT);

    cmd = ActionManager::registerAction(ui->actionResetLayout, RESET_LAYOUT);
    viewMenu->addAction(cmd, G_VIEW_LAYOUT);

    // === Help Menu ===
    ActionContainer *helpMenu = ActionManager::createMenu(M_HELP);
    menubar->addMenu(helpMenu, G_HELP);
    helpMenu->menu()->setTitle(tr("帮助"));

    helpMenu->appendGroup(G_HELP_SUPPORT);
    helpMenu->appendGroup(G_HELP_ABOUT);
    helpMenu->addSeparator(G_HELP_SUPPORT);

    if (!HostOsInfo::isMacHost()) {
        helpMenu->addSeparator(G_HELP_ABOUT);
    }

    cmd = ActionManager::registerAction(ui->actionBrowseConfig, BROWSE_CONFIG);
    helpMenu->addAction(cmd, G_HELP_SUPPORT);

    // About Action
    icon = QIcon::fromTheme(QLatin1String("help-about"));
    ui->actionAbout->setMenuRole(QAction::AboutRole);
    cmd = ActionManager::registerAction(ui->actionAbout, ABOUT_WAFERSCAN);
    helpMenu->addAction(cmd, G_HELP_ABOUT);

    //About Plugins Action
//    ui->actionPlugins->setMenuRole(QAction::ApplicationSpecificRole);
//    cmd = ActionManager::registerAction(ui->actionPlugins, ABOUT_PLUGINS);
//    helpMenu->addAction(cmd, G_HELP_ABOUT);
}

void MainWindow::on_actionPlugins_triggered()
{
    Core::Internal::PluginDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    about->exec();
}

void MainWindow::on_actionBrowseConfig_triggered()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(ICore::settingsPath()));
}

void MainWindow::on_actionExit_triggered()
{
    ThrottleUpdate::call(this, &MainWindow::close);
}

void MainWindow::initUi()
{
    ActionContainer *viewMenu = ActionManager::actionContainer(Constants::M_VIEW);

    viewMenu->addSeparator(Constants::G_VIEW_PANEL);

#ifdef ARS_LIB_OSG
    AddPanelToDockWidget(new PanelMachine(this), Qt::RightDockWidgetArea, tr("OSG View"), "OSGView");
#endif
    if (QApplication::arguments().contains("-viewonly")) {
        LoadWindowState(platform()->Get("ViewOnly"));
    } else {
        LoadWindowState(platform()->Get("WindowsState"));
    }

    for (QDockWidget *dock: findChildren<QDockWidget *>(QString(), Qt::FindDirectChildrenOnly)) {
        if (platform()->Get(dock->objectName()).isBool()) {
            dock->setVisible(platform()->Get(dock->objectName()).toBool());
        }
    }
}

/*! 添加一个面板到一个dockwidget当中 */
QDockWidget *MainWindow::AddPanelToDockWidget(QWidget *obj, Qt::DockWidgetArea area, const QString &name, const QString &enabledKey, bool isVisible, bool hasIcon)
{
    PluginManager::splashMessage(tr("正在初始化 %1 ...").arg(name));
    obj->setWindowTitle(name);
    obj->setObjectName(enabledKey);
    auto *dock = qobject_cast<QDockWidget *>(obj);
    if (dock == nullptr) {
        dock = new DockWidget(this);
        dock->setWidget(obj);
    }
    dock->setObjectName(enabledKey);
    dock->setWindowTitle(name);
    addDockWidget(area, dock);
    QAction *action = dock->toggleViewAction();
    if (hasIcon) {
        action->setIcon(obj->windowIcon());
    }

    ActionContainer *viewMenu = ActionManager::actionContainer(Constants::M_VIEW);
    Command *cmd = ActionManager::registerAction(action, Id::fromString("WaferScan." + enabledKey));
    viewMenu->addAction(cmd, Constants::G_VIEW_PANEL);

    connect(action, &QAction::triggered, this, QOverload<>::of(&QWidget::update));

    dock->setVisible(isVisible);
    return dock;
}

void MainWindow::addPreCloseListener(const std::function<bool ()> &listener)
{
    m_preCloseListeners << listener;
}

QJsonValue MainWindow::SaveWindowState() const
{
    QJsonObject obj;
    obj["State"] = QString::fromUtf8(saveState().toBase64());
    obj["Geometry"] = QString::fromUtf8(saveGeometry().toBase64());

    QJsonArray cscan;
    for (PanelCScan *panel: findChildren<PanelCScan *>()) {
        cscan << panel->saveSettings();
    }

    QJsonArray ascan;
    for (PanelAScan *panel: findChildren<PanelAScan *>()) {
        ascan << panel->saveSettings();
    }

    obj["PanelCScan"] = cscan;
    obj["PanelAScan"] = ascan;

    return obj;
}

bool MainWindow::LoadWindowState(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    QByteArray state = QByteArray::fromBase64(obj["State"].toString().toUtf8());
    QByteArray geometry = QByteArray::fromBase64(obj["Geometry"].toString().toUtf8());
    bool ok = restoreGeometry(geometry) && restoreState(state);
    qApp->processEvents();
    QJsonArray cscan = obj["PanelCScan"].toArray();
    for (PanelCScan *panel: findChildren<PanelCScan *>()) {
        panel->loadSettings(cscan.takeAt(0));
    }

    QJsonArray ascan = obj["PanelAScan"].toArray();
    for (PanelAScan *panel: findChildren<PanelAScan *>()) {
        panel->loadSettings(ascan.takeAt(0).toObject());
    }

    return ok;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for (const auto &listener: m_preCloseListeners) {
        if (!listener()) {
            event->ignore();
            return;
        }
    }
    if (FileOperateManager::closeFile()) {
        event->ignore();
        return;
    }
#ifndef QT_DEBUG                                                                /* 为了方便调试，退出对话框在调试模式中禁用 */
    if (QMessageBox::question(this, QString(), tr("确定要退出吗"), QMessageBox::Yes | QMessageBox::No ) != QMessageBox::Yes) {
        return event->ignore();
    }
#endif
    if (QApplication::arguments().contains("-viewonly")) {
        platform()->Set("ViewOnly", SaveWindowState());
    } else {
        platform()->Set("WindowsState", SaveWindowState());
    }

    for (DockWidget *dock: findChildren<DockWidget *>(QString(), Qt::FindDirectChildrenOnly)) {
        platform()->Set(dock->objectName(), dock->isVisible());
    }

    QMainWindow::closeEvent(event);
}

void Core::MainWindow::on_actionImportLayout_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("载入界面布局"), QString(), tr("界面布局文件 (*.layout)") );
    if (!fileName.isEmpty()) {
        QJsonValue v = ReadFileAsJson(fileName);
        if (!LoadWindowState(v)) {
            QMessageBox::warning(this, QString(), tr("载入界面布局失败"));
        }
    }
}

void Core::MainWindow::on_actionExportLayout_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存界面布局"), QString(), tr("界面布局文件 (*.layout)") );
    if (!fileName.isEmpty()) {
        if (!WriteFileAsJson(fileName, SaveWindowState())) {
            QMessageBox::warning(this, QString(), tr("保存界面布局失败：无法写入文件"));
        }
    }
}

void Core::MainWindow::on_actionResetLayout_triggered()
{
    LoadWindowState(platform()->Get("DefaultWindowsState"));
}

void Core::MainWindow::on_actionOptions_triggered()
{
    showOptionsDialog({}, this);
}
