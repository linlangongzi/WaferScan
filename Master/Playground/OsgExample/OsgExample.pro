#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T17:12:20
#
#-------------------------------------------------

QT += core gui
QT += opengl
QT += widgets
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OsgTutorial
TEMPLATE = app


SOURCES += main.cpp

FORMS    +=




win32:CONFIG(release, debug|release): LIBS += $$PWD/../../../osg-lib/lib/*.lib
else:win32:CONFIG(debug, debug|release): LIBS += $$PWD/../../../osg-lib/lib/*.lib

INCLUDEPATH += $$PWD/../../../osg-lib/include
DEPENDPATH += $$PWD/../../../osg-lib/include
