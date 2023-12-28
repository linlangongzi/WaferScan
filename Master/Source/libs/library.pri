depfile = $$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri)
include($$depfile)

isEmpty(DS_LIB_NAME): error("DS_LIB_NAME not defined for $$_PRO_FILE_. It should be defined in $$depfile")
TARGET = $$DS_LIB_NAME

include(../DolphinSense.pri)

# use precompiled header for libraries by default
contains(CONFIG, qt): isEmpty(PRECOMPILED_HEADER) {
    PRECOMPILED_HEADER = $$PWD/../shared/pch.h
}

DLLDESTDIR = $$DS_LIBRARY_DLL_PATH
DESTDIR = $$DS_LIBRARY_PATH

osx {
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/Frameworks/
}
#include(rpath.pri)

TARGET = $$qtLibraryTargetName($$TARGET)

TEMPLATE = lib
CONFIG += shared dll

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols
