#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CardWork/AECWork.h"
#include "Platform/Platform.h"
#include "TcpServer/DataDef.h"
#include "TcpServer/TcpServer.h"
#include "ConfigObject/User.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#if defined(Q_OS_WIN32) && !defined(Q_OS_WIN64)
    qCritical() << tr("Compile environment is NOT 64 bit.");
#endif

    platform(); /* 强制构造Platform单例，载入配置 */

#ifdef QT_DEBUG
    User::SwitchUser(platform()->Users()->GetByID<User>(1));
#endif

    ui->setupUi(this);
    CreateTcpServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::CreateTcpServer()
{
    TcpServer* serveAScan = new TcpServer(this, ASCAN_PORT);
    TcpServer* serveMonitor = new TcpServer(this, MONITOR_PORT);
    TcpServer* serveCScan = new TcpServer(this, CSCAN_PORT);
    TcpServer* serveParam = new TcpServer(this, PARAM_PORT);

    if(serveAScan && serveMonitor && serveCScan && serveParam) {
        qDebug() << "TcpServer create succeed";
        return true;
    }
    else {
        qCritical() << "TcpServer create failed, please restart!";
        return false;
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::on_probeBoxSimulation_clicked(bool checked)
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();
    for(AecWorkVector::iterator it = pAecWorkVec->begin(); it != pAecWorkVec->end(); ++it) {
        sharedAecWork aecWork = *it;
        if (aecWork.data()) {
            aecWork->SetProbeBoxSimulationData(checked);
        }
    }
}

void MainWindow::on_PLCSwitch_clicked(bool checked)
{
    sharedCardWork mfcWork = platform()->GetMfcWork();
    mfcWork->SetSimulationData(checked);
}
