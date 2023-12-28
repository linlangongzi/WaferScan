#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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
    void on_command_returnPressed();
    void on_connect_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket socket;
};

#endif // MAINWINDOW_H
