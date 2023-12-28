msvc {
    HEADERS += \
        $$PWD/DataDefine.h \
        $$PWD/opcda.h \
        $$PWD/OPCDataCallback.h \
        $$PWD/opcerrors.h \
        $$PWD/SimpleFree.h

    SOURCES += \
        $$PWD/OPCDataCallback.cpp \
        $$PWD/SimpleFree.cpp \
        $$PWD/opcda_i.c

    DEFINES += OPC
}
