#include "MainWindow.h"
#include "SixJoints.h"
#include "SerialPort.h"

#include <QApplication>
#include <QDebug.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();
    ForwardSolver solver;
    qreal j1 = 30;
    qreal j2 = 0;
    qreal j3 = 0;
    qreal j4 = 0;
    qreal j5 = 0;
    qreal j6 = 0;
    QVector<qreal> joints;
    joints = QVector<qreal>() << j1 << j2 << j3 << j4 << j5 << j6;
    qDebug() << "joints angle are : " << joints;
    qDebug() << "TCP pos are: " << solver.getTCP(joints);
    return a.exec();
}
