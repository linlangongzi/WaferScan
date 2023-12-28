QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# Windows 64 detection
win32:contains(QMAKE_TARGET.arch, x86_64) {
    CONFIG += win64
}


#QT -= gui

CONFIG += C++11
TARGET = BSpline
#CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../../Source

SOURCES += main.cpp \
    BSplineCurve.cpp \
    BSplineSurface.cpp \
    MyMath.cpp \
    BSpline.cpp \
    PathPlanning.cpp \
    ../../../Source/arsCore/TriangulatedSurface.cpp \
    ../../../Source/arsCore/QtExtend.cpp

HEADERS += \
    BSplineCurve.h \
    BSplineSurface.h \
    MyMath.h \
    BSpline.h \
    PathPlanning.h \
    ../../../Source/arsCore/TriangulatedSurface.h \
    ../../../Source/arsCore/QtExtend.h

ARS_LIB_ROOT = $$PWD/../../../../Lib
ARS_LIB_INC_ROOT = $$PWD/../../../Libs
include($$ARS_LIB_INC_ROOT/opencv.pri)
