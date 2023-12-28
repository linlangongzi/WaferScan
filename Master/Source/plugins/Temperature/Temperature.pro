DEFINES += TEMPERATURE_LIBRARY

QT += gui widgets charts serialport

include(../plugin.pri)

HEADERS += \
    TemperatureGlobal.h \
    TemperaturePlugin.h \
    TemperatureView.h \
    TemperatureManager.h \
    AT4500.h

SOURCES += \
    TemperaturePlugin.cpp \
    TemperatureView.cpp \
    TemperatureManager.cpp \
    AT4500.cpp

DISTFILES += Temperature.json.in

FORMS += \
    TemperatureView.ui
