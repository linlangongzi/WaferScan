DEFINES += ULTRASONIC_LIBRARY

QT += gui widgets

include(../plugin.pri)

HEADERS += \
    UltrasonicPlugin.h \
    UltrasonicGlobal.h \
    UTSetup.h \
    UTSetupGate.h \
    UTSetupTimebase.h \
    InterpolateTable.h \
    Material.h \
    UTChannel.h \
    UTInstrument.h \
    IInstrumentDriver.h \
    UTData.h \
    UTDataHeader.h \
    Benchmark.h \
    UltrasonicManager.h \
    UTPose.h \
    InstrumentSetup.h \
    InstrumentOptionsEdit.h \
    InstrumentOptions.h \
    UltrasonicOptions.h \
    UltrasonicConstants.h

SOURCES += \
    UltrasonicPlugin.cpp \
    UTSetup.cpp \
    UTSetupGate.cpp \
    UTSetupTimebase.cpp \
    InterpolateTable.cpp \
    Material.cpp \
    UTChannel.cpp \
    UTInstrument.cpp \
    IInstrumentDriver.cpp \
    UTData.cpp \
    UTDataHeader.cpp \
    Benchmark.cpp \
    UltrasonicManager.cpp \
    InstrumentSetup.cpp \
    InstrumentOptionsEdit.cpp \
    InstrumentOptions.cpp \
    UltrasonicOptions.cpp

FORMS += \
    InstrumentOptionsEdit.ui \
    InstrumentOptions.ui \
    UltrasonicOptions.ui

RESOURCES += \
    Ultrasonic.qrc
