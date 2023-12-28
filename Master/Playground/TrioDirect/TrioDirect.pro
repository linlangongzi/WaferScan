TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    ../../Source/arsCore/UTPose.cpp \
    ../../Source/arsCore/Util.cpp

HEADERS += ../../Source/arsCore/UTPose.h \
    ../../Source/arsCore/Util.h

INCLUDEPATH += ../../Source ../../ARS3D

RESOURCES += qml.qrc \

contains(QMAKE_COMPILER, msvc): {
    PRECOMPILED_HEADER += ../../Source/arsCore/ExecEncodingUTF8.hpp
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
#include(../../Source/arsCore/arsCore.pri)
include(../../Source/arsMotion/arsMotion.pri)

DEFINES += ARS_DEBUG_TRIO
