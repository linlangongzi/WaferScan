#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SixJoints.h>
#include <SerialPort.h>


class QAction;

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

private:
    void createToolBar();
    void createActions();

private:
    Ui::MainWindow *ui;
    SixJoints *sixJointsCtrl;
    SerialPort *serialPort;

    QAction *serialPortAction;
    QToolBar *toolBar;
    QDockWidget *serialDock;
};

#endif // MAINWINDOW_H
