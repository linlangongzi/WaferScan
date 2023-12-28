include(../library.pri)

dll {
    DEFINES += UTILS_LIBRARY
} else {
    DEFINES += QTCREATOR_UTILS_STATIC_LIB
}

QT += gui widgets

HEADERS += \
    algorithm.h \
    categorysortfiltermodel.h \
    executeondestruction.h \
    fadingindicator.h \
    fancymainwindow.h \
    hostosinfo.h \
    icon.h \
    itemviews.h \
    osspecificaspects.h \
    proxyaction.h \
    qtcassert.h \
    stylehelper.h \
    theme/theme.h \
    theme/theme_p.h \
    treemodel.h \
    utils_global.h \
    utilsicons.h \
    stringutils.h \
    fileutils.h \
    savefile.h \
    benchmarker.h \
    synchronousprocess.h \
    predicates.h \
    qtcolorbutton.h

SOURCES += \
    categorysortfiltermodel.cpp \
    fadingindicator.cpp \
    fancymainwindow.cpp \
    hostosinfo.cpp \
    icon.cpp \
    itemviews.cpp \
    proxyaction.cpp \
    qtcassert.cpp \
    stylehelper.cpp \
    theme/theme.cpp \
    treemodel.cpp \
    utilsicons.cpp \
    stringutils.cpp \
    fileutils.cpp \
    savefile.cpp \
    benchmarker.cpp \
    synchronousprocess.cpp \
    qtcolorbutton.cpp

RESOURCES += \
    utils.qrc

# Used by FileUtils::normalizePathName
#win32: LIBS += -lshell32
#osx {
#    HEADERS += \
#        $$PWD/fileutils_mac.h
#    OBJECTIVE_SOURCES += \
#        $$PWD/fileutils_mac.mm
#    LIBS += -framework Foundation #-framework AppKit
#}
