#-------------------------------------------------
#
# Project created by QtCreator 2015-10-31T23:39:42
#
#-------------------------------------------------

QT       += core gui
QT       += serialport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobotControler
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
        MainWindow.cpp \
    SixJoints.cpp \
    SerialPort.cpp \
    SysParas.cpp \
    ForwardSolver.cpp

HEADERS  += MainWindow.h \
    SixJoints.h \
    SerialPort.h \
    SysParas.h \
    ForwardSolver.h

FORMS    += MainWindow.ui \
    SixJoints.ui \
    SerialPort.ui
