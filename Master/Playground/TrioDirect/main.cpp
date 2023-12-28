#include <QApplication>
#include <QQmlApplicationEngine>
#include <qqml.h>
#include "arsMotion/Trio/Driver.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<arsMotion::Trio::Driver>("com.allrising.test", 1, 0, "MotionDriverTrio");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
