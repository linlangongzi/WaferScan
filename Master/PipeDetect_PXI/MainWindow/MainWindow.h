#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEvent>
#include <QMainWindow>
#include <QTimerEvent>

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
    void on_probeBoxSimulation_clicked(bool checked);

    void on_PLCSwitch_clicked(bool checked);

private:
    Ui::MainWindow *ui;

private:
    bool CreateTcpServer();
    void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
