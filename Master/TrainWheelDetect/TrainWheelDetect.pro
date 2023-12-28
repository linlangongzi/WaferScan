#-------------------------------------------------
#
# Project created by QtCreator 2014-11-03T19:25:25
#
#-------------------------------------------------

include($$PWD/../Source/.qmake.conf)

QT       += core gui charts
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrainWheelDetect
TEMPLATE = app
INCLUDEPATH += .
INCLUDEPATH += ../Source ../Source/WaferScan
DEPENDPATH  += .

OBJECTS_DIR = obj
MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui

RC_ICONS = ../Resources/Logo.ico

# Windows 64 detection
win32:contains(QMAKE_TARGET.arch, x86_64) {
    CONFIG += win64
}

# Windows平台编译
win32 | win64 {
    message("Windows detected.")
#    Debug:      DESTDIR = build_Debug
#    Release:    DESTDIR = build_Release

    CONFIG += C++11         # GCC默认不支持C++11，禁用C++11将导致初始化列表无法使用

    QT += axcontainer
}

ARS_LIB_ROOT = $$PWD/../../Lib
ARS_LIB_INC_ROOT = $$PWD/../Libs

exists($$PWD/config.pri): include(config.pri)
exists($$OUT_PWD/build.pri): include($$OUT_PWD/build.pri)

