QT += network

include(../plugin.pri)

SOURCES += Ars500.cpp \
    AScanSocket.cpp \
    CScanSocket.cpp \
    IncomingBaseSocket.cpp \
    MonitorSocket.cpp \
    ParameterSocket.cpp \
    Parser.cpp \
    Ars500Plugin.cpp

HEADERS += Ars500.h \
    AScanSocket.h \
    CScanSocket.h \
    DataDef.h \
    IncomingBaseSocket.h \
    MonitorSocket.h \
    ParameterSocket.h \
    Parser.h \
    Ars500Plugin.h
