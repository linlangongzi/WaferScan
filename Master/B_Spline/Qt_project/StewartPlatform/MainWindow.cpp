#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SixJoints.h"
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createToolBar();

    //将串口设置窗口设置成一个dock
    serialPort = new SerialPort;
    serialDock = new QDockWidget(this);
    serialDock->setWidget(serialPort);
    serialDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    serialDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    serialDock->close();
    setCentralWidget(serialDock);

    sixJointsForStepperPlatform = new SixJoints(NULL, Platform::ClassicType);
    stepperPlatformDock = new QDockWidget(this);
    stepperPlatformDock->setWindowTitle(QString("Classical Structure"));
    stepperPlatformDock->setWidget(sixJointsForStepperPlatform);
    stepperPlatformDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    stepperPlatformDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    stepperPlatformDock->close();

    sixJointsForSteerPlatform = new SixJoints(NULL, Platform::LinkType);
    steerPlatformDock = new QDockWidget(this);
    steerPlatformDock->setWindowTitle(QString("Link Structure"));
    steerPlatformDock->setWidget(sixJointsForSteerPlatform);
    steerPlatformDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    steerPlatformDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    steerPlatformDock->close();

    //串口连接
    connect(sixJointsForStepperPlatform, SIGNAL(SendDataBySerial(QVector<qreal>)), serialPort, SLOT(sendData(QVector<qreal>)));
    connect(sixJointsForSteerPlatform, SIGNAL(SendDataBySerial(QVector<qreal>)), serialPort, SLOT(sendData(QVector<qreal>)));

    //发送udp报文
    connect(sixJointsForStepperPlatform, SIGNAL(SendMessage(QVector<qreal>)), serialPort, SLOT(sendMessage(QVector<qreal>)));
    connect(sixJointsForSteerPlatform, SIGNAL(SendMessage(QVector<qreal>)), serialPort, SLOT(sendMessage(QVector<qreal>)));

    cg27.start();
    timerForCG27 = new QTimer(this);
    timerForCG27->start(50);
    connect(timerForCG27, SIGNAL(timeout()), this, SLOT(SetPos()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::createToolBar     设置工具栏
 */
void MainWindow::createToolBar()
{
    toolBar = addToolBar(tr("&Set"));
    toolBar->addAction(serialPortAction);
    toolBar->addAction(steerPlatformAction);
    toolBar->addAction(stepperPlatformAction);
}
/*!
 * \brief MainWindow::createActions     创建QAction
 */
void MainWindow::createActions()
{
    serialPortAction = new QAction(tr("SerialPort"), this);
    connect(serialPortAction, SIGNAL(triggered(bool)), this, SLOT(setSerialPort()));
    steerPlatformAction = new QAction(tr("Link"), this);
    connect(steerPlatformAction, SIGNAL(triggered(bool)), this, SLOT(setSteerPlatform()));
    stepperPlatformAction = new QAction(tr("Classical"), this);
    connect(stepperPlatformAction, SIGNAL(triggered(bool)), this, SLOT(setStepperPlatform()));
}
/*!
 * \brief MainWindow::setSerialPort     打开串口设置功能
 */
void MainWindow::setSerialPort()
{
    addDockWidget(Qt::LeftDockWidgetArea, serialDock);
    serialDock->show();
}

void MainWindow::setSteerPlatform()
{
    addDockWidget(Qt::RightDockWidgetArea, steerPlatformDock);
    stepperPlatformDock->hide();
    steerPlatformDock->show();
    sixJointsForSteerPlatform->setEnabled(true);
    sixJointsForStepperPlatform->setEnabled(false);
}

void MainWindow::setStepperPlatform()
{
    addDockWidget(Qt::RightDockWidgetArea, stepperPlatformDock);
    steerPlatformDock->hide();
    stepperPlatformDock->show();
    sixJointsForStepperPlatform->setEnabled(true);
    sixJointsForSteerPlatform->setEnabled(false);
}

/*!
 * \brief MainWindow::SetPos    响应定时器，用来抓取CG27的数据
 */
void MainWindow::SetPos()
{
    QVector<qreal> dataFromCg27 = cg27.ReadData();
    dataFromCg27[0] *= 1.0 / 4500.0;        //方向盘
    dataFromCg27[1] /= 1000.0;                //油门
    dataFromCg27[2] /= 1000.0;                //刹车
    QVector<qreal> pose(6);

    //如果连杆结构界面使能了CG27并且显示出来，刚进行处理
    if (sixJointsForSteerPlatform->getCg27Status() && !sixJointsForSteerPlatform->isHidden()) {
        pose = sixJointsForSteerPlatform->GetPose();
        pose[3] = (dataFromCg27[2] - dataFromCg27[1]) * 10;
        pose[4] = -dataFromCg27[0] * 20;
        pose[5] = -dataFromCg27[0] * 20;
        sixJointsForSteerPlatform->setSpinBoxPos(pose);
        qDebug() << "Pose: " << pose;
    }
    //如果电动缸结构界面使能了CG27并且显示出来，刚进行处理
    if (sixJointsForStepperPlatform->getCg27Status() && !sixJointsForStepperPlatform->isHidden()) {
        pose = sixJointsForStepperPlatform->GetPose();
        pose[3] = (dataFromCg27[2] - dataFromCg27[1]) * 10;
        pose[4] = -dataFromCg27[0] * 10;
        pose[5] = -dataFromCg27[0] * 10;
        sixJointsForStepperPlatform->setSpinBoxPos(pose);
    }
}

