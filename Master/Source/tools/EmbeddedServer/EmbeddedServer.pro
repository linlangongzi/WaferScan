QT += core network

TARGET = EmbeddedServer
CONFIG += console c++11
CONFIG -= app_bundle
TEMPLATE = app

target.path = /home/root/EmbeddedServer
INSTALLS += target

ARS_LIB_ROOT = $$PWD/../../../../Lib
include(../../../Libs/lua.pri)

INCLUDEPATH += $$OUT_PWD

SOURCES += \
    main.cpp \
    Device.cpp \
    Emulator.cpp \
    Client.cpp \
    Server.cpp

HEADERS += \
    Device.h \
    Emulator.h \
    Client.h \
    Server.h

DS_REVISION_SVN = $$system("svnversion -n")
contains(DS_REVISION_SVN, Unversioned) {
    DS_REVISION_TYPE = git
    DS_REVISION = $$system("git log --max-count=1 --format=%h:%b")
    DS_BRANCH = $$system("git rev-parse --abbrev-ref HEAD")
} else {
    DS_REVISION_TYPE = svn
    DS_REPO_ROOT = $$system("svn info --show-item wc-root --no-newline")
    DS_REVISION = $$system("svn info $$DS_REPO_ROOT --show-item last-changed-revision --no-newline")
    DS_BRANCH = $$system("svn info $$DS_REPO_ROOT --show-item relative-url --no-newline")
}
!build_pass: message("Revision: $$DS_REVISION in branch $$DS_BRANCH")
QMAKE_SUBSTITUTES += $$PWD/../../app/app_version.h.in
