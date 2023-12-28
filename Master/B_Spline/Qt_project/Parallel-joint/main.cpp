#include "joint_control.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    joint_control w;
    w.show();

    return a.exec();
}
