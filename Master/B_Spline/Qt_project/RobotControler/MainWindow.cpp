#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_SixJoints.h"
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();                            //创建QAction
    createToolBar();                            //创建工具栏

    //将控制器设置为主窗口
    sixJointsCtrl = new SixJoints();
    setCentralWidget(sixJointsCtrl);
    sixJointsCtrl->setEnabled(1);

    //将串口设置窗口设置成一个dock
    serialPort = new SerialPort;
    serialDock = new QDockWidget(this);
    serialDock->setWidget(serialPort);
    serialDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    serialDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    serialDock->close();

    /*signal and slot*/
    //连接轴坐标与串口发送
    connect(sixJointsCtrl, SIGNAL(SendDataBySerial(const char*)), serialPort, SLOT(sendData(const char*)));
    //连接串口与控制器
//    connect(serialPort, SIGNAL(serialPortIsOpen(bool)), sixJointsCtrl, SLOT(setEnabled(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::setSerialPort     打开串口设置功能
 */
void MainWindow::setSerialPort()
{
    addDockWidget(Qt::LeftDockWidgetArea, serialDock);
    serialDock->show();
}


/*!
 * \brief MainWindow::createToolBar     设置工具栏
 */
void MainWindow::createToolBar()
{
    toolBar = addToolBar(tr("&Set"));
    toolBar->addAction(serialPortAction);
}
/*!
 * \brief MainWindow::createActions     创建QAction
 */
void MainWindow::createActions()
{
    serialPortAction = new QAction(tr("&SerialPort"), this);
    connect(serialPortAction, SIGNAL(triggered(bool)), this, SLOT(setSerialPort()));
}

