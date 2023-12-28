# This file could be broken when Chinese characters are used

lessThan(QT_MAJOR_VERSION, 5) | lessThan(QT_MINOR_VERSION, 5) {
    error(This application may only be compiled with Qt 5.5 or higher)
}
QT += core gui widgets
win32: QT += winextras

# Target executable WaferScan.exe
TARGET = WaferScan

TEMPLATE = app
DEPENDPATH += .
DESTDIR = $$DS_BIN_PATH

OBJECTS_DIR = obj
MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui

RC_ICONS = resources/Logo.ico
ICON = resources/Logo.icns

INCLUDEPATH += $$DS_SRC_PATH

DS_LIB_DEPENDS = extensionsystem allrising utils lua qtxlsx
DS_EXTERNAL_DEPENDS = opencv elep

PRECOMPILED_HEADER = shared/pch_gui.h

PLUGINS_AVAILABLE = $$fromfile(plugins/plugins.pro, SUBDIRS)
for(plugin, PLUGINS_AVAILABLE) {
    include($$PWD/plugins/$${plugin}/$${plugin}.pri)
}
include(DolphinSense.pri)

BREAKPAD_SOURCE_DIR = $$DS_EXTERNAL_LIBRARY_ROOT/breakpad
include(libs/qt-breakpad/qtbreakpad.pri)

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
    VERSION = $${DS_VERSION}.$${DS_REVISION}
}
!build_pass: message("Version: $$VERSION rev $$DS_REVISION in branch $$DS_BRANCH")
QMAKE_SUBSTITUTES += app/app_version.h.in

QMAKE_TARGET_PRODUCT = $$DS_APP_NAME
QMAKE_TARGET_DESCRIPTION = $$DS_APP_DISPLAY_NAME
QMAKE_TARGET_COMPANY = $$DS_AUTHOR_LONG
QMAKE_TARGET_COPYRIGHT = $$DS_COPYRIGHT

SOURCES += \
    app/main.cpp \
    app/PlatformTranslations.cpp

RESOURCES += \
    resources/Resources.qrc

DISTFILES += \
    .qmake.conf \
    app/app_version.h.in
