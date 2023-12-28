#-------------------------------------------------
#
# Project created by QtCreator 2014-11-24T17:49:34
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrainWheel_PXI
TEMPLATE = app
DEFINES += PLX_DLL_EXPORT
DEFINES += _XKEYCHECK_H
INCLUDEPATH += .
DEPENDPATH  += .

OBJECTS_DIR = obj
MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui

# Windows平台编译
win32 | win64 {
    message("Windows detected.")
#    Debug:      DESTDIR = build_Debug
#    Release:    DESTDIR = build_Release

    CONFIG += C++11         # GCC默认不支持C++11，禁用C++11将导致初始化列表无法使用

    QT += axcontainer
#    QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,5.01 #支持qt-msvc2013编译后在XP下运行

}

SOURCES += main.cpp\
    MainWindow.cpp\
    TcpServer/TcpServer.cpp \
    TcpServer/ParameterSocket.cpp \
    TcpServer/UTInstrumentDriver.cpp \
    ARS3D/UTData.cpp \
    ARS3D/UTDataContext.cpp \
    ARS3D/UTPose.cpp \
    Platform/Platform.cpp \
    ARS3D/Channel.cpp \
    ConfigObject/UTSetup.cpp \
    ConfigObject/Material.cpp \
    ConfigObject/UTGate.cpp \
    PanelUTSetup/PanelUTSetup.cpp \
    PanelUTSetup/PanelUTSetupTimebase.cpp \
    ConfigControl/ConfigControlInterface.cpp \
    ConfigControl/ConfigDoubleSlider.cpp \
    ConfigControl/ConfigDoubleSpinBox.cpp \
    ConfigControl/ConfigRadioButton.cpp \
    ConfigControl/ConfigSlider.cpp \
    ConfigControl/ConfigSpinBox.cpp \
    PanelUTGate/PanelUTGate.cpp \
    PanelTCG/PanelTCG.cpp \
    PanelTCG/PanelTCGEdit.cpp \
    ConfigObject/UTChannel.cpp \
    PanelMaterial/DialogMaterial.cpp \
    PanelMaterial/PanelMaterial.cpp \
    PanelMaterial/PanelMaterialEdit.cpp \
    ConfigControl/ConfigCombox.cpp \
    ConfigControl/ConfigCheckBox.cpp \
    TcpServer/CScanSocket.cpp \
    TcpServer/MonitorSocket.cpp \
    TcpServer/UTSetupSetOne.cpp \
    AbstractModel/AbstractItem.cpp \
    AbstractModel/ChannelFilterProxyModel.cpp \
    AbstractModel/ChannelItem.cpp \
    AbstractModel/ChannelItemModel.cpp \
    AbstractModel/ItemModel.cpp \
    ChannelManager/ChannelManager.cpp \
    PanelChannel/ChannelTreeView.cpp \
    PanelChannel/PanelChannelMgr.cpp \
    CardWork/AECWork.cpp \
    ConfigObject/UTInstrument.cpp \
    CardWork/MFCWork.cpp \
    TcpServer/SocketWork.cpp \
    CardWork/AECRuningWork.cpp \
    TcpServer/EyesStatusSocket.cpp \
    DriverPxi100/DriverPxi100.cpp \
    TcpServer/IncomingBaseSocket.cpp

HEADERS  += MainWindow.h \
    CardWork/HardwareAddress.h \
    TcpServer/TcpServer.h \
    TcpServer/ParameterSocket.h \
    TcpServer/DataDef.h \
    TcpServer/UTInstrumentDriver.h \
    ARS3D/UTData.h \
    ARS3D/UTDataContext.h \
    ARS3D/UTPose.h \
    Platform/Platform.h \
    ARS3D/Channel.h \
    ConfigObject/UTSetup.h \
    ConfigObject/Material.h \
    ConfigObject/UTGate.h \
    PanelUTSetup/PanelUTSetup.h \
    PanelUTSetup/PanelUTSetupTimebase.h \
    ConfigControl/ConfigControlInterface.h \
    ConfigControl/ConfigDoubleSlider.h \
    ConfigControl/ConfigDoubleSpinBox.h \
    ConfigControl/ConfigRadioButton.h \
    ConfigControl/ConfigSlider.h \
    ConfigControl/ConfigSpinBox.h \
    PanelUTGate/PanelUTGate.h \
    PanelTCG/PanelTCG.h \
    PanelTCG/PanelTCGEdit.h \
    ConfigObject/UTChannel.h \
    PanelMaterial/DialogMaterial.h \
    PanelMaterial/PanelMaterial.h \
    PanelMaterial/PanelMaterialEdit.h \
    ConfigControl/ConfigCombox.h \
    ConfigControl/ConfigCheckBox.h \
    TcpServer/CScanSocket.h \
    TcpServer/MonitorSocket.h \
    TcpServer/UTSetupSetOne.h \
    AbstractModel/AbstractItem.h \
    AbstractModel/ChannelFilterProxyModel.h \
    AbstractModel/ChannelItem.h \
    AbstractModel/ChannelItemModel.h \
    AbstractModel/ItemModel.h \
    ChannelManager/ChannelManager.h \
    PanelChannel/ChannelTreeView.h \
    PanelChannel/PanelChannelMgr.h \
    CardWork/AECWork.h \
    ConfigObject/UTInstrument.h \
    CardWork/MFCWork.h \
    TcpServer/SocketWork.h \
    CardWork/AECRuningWork.h \
    TcpServer/EyesStatusSocket.h \
    DriverPxi100/DriverPxi100.h \
    TcpServer/IncomingBaseSocket.h

