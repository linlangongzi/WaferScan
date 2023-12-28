SOURCES += $$PWD/CardWork/CardRunningWork.cpp \
    $$PWD/CardWork/CardWorkBase.cpp \
    $$PWD/CardWork/PLXCommunication.cpp \
    $$PWD/ThreadController/ThreadWork.cpp \
    $$PWD/ThreadController/ThreadController.cpp \
    $$PWD/ProbeConnectManager/ProbeConnectManager.cpp \
    $$PWD/ConfigObject/Config.cpp \
    $$PWD/ConfigObject/ConfigManager.cpp \
    $$PWD/ConfigObject/ConfigObject.cpp \
    $$PWD/ConfigObject/JsonFile.cpp \
    $$PWD/ConfigObject/User.cpp \
    $$PWD/TcpServer/TcpServerBase.cpp \
    $$PWD/TcpServer/SocketWorkBase.cpp \
    $$PWD/Platform/PlatformBase.cpp

HEADERS += $$PWD/CardWork/CardRunningWork.h \
    $$PWD/CardWork/CardWorkBase.h \
    $$PWD/CardWork/PLXCommunication.h \
    $$PWD/CardWork/CardDataDefine.h \
    $$PWD/CardWork/CardHardwareAddr.h \
    $$PWD/ThreadController/ThreadWork.h \
    $$PWD/ThreadController/ThreadController.h \
    $$PWD/ProbeConnectManager/ProbeConnectManager.h \
    $$PWD/ConfigObject/Config.h \
    $$PWD/ConfigObject/ConfigManager.h \
    $$PWD/ConfigObject/ConfigObject.h \
    $$PWD/ConfigObject/JsonFile.h \
    $$PWD/ConfigObject/User.h \
    $$PWD/const.h \
    $$PWD/TcpServer/TcpServerBase.h \
    $$PWD/TcpServer/SocketWorkBase.h \
    $$PWD/Platform/PlatformBase.h \
    $$PWD/pxiCom.hpp

ARS_LIB_ROOT = $$PWD/../../../Lib
!exists($$ARS_LIB_ROOT) {
    ARS_LIB_ROOT = $$PWD/lib
#    warning("External libraries is moving outside of SVN repo. Please copy \\192.168.1.220\DevTools\Lib to svn\..\Lib")
}

include(../../Libs/pci9054.pri)
