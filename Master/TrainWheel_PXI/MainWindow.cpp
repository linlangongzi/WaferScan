#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Platform/Platform.h"
#include "ConfigObject/User.h"
#include "ConfigObject/UTSetup.h"
#include "PanelUTSetup/PanelUTSetup.h"
#include <QDebug>
#include <QDockWidget>
#include "TcpServer/TcpServer.h"
#include "TcpServer/DataDef.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qSetMessagePattern("[%{type}] (%{file}:%{line}) - %{message}"); /* 这个pattern仅在未加载日志面板时生效 */

#if defined(Q_OS_WIN32) && !defined(Q_OS_WIN64)
    qCritical() << tr("Compile environment is NOT 64 bit.");
#endif

    platform(); /* 强制构造Platform单例，载入配置 */

#ifdef QT_DEBUG
    User::SwitchUser(platform()->Users()->GetByID<User>(1));
#endif

    ui->setupUi(this);
    InitUI();
    Connect();

    CreateTcpServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ChannelChanged(UTSetup *setup)
{
    p_PanelUTSetup->UnbindUTSetup();
    p_PanelUTSetup->BindUTSetup(setup);
}

void MainWindow::InitUI()
{
    p_PanelUTSetup = new PanelUTSetup(this);
    p_PanelUTSetup->BindUTSetup(platform()->Setups()->GetByID<UTSetup>(10000));
    QDockWidget *pDockPanelUTSetup = new QDockWidget(this);
    pDockPanelUTSetup->setWindowTitle(tr("PanelUTSetup"));
    pDockPanelUTSetup->setAllowedAreas(Qt::TopDockWidgetArea);
    pDockPanelUTSetup->setWidget(p_PanelUTSetup);
    addDockWidget(Qt::TopDockWidgetArea, pDockPanelUTSetup);
}

void MainWindow::Connect()
{
    connect(p_PanelUTSetup, SIGNAL(SetupChanged(UTSetup*)), this, SLOT(ChannelChanged(UTSetup*)));
}

void MainWindow::on_actionStart_triggered()
{
    platform()->AecWorkStart();
}

void MainWindow::on_actionStop_triggered()
{
    platform()->AecWorkStop();
}

bool MainWindow::CreateTcpServer()
{
    TcpServer* serveParam = new TcpServer(this, PARAM_PORT);
    TcpServer* serveMonitor = new TcpServer(this, MONITOR_PORT);
    TcpServer* serveAScan = new TcpServer(this, ASCAN_PORT);
    TcpServer* serveCScan = new TcpServer(this, CSCAN_PORT);
    TcpServer* serveEyes = new TcpServer(this, EYES_STATUS_PORT);
    TcpServer* serveMfcEyes = new TcpServer(this, MFC_EYES_STATUS_PORT);
    TcpServer* serveLua = new TcpServer(this, LUA_PORT);

    if(serveMonitor && serveAScan
        && serveCScan && serveParam && serveEyes && serveMfcEyes && serveLua)
    {
        return true;
    }
    else
    {
        qCritical() << "TcpServer create failed, please restart!";
        return false;
    }
}
