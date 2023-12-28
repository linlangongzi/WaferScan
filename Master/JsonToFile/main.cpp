#include "UTSetupToJson.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UTSetupToJson w;
    w.show();

    return a.exec();
}
