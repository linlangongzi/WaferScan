#pragma once

#include "DialogAbout.h"
#include "Id.h"
#include <utils/fancymainwindow.h>
#include <QMap>
#include <functional>

namespace Ui {
class MainWindow;
}

class FileOperateManager;
namespace Core {

class ICore;
class IContext;

class CORE_EXPORT MainWindow : public Utils::FancyMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    bool showOptionsDialog(Id page, QWidget *parent = nullptr);

    void insertMenu(QMenu *menu);
    IContext *contextObject(QWidget *widget);
    void addContextObject(IContext *context);
    void removeContextObject(IContext *context);
    IContext *currentContextObject() const;
    QDockWidget *AddPanelToDockWidget(QWidget *obj, Qt::DockWidgetArea area, const QString &name, const QString &enabledKey, bool isVisible = true, bool hasIcon = false);
    void addPreCloseListener(const std::function<bool ()> &listener);
    void initUi();

private slots:
    void aboutToShutdown();
    void updateFocusWidget(QWidget *old, QWidget *now);
    void updateContextObject(const QList<Core::IContext *> &context);
    void updateContext();
    void createDefaultMenu();

    void on_actionPlugins_triggered();
    void on_actionAbout_triggered();
    void on_actionBrowseConfig_triggered();
    void on_actionExit_triggered();
    void on_actionImportLayout_triggered();
    void on_actionExportLayout_triggered();
    void on_actionResetLayout_triggered();
    void on_actionOptions_triggered();

private:
    QJsonValue SaveWindowState() const;
    bool LoadWindowState(const QJsonValue &value);
    void closeEvent(QCloseEvent *event) override;

private:
    ICore *core;
    Ui::MainWindow *ui;
    DialogAbout *about;

    QList<IContext *> m_activeContext;
    QMap<QWidget *, IContext *> m_contextWidgets;
    QList<std::function<bool ()>> m_preCloseListeners;

    FileOperateManager *actionManager = nullptr;
};

}   // namespace Core
