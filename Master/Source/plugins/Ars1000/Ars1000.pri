QT += network

SOURCES += \
    $$PWD/Ars1000Plugin.cpp \
    $$PWD/PipeParser.cpp \
    $$PWD/PipeAScanSocket.cpp \
    $$PWD/PipeCScanSocket.cpp \
    $$PWD/PipeMonitorSocket.cpp \
    $$PWD/PipePrameterSocket.cpp \
    $$PWD/PipeInstrumentDriver.cpp \
    $$PWD/PipeIncomingBaseSocket.cpp

HEADERS += \
    $$PWD/Ars1000Plugin.h \
    $$PWD/PipeParser.h \
    $$PWD/PipeDataDef.h \
    $$PWD/PipeAScanSocket.h \
    $$PWD/PipeCScanSocket.h \
    $$PWD/PipeMonitorSocket.h \
    $$PWD/PipePrameterSocket.h \
    $$PWD/PipeInstrumentDriver.h \
    $$PWD/PipeIncomingBaseSocket.h

DISTFILES += \
    $$PWD/Ars1000.json.in
