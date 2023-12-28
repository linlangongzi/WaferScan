#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ConfigObject/ConfigManager.h"

class UTSetup;
class PanelUTSetup;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef ConfigManager::pointer pointer;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void ChannelChanged(UTSetup *setup);

private slots:
    void on_actionStart_triggered();

    void on_actionStop_triggered();

private:
    void InitUI();
    void Connect();
    bool CreateTcpServer();

private:
    Ui::MainWindow *ui;
    PanelUTSetup *p_PanelUTSetup;
};

#endif // MAINWINDOW_H
