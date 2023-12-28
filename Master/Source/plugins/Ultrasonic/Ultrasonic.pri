DEFINES += DS_ULTRASONIC_STATIC_LIB

HEADERS += \
    $$PWD/UTChannel.h \
    $$PWD/UTData.h \
    $$PWD/UTDataContext.h \
    $$PWD/UTDataHeader.h \
    $$PWD/UTInstrument.h \
    $$PWD/UTPose.h \
    $$PWD/UTSetup.h \
    $$PWD/UTSetupGate.h \
    $$PWD/IInstrumentDriver.h \
    $$PWD/Benchmark.h \
    $$PWD/InterpolateTable.h \
    $$PWD/Material.h \
    $$PWD/InstrumentStatusButton.h \
    $$PWD/UTStreamFile.h \
    $$PWD/UltrasonicGlobal.h \
    $$PWD/InstrumentSetup.h \
    $$PWD/UltrasonicManager.h \
    $$PWD/UltrasonicPlugin.h \
    $$PWD/InstrumentModel.h \
    $$PWD/InstrumentOptions.h \
    $$PWD/InstrumentOptionsEdit.h \
    $$PWD/ChannelConfig.h \
    $$PWD/ChannelModel.h \
    $$PWD/ChannelOptions.h \
    $$PWD/DialogChannelConfig.h \
    $$PWD/UTSetupTimebase.h \
    $$PWD/Probe.h \
    $$PWD/UltrasonicOptions.h \
    $$PWD/DialogConnect.h \
    $$PWD/InstrumentConnection.h \
    $$PWD/DgsTable.h \
    $$PWD/DgsCurveModel.h

SOURCES += \
    $$PWD/UTChannel.cpp \
    $$PWD/UTData.cpp \
    $$PWD/UTDataContext.cpp \
    $$PWD/UTDataHeader.cpp \
    $$PWD/UTInstrument.cpp \
    $$PWD/UTPose.cpp \
    $$PWD/UTSetup.cpp \
    $$PWD/UTSetupGate.cpp \
    $$PWD/IInstrumentDriver.cpp \
    $$PWD/Benchmark.cpp \
    $$PWD/InterpolateTable.cpp \
    $$PWD/Material.cpp \
    $$PWD/InstrumentStatusButton.cpp \
    $$PWD/UTStreamFile.cpp \
    $$PWD/InstrumentSetup.cpp \
    $$PWD/UltrasonicManager.cpp \
    $$PWD/UltrasonicPlugin.cpp \
    $$PWD/InstrumentModel.cpp \
    $$PWD/InstrumentOptions.cpp \
    $$PWD/InstrumentOptionsEdit.cpp \
    $$PWD/ChannelConfig.cpp \
    $$PWD/ChannelModel.cpp \
    $$PWD/ChannelOptions.cpp \
    $$PWD/DialogChannelConfig.cpp \
    $$PWD/UTSetupTimebase.cpp \
    $$PWD/Probe.cpp \
    $$PWD/UltrasonicOptions.cpp \
    $$PWD/DialogConnect.cpp \
    $$PWD/InstrumentConnection.cpp \
    $$PWD/DgsTable.cpp \
    $$PWD/DgsCurveModel.cpp

FORMS += \
    $$PWD/InstrumentOptions.ui \
    $$PWD/InstrumentOptionsEdit.ui \
    $$PWD/ChannelOptions.ui \
    $$PWD/DialogChannelConfig.ui \
    $$PWD/UltrasonicOptions.ui \
    $$PWD/DialogConnect.ui \
    $$PWD/InstrumentConnection.ui

RESOURCES += \
    $$PWD/Ultrasonic.qrc