FORMS    += MainWindow.ui \
    PanelUTSetup/PanelUTSetup.ui \
    PanelUTSetup/PanelUTSetupTimebase.ui \
    PanelUTGate/PanelUTGate.ui \
    PanelTCG/PanelTCG.ui \
    PanelTCG/PanelTCGEdit.ui \
    PanelMaterial/DialogMaterial.ui \
    PanelMaterial/PanelMaterial.ui \
    PanelMaterial/PanelMaterialEdit.ui \
    PanelChannel/PanelChannelMgr.ui

ARS_SOURCE_ROOT = ../Source
INCLUDEPATH += $$ARS_SOURCE_ROOT/pxiCom
#DEPENDPATH  += $$ARS_SOURCE_ROOT
include($$ARS_SOURCE_ROOT/pxiCom/pxiCom.pri)

INCLUDEPATH += $$ARS_SOURCE_ROOT/arsLua
include($$ARS_SOURCE_ROOT/arsLua/arsLua.pri)

#pci9054_path = $$PWD/lib/pci9054
#pci9054_lib_path = $$pci9054_path/lib
#pci9054_include_path = $$pci9054_path/include
#pci9054_bin_path = $$pci9054_path/bin

#INCLUDEPATH += $$pci9054_include_path
##DEPENDPATH += $$pci9054_include_path

##win32:CONFIG(release, debug|release): LIBS += -L$$PWD/pci9054_lib_path/ -lplx_dll
##else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/pci9054_lib_path/ -lplx_dlld
#win32:CONFIG(release, debug|release): LIBS += -L$$pci9054_lib_path/ -lplx_dll -lwdapi901
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$pci9054_lib_path/ -lplx_dlld -lwdapi901

##INCLUDEPATH += $$PWD/pci9054_dll
##DEPENDPATH += $$PWD/pci9054_dll

win32 {
    COPY = copy /y
} else {
    COPY = cp
}
# 复制pci9054的dll
#copy_dest = $$OUT_PWD/$$DESTDIR
#copy_source = $$pci9054_bin_path/*.dll
#COPY_CMD = $$COPY $$quote($$shell_path($$copy_source)) $$quote($$shell_path($$copy_dest))
#message("Copying pci9054 DLLs...")
#message($$COPY_CMD)
#system($$COPY_CMD)

# 复制ConfigTemplates中的Json模板
copy_source = $$PWD/ConfigTemplates/*
copy_dest = $$OUT_PWD/$$DESTDIR
COPY_CMD = $$COPY $$quote($$shell_path($$copy_source)) $$quote($$shell_path($$copy_dest))
message("Copying config templates...")
#message($$COPY_CMD)
system($$COPY_CMD)

RESOURCES += \
    ConfigControl/ConfigRadioImage.qrc

#SVN_REVISION = $$system("svnversion")
#isEmpty(SVN_REVISION) | contains(SVN_REVISION, Unversioned) {
#    SVN_REVISION = $$system("git log --max-count=1 --format=%h:%b")
#}
#DEFINES += SVN_REVISION=\"\\\"$$SVN_REVISION\\\"\"
#!build_pass: message("Revision: "$$SVN_REVISION)

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
HEADERS += $$PWD/SvnRevision.inc
versionTarget.target = $$PWD/SvnRevision.inc
versionTarget.depends = FORCE
host-win32: versionTarget.commands = $$PWD/SvnRevision.bat $$PWD $$PWD
else:       versionTarget.commands = $$PWD/SvnRevision.sh $$PWD $$PWD
PRE_TARGETDEPS += $$PWD/SvnRevision.inc
QMAKE_EXTRA_TARGETS += versionTarget
