#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T14:39:51
#
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 5) | lessThan(QT_MINOR_VERSION, 3) {
    error(This application may only be compiled with Qt 5.3 or later)
}

!build_pass: lessThan(QT_MINOR_VERSION, 5) {
    warning("This application is suggested to be compiled with Qt 5.5")
}

win32: QT += winextras
QT += network
QT += core gui widgets

TARGET = PipeDetect_PXI
TEMPLATE = app
DEFINES += PLX_DLL_EXPORT
DEFINES += _XKEYCHECK_H
DEFINES += PIPE_DETECT
INCLUDEPATH += .
DEPENDPATH  += .

# Windows平台编译
win32 | win64 {
    message("Windows detected.")
#    Debug:      DESTDIR = build_Debug
#    Release:    DESTDIR = build_Release

    CONFIG += C++11         # GCC默认不支持C++11，禁用C++11将导致初始化列表无法使用

    QT += axcontainer
#    QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,5.01
}

SOURCES += MainWindow/main.cpp \
        MainWindow/MainWindow.cpp \
    CardWork/AECRuningWork.cpp \
    CardWork/AECWork.cpp \
    CardWork/MFCWork.cpp \
    ConfigObject/Material.cpp \
    ConfigObject/UTChannel.cpp \
    ConfigObject/UTGate.cpp \
    ConfigObject/UTInstrument.cpp \
    ConfigObject/UTSetup.cpp \
    ARS3D/Channel.cpp \
    ARS3D/UTData.cpp \
    ARS3D/UTDataContext.cpp \
    ARS3D/UTPose.cpp \
    Platform/Platform.cpp \
    TcpServer/CScanSocket.cpp \
    TcpServer/MonitorSocket.cpp \
    TcpServer/ParameterSocket.cpp \
    TcpServer/SocketWork.cpp \
    TcpServer/TcpServer.cpp \
    TcpServer/UTInstrumentDriver.cpp \
    TcpServer/UTSetupSetOne.cpp \
    CardWork/MFCRunningWork.cpp

HEADERS  += MainWindow/MainWindow.h \
    CardWork/AECRuningWork.h \
    CardWork/AECWork.h \
    CardWork/HardwareAddress.h \
    CardWork/MFCWork.h \
    ConfigObject/Material.h \
    ConfigObject/UTChannel.h \
    ConfigObject/UTGate.h \
    ConfigObject/UTInstrument.h \
    ConfigObject/UTSetup.h \
    ARS3D/Channel.h \
    ARS3D/UTData.h \
    ARS3D/UTDataContext.h \
    ARS3D/UTPose.h \
    Platform/Platform.h \
    TcpServer/CScanSocket.h \
    TcpServer/DataDef.h \
    TcpServer/MonitorSocket.h \
    TcpServer/ParameterSocket.h \
    TcpServer/SocketWork.h \
    TcpServer/TcpServer.h \
    TcpServer/UTInstrumentDriver.h \
    TcpServer/UTSetupSetOne.h \
    CardWork/MFCRunningWork.h

FORMS    += MainWindow/MainWindow.ui

ARS_SOURCE_ROOT = ../Source/pxiCom
INCLUDEPATH += $$ARS_SOURCE_ROOT
include($$ARS_SOURCE_ROOT/pxiCom.pri)

# 复制ConfigTemplates中的Json模板
copy_source = $$PWD/ConfigTemplates/*
copy_dest = $$OUT_PWD/$$DESTDIR
COPY_CMD = $$COPY $$quote($$shell_path($$copy_source)) $$quote($$shell_path($$copy_dest))
message("Copying config templates...")
#message($$COPY_CMD)
system($$COPY_CMD)
