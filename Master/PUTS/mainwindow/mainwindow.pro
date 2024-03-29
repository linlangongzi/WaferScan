# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = PUTS
QT += core gui sql axcontainer printsupport
QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,5.01
#CONFIG += debug axserver
#DEFINES += _WINDOWS QT_LARGEFILE_SUPPORT QT_HAVE_MMX QT_HAVE_3DNOW QT_HAVE_SSE QT_HAVE_MMXEXT QT_HAVE_SSE2 _PLX QAXSERVER _USING_V110_SDK71_
DEFINES += _ALLOW_KEYWORD_MACROS NOMINMAX __MIDL_user_allocate_free_DEFINED__ _USING_V110_SDK71_ _UNICODE UNICODE
LIBS += \
#    -lQAxContainer \
#    -lgdi32 \
#    -lcomdlg32 \
#    -loleaut32 \
#    -limm32 \
#    -lwinmm \
#    -lwinspool \
#    -lws2_32 \
#    -lole32 \
#    -luser32 \
#    -ladvapi32 \
#    -lmsimg32 \
#    -lshell32 \
#    -lkernel32 \
#    -luuid \
    $$PWD/plx_dll.lib \
    $$PWD/elep.lib
#    -lQAxServerd

lessThan(QT_MAJOR_VERSION, 5) {
    INCLUDEPATH += ../../Qt4Compatible
}

QMAKE_CFLAGS_RELEASE -= -Zc:strictStrings

DEPENDPATH += .
DESTDIR = .

INCLUDEPATH += ../../ARS3D
include(mainwindow.pri)
TRANSLATIONS += ./mainwindow_en.ts \
    ./mainwindow_zh.ts
win32:RC_FILE = mainwindow.rc

CODECFORTR += UTF-8

FORMS += \
    ascan.ui

HEADERS += \
    ../../ARS3D/ARS3D/UTImagePalette.h \
    ../../ARS3D/ARS3D/UTImagePrivate.h \
    ../../ARS3D/Defect/PUTS/ClassFactory.h

SOURCES += \
    ../../ARS3D/ARS3D/UTImagePrivate.cpp \
    ../../ARS3D/Defect/PUTS/ClassFactory.cpp

install_dlls.files = plx_dll.dll elep.dll wdapi901.dll
install_dlls.path = $$OUT_PWD
INSTALLS += install_dlls

install_config.files = database.cfg channelglobal.txt
install_config.path = $$OUT_PWD
INSTALLS += install_config

ARS_LIB_ROOT = ../../../Lib
include(../../Libs/opencv.pri)
