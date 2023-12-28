HEADERS += \
    $$PWD/algorithm.h \
    $$PWD/categorysortfiltermodel.h \
    $$PWD/itemviews.h \
    $$PWD/qtcassert.h \
    $$PWD/treemodel.h \
    $$PWD/utils_global.h \
    $$PWD/icon.h \
    $$PWD/utilsicons.h \
    $$PWD/theme/theme.h \
    $$PWD/theme/theme_p.h \
    $$PWD/stylehelper.h \
    $$PWD/hostosinfo.h \
    $$PWD/osspecificaspects.h

SOURCES += \
    $$PWD/categorysortfiltermodel.cpp \
    $$PWD/itemviews.cpp \
    $$PWD/qtcassert.cpp \
    $$PWD/treemodel.cpp \
    $$PWD/icon.cpp \
    $$PWD/theme/theme.cpp \
    $$PWD/stylehelper.cpp \
    $$PWD/hostosinfo.cpp

RESOURCES += \
    $$PWD/utils.qrc

DEFINES += QTCREATOR_UTILS_STATIC_LIB
