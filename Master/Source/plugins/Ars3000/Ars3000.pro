QT += network

include(../plugin.pri)

HEADERS += \
    Ars3000Plugin.h \
    Ars3000.h

SOURCES += \
    Ars3000Plugin.cpp \
    Ars3000.cpp

RESOURCES += \
    LuaScripts.qrc
