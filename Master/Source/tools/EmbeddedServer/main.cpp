#include "Server.h"
#include <QCoreApplication>
#include <QLoggingCategory>
#include <signal.h>

static Q_LOGGING_CATEGORY(debug, "server.main")

static void interruptHandler(int signal)
{
    Q_UNUSED(signal);
    QCoreApplication::quit();
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    signal(SIGTERM, &interruptHandler);
    signal(SIGINT, &interruptHandler);

    qSetMessagePattern("%{time process}[%{threadid}][%{type}]%{category}: %{message}");

    Server server;
    if (!server.start()) {
        qCDebug(debug) << "Failed starting server. Exiting.";
        return 1;
    }

    return a.exec();
}
