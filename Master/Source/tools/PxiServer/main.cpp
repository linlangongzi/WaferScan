#include <QCoreApplication>
#include <PxiManager.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PxiManager pxi;
    pxi.rescan();

    return a.exec();
}
