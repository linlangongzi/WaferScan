#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "SixJoints.h"
#include <QMainWindow>
#include <QTimer>
#include "SerialPort.h"
#include "cg27.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void setSerialPort();
    void setSteerPlatform();
    void setStepperPlatform();
    void SetPos();
private:
    void createActions();
    void createToolBar();

    Ui::MainWindow *ui;
//    SixJoints *sixJoints;
    SixJoints *sixJointsForSteerPlatform;
    SixJoints *sixJointsForStepperPlatform;
    SerialPort *serialPort;

    QDockWidget *serialDock;
    QDockWidget *steerPlatformDock;
    QDockWidget *stepperPlatformDock;

    QAction *serialPortAction;
    QAction *steerPlatformAction;
    QAction *stepperPlatformAction;

    QToolBar *toolBar;

    CG27 cg27;
    QTimer *timerForCG27;
};

#endif // MAINWINDOW_H
