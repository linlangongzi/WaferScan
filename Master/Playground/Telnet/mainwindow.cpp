#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../Source/arsCore/Util.h"

enum {
    WillEcho = 251,
    DoEcho = 253,
};
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&socket, &QAbstractSocket::stateChanged, [this](QAbstractSocket::SocketState state) {
        ui->history->appendPlainText(EnumToKey(state));
//        if (state == QAbstractSocket::ConnectedState) {
//            write("")
//        }
    });
    connect(&socket, &QIODevice::readyRead, [this]() {
        const QByteArray d = socket.readAll();
        if (d.contains("\xFF\xFB\x01")) {
            qDebug() << "Replying will";
            socket.write("\xFF\xFD\x01");
            socket.flush();
        }
        if (d.contains("\xFF\xFB\x03")) {
            qDebug() << "Replying gh";
            socket.write("\xFF\xFD\x03");
        }
        ui->history->appendPlainText(QString::fromLocal8Bit(d));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_command_returnPressed()
{
    socket.write(ui->command->text().toLocal8Bit() + "\r\n");
    socket.flush();
    ui->command->clear();
}

void MainWindow::on_connect_clicked()
{
    socket.connectToHost("192.168.1.80", 23);
}
