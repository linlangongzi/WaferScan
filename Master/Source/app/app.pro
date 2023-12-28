DS_LIB_DEPENDS = extensionsystem

include(../DolphinSense.pri)
#include(../shared/qtsingleapplication/qtsingleapplication.pri)

QT += gui widgets
TEMPLATE = app
CONFIG += qtc_runnable sliced_bundle
TARGET = WaferScan
DESTDIR = $$DS_APP_PATH
VERSION = $$DS_VERSION
QT -= testlib

#HEADERS += ../tools/qtcreatorcrashhandler/crashhandlersetup.h
SOURCES += main.cpp #../tools/qtcreatorcrashhandler/crashhandlersetup.cpp
PRECOMPILED_HEADER += ../shared/pch_gui.h

#include(../rpath.pri)
#LIBS *= -l$$qtLibraryName(ExtensionSystem)# -l$$qtLibraryName(Aggregation) -l$$qtLibraryName(Utils)

macx {
    QMAKE_LFLAGS += -Wl,-rpath,@loader_path/../,-rpath,@executable_path/../
}

DS_REVISION_SVN = $$system("svnversion -n")
contains(DS_REVISION_SVN, Unversioned) {
    DS_REVISION_TYPE = git
    DS_REVISION = $$system("git log --max-count=1 --format=%h:%b")
    DS_BRANCH = $$system("git rev-parse --abbrev-ref HEAD")
} else {
    DS_REVISION_TYPE = svn
    DS_REPO_ROOT = $$system("svn info --show-item wc-root --no-newline")
    DS_REVISION = $$system("svn info $$DS_REPO_ROOT --show-item revision --no-newline")
    DS_BRANCH = $$system("svn info $$DS_REPO_ROOT --show-item relative-url --no-newline")
}
!build_pass: message("Revision: $$DS_REVISION in branch $$DS_BRANCH")

QT_BREAKPAD_ROOT_PATH = $$(QT_BREAKPAD_ROOT_PATH)
!isEmpty(QT_BREAKPAD_ROOT_PATH) {
    include($$QT_BREAKPAD_ROOT_PATH/qtbreakpad.pri)
}

win32 {
    RC_ICONS = Logo.ico
    QMAKE_TARGET_COMPANY = $$DS_AUTHOR
    QMAKE_TARGET_COPYRIGHT = $$DS_COPYRIGHT
    QMAKE_TARGET_PRODUCT = $$TARGET
} else:macx {
#    LIBS += -framework CoreFoundation
#    ASSETCATALOG.files = $$PWD/qtcreator.xcassets
#    macx-xcode {
#        QMAKE_BUNDLE_DATA += ASSETCATALOG
#    } else {
#        ASSETCATALOG.output = $$IDE_DATA_PATH/qtcreator.icns
#        ASSETCATALOG.commands = xcrun actool \
#            --app-icon qtcreator \
#            --output-partial-info-plist $$shell_quote($(TMPDIR)/qtcreator.Info.plist) \
#            --platform macosx \
#            --minimum-deployment-target 10.7 \
#            --compile $$shell_quote($$IDE_DATA_PATH) \
#            $$shell_quote($$PWD/qtcreator.xcassets) > /dev/null
#        ASSETCATALOG.input = ASSETCATALOG.files
#        ASSETCATALOG.CONFIG += no_link target_predeps
#        QMAKE_EXTRA_COMPILERS += ASSETCATALOG
#        icns.files = \
#            $$IDE_DATA_PATH/qtcreator.icns \
#            $$IDE_DATA_PATH/prifile.icns \
#            $$IDE_DATA_PATH/profile.icns
#        icns.path = $$INSTALL_DATA_PATH
#        icns.CONFIG += no_check_exist
#        INSTALLS += icns
#    }
#    QMAKE_INFO_PLIST = Info.plist
}

#target.path = $$INSTALL_APP_PATH
#INSTALLS += target

DISTFILES += \
    Info.plist \
    $$PWD/app_version.h.in \
    ../.qmake.conf \
    ../.clang-format

exists(../config.pri) {
    DISTFILES += ../config.pri
}

QMAKE_SUBSTITUTES += $$PWD/app_version.h.in

CONFIG += no_batch

RESOURCES += \
    splash.qrc
