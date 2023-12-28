#-------------------------------------------------
#
# Project created by QtCreator 2015-06-18T14:39:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = call_matlab
TEMPLATE = app


SOURCES += main.cpp\
        matlab.cpp

HEADERS  += matlab.h

FORMS    += matlab.ui


INCLUDEPATH += C:\MATLAB\R2014a\extern\include
LIBS += C:\MATLAB\R2014a\extern\lib\win64\microsoft\libeng.lib
LIBS += C:\MATLAB\R2014a\extern\lib\win64\microsoft\libmat.lib
LIBS += C:\MATLAB\R2014a\extern\lib\win64\microsoft\libmx.lib
LIBS += C:\MATLAB\R2014a\extern\lib\win64\microsoft\libmex.lib
