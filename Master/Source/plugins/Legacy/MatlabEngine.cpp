#include "MatlabEngine.h"
#include <Core/Platform.h>
#include <QLoggingCategory>
#ifdef Q_OS_WIN
#   include "qt_windows.h"
#endif

static Q_LOGGING_CATEGORY(debug, "matlab");
static const int OUTPUT_BUFFER_SIZE = 1024;

MatlabEngine::MatlabEngine(QObject *parent) :
    QObject(parent),
    eng("libeng")
//    mx("libmx")
{
    SetBasePath(platform()->Get("MatlabBasePath").toString());
}

MatlabEngine::~MatlabEngine()
{
    auto engClose = (EngCloseFunc) eng.resolve("engClose");
    if (engClose && engine) {
        int ret = engClose(engine);
        qDebug(debug) << "Closed engine" << engine << ":" << ret;
    }
}

bool MatlabEngine::Load()
{
    if (engine) {
        return engEvalString;
    }
#ifdef Q_OS_WIN
    if (!basePath.isEmpty()) {
        SetDllDirectoryA(qPrintable(basePath));
    }
#endif
    if (!eng.load()/* || !mx.load()*/) {
        qWarning(debug) << "Could not load library:" << eng.errorString();
        return false;
    }
    auto engOpen = (EngOpenFunc) eng.resolve("engOpen");
    if (!engOpen) {
        qWarning(debug) << "Could not resolve engOpen";
        return false;
    }
    engine = engOpen(nullptr);
    auto engSetVisible = (EngSetVisibleFunc) eng.resolve("engSetVisible");
    if (engSetVisible) {
        engSetVisible(engine, false);
    }
    auto engOutputBuffer = (EngOutputBufferFunc) eng.resolve("engOutputBuffer");
    if (engOutputBuffer) {
        buffer.resize(OUTPUT_BUFFER_SIZE);
        engOutputBuffer(engine, buffer.data(), OUTPUT_BUFFER_SIZE);
    }
    engEvalString = (EngEvalStringFunc) eng.resolve("engEvalString");
    return engEvalString;
}

QString MatlabEngine::DoString(const QString &command)
{
    if (!Load()) {
        return eng.errorString();
    }
    buffer.fill('\0');
    const QByteArray commandLocal = command.toLocal8Bit();
    if (engEvalString(engine, commandLocal.data()) != 0) {
        return QString();
    }
    return QString::fromLocal8Bit(buffer);
}
