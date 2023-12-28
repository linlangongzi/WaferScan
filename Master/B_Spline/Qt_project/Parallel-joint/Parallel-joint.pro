#-------------------------------------------------
#
# Project created by QtCreator 2015-07-02T10:57:56
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Parallel-joint
TEMPLATE = app


SOURCES += main.cpp\
        joint_control.cpp

HEADERS  += joint_control.h

FORMS    += joint_control.ui