!exists($$ARS_LIB_ROOT) {
    ARS_LIB_ROOT = $$PWD/lib
    error(Library not found. Please checkout http://192.168.1.220/Lib to parent folder of trunk.)
}

SOURCES += \
    UTInstrumentDriver/PXI100/PXI100.cpp \
    UTInstrumentDriver/PXI100/CScanSocket.cpp \
    UTInstrumentDriver/PXI100/IncomingBaseSocket.cpp \
    UTInstrumentDriver/PXI100/MonitorSocket.cpp \
    UTInstrumentDriver/PXI100/ParameterSocket.cpp \
    UTInstrumentDriver/PXI100/Parser.cpp \
    BScanView/PanelBScan.cpp \
    ProbeNoManager/ProbeNoManager.cpp \
    PanelSensorDebug/PanelSensorDebug.cpp \
    UTInstrumentDriver/PXI100/EyesStatusSocket.cpp \
#    DialogTest/DialogTest.cpp \
#    DialogTest/TimerTest.cpp \
    PanelSensorStatus/PanelSensorStatus.cpp \
    PanelInformation/PanelInformation.cpp \
    PanelProbeBScan/PanelProbeBScan.cpp \
    ProbeNoManager/ProbeNoInfo.cpp \
    DialogGlobalConfig/DialogGlobalConfig.cpp \
    BScanView/DialogChannelGroup.cpp \
    DialogGlobalConfig/DialogFeatureFilter.cpp \
    DialogGlobalConfig/DialogFeatureFilters.cpp \
    Defect/DefectItem.cpp \
    Defect/PanelDefect.cpp \
    PanelChannel/ChannelItem.cpp \
    PanelChannel/PanelChannel.cpp \
    DialogGlobalConfig/DialogTrainConfig.cpp \
    Wheel/ControlPanel.cpp \
    Wheel/WheelPosition.cpp \
    Wheel/PanelTrainInfo.cpp \
    Wheel/PanelDocumentInfo.cpp \
    WheelDataset/WheelDataset.cpp \
    Wheel/DialogCheating.cpp \
    Wheel/WheelConfig.cpp

HEADERS  += \
    UTInstrumentDriver/PXI100/PXI100.h \
    UTInstrumentDriver/PXI100/CScanSocket.h \
    UTInstrumentDriver/PXI100/DataDef.h \
    UTInstrumentDriver/PXI100/IncomingBaseSocket.h \
    UTInstrumentDriver/PXI100/MonitorSocket.h \
    UTInstrumentDriver/PXI100/ParameterSocket.h \
    UTInstrumentDriver/PXI100/Parser.h \
    BScanView/PanelBScan.h \
    ProbeNoManager/ProbeNoManager.h \
    PanelSensorDebug/PanelSensorDebug.h \
    UTInstrumentDriver/PXI100/EyesStatusSocket.h \
#    DialogTest/DialogTest.h \
#    DialogTest/TimerTest.h \
    PanelSensorStatus/PanelSensorStatus.h \
    PanelInformation/PanelInformation.h \
    PanelProbeBScan/PanelProbeBScan.h \
    ProbeNoManager/ProbeNoInfo.h \
    DialogGlobalConfig/DialogGlobalConfig.h \
    BScanView/DialogChannelGroup.h \
    DialogGlobalConfig/DialogFeatureFilter.h \
    DialogGlobalConfig/DialogFeatureFilters.h \
    Defect/DefectItem.h \
    Defect/PanelDefect.h \
    PanelChannel/ChannelItem.h \
    PanelChannel/PanelChannel.h \
    DialogGlobalConfig/DialogTrainConfig.h \
    Wheel/ControlPanel.h \
    Wheel/WheelPosition.h \
    Wheel/PanelTrainInfo.h \
    Wheel/PanelDocumentInfo.h \
    WheelDataset/WheelDataset.h \
    Wheel/DialogCheating.h \
    Wheel/WheelConfig.h

FORMS    += \
    BScanView/PanelBScan.ui \
    PanelSensorDebug/PanelSensorDebug.ui \
#    DialogTest/DialogTest.ui \
    PanelSensorStatus/PanelSensorStatus.ui \
    PanelInformation/PanelInformation.ui \
    PanelProbeBScan/PanelProbeBScan.ui \
    DialogGlobalConfig/DialogGlobalConfig.ui \
    BScanView/DialogChannelGroup.ui \
    DialogGlobalConfig/DialogFeatureFilter.ui \
    DialogGlobalConfig/DialogFeatureFilters.ui \
    Defect/PanelDefect.ui \
    PanelChannel/PanelChannel.ui \
    DialogGlobalConfig/DialogTrainConfig.ui \
    Wheel/ControlPanel.ui \
    Wheel/PanelTrainInfo.ui \
    Wheel/PanelDocumentInfo.ui \
    Wheel/DialogCheating.ui

# MSVC Detection
contains(QMAKE_COMPILER, msvc) {
    CONFIG += win32-msvc
}

###############################

DEFINES += TRAINWHEEL

TRANSLATIONS += MainWindow/TrainWheelDetect_zhCN.ts
CODECFORTR += UTF-8

message("Generating translations...")
#system("lupdate TrainWheelDetect.pro")
system("lrelease TrainWheelDetect.pro")

DS_EXTERNAL_LIBRARY_ROOT = $$PWD/../../Lib

INCLUDEPATH +=  ../Source/libs ../Source/plugins ../Source/externals
DEFINES += QTCREATOR_UTILS_STATIC_LIB DS_CORE_STATIC_LIB
include($$ARS_LIB_INC_ROOT/elite-e.pri)
include($$PWD/../Source/legacy.pri)
#include($$PWD/../Source/plugins/plugins.pri)
include($$PWD/../Source/plugins/Core/Core.pri)
include($$PWD/../Source/plugins/Ultrasonic/Ultrasonic.pri)
include($$PWD/../Source/plugins/Waveform/Waveform.pri)
include($$PWD/../Source/plugins/Motion/Motion.pri)
include($$PWD/../Source/plugins/Imaging/Imaging.pri)
include($$PWD/../Source/plugins/Acquisition/Acquisition.pri)
include($$PWD/../Source/plugins/Plot/Plot.pri)
include($$PWD/../Source/libs/libs.pri)

PRECOMPILED_HEADER = $$PWD/../Source/shared/pch_gui.h

DS_EXTERNAL_DEPENDS = opencv elep
include($$PWD/../Source/DolphinSense.pri)

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

# Copy translate files
copy_source = $$PWD/MainWindow/TrainWheelDetect_zhCN.qm
COPY_CMD = $$COPY $$quote($$shell_path($$copy_source)) $$quote($$shell_path($$copy_dest))
message("Copying translate files...")
system($$COPY_CMD)

# Since qmake may be used for cross-compile, which means "win32" and "unix"
# indicate target platform.
# $$[QMAKE_SPEC] is a property of qmake executable, which could be used for
# host platform detection.
host_qmake_spec = $$[QMAKE_SPEC]
host_platform = $$find(host_qmake_spec, "^win32")
count(host_platform, 1) {   # Host is Windows
    CONFIG += host-win32
} else {
    CONFIG += host-unix
}

# Extract svn/git revision, and generate SvnRevision.inc
REVISION_STR = $$system("svnversion -n $$shell_path($$clean_path($$PWD/..))")
isEmpty(REVISION_STR) | contains(REVISION_STR, Unversioned) {
    REVISION_STR = $$system("git -C $$shell_path($$clean_path($$PWD/../..)) log --max-count=1 --format=%h:%b")
}
!build_pass: message("Using revision: $$REVISION_STR")
REVISION_INC = $$PWD/../Source/arsCore/SvnRevision.inc
REVISION_INC_IN = $${REVISION_INC}.in
DISTFILES += $$REVISION_INC_IN
revision_inc_subs.input = $$REVISION_INC_IN
revision_inc_subs.output=$$OUT_PWD/SvnRevision.inc
QMAKE_SUBSTITUTES += revision_inc_subs
