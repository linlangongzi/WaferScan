#-------------------------------------------------
#
# Project created by QtCreator 2014-12-12T11:36:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UTSetupToJson
TEMPLATE = app

PRECOMPILED_HEADER += ../Source/arsCore/ExecEncodingUTF8.hpp

SOURCES += main.cpp\
        UTSetupToJson.cpp \
    ../TrainWheelDetect/ConfigObject/Config.cpp \
    ../TrainWheelDetect/ConfigObject/ConfigObject.cpp \
    ../TrainWheelDetect/ConfigObject/JsonFile.cpp \
    ../TrainWheelDetect/ConfigObject/User.cpp \
    ../TrainWheelDetect/ConfigObject/ConfigManager.cpp \
    ../TrainWheelDetect/Util/Util.cpp \
    ../TrainWheelDetect/ProbeNoManager/ProbeNoInfo.cpp \
    ../TrainWheelDetect/ProbeNoManager/ProbeNoManager.cpp

HEADERS  += UTSetupToJson.h \
    ../TrainWheelDetect/ConfigObject/Config.h \
    ../TrainWheelDetect/ConfigObject/ConfigObject.h \
    ../TrainWheelDetect/ConfigObject/JsonFile.h \
    ../TrainWheelDetect/ConfigObject/User.h \
    ../TrainWheelDetect/ConfigObject/ConfigManager.h \
    ../TrainWheelDetect/Util/Util.h \
    ../TrainWheelDetect/Util/JsonConverts.h \
    ../TrainWheelDetect/ProbeNoManager/ProbeNoInfo.h \
    ../TrainWheelDetect/ProbeNoManager/ProbeNoManager.h

FORMS    += UTSetupToJson.ui

INCLUDEPATH += ../TrainWheelDetect

# 复制ConfigTemplates中的Json模板
win32 {
    COPY = copy /y
} else {
    COPY = cp
}
copy_source = $$PWD/ConfigTemplates/*
copy_dest = $$OUT_PWD/$$DESTDIR
COPY_CMD = $$COPY $$quote($$shell_path($$copy_source)) $$quote($$shell_path($$copy_dest))
message("Copying config templates...")
#message($$COPY_CMD)
system($$COPY_CMD)
