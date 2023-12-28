DEFINES += WAVEFORM_LIBRARY

QT += gui widgets

include(../plugin.pri)

HEADERS += \
    WaveformGlobal.h \
    WaveformPlugin.h \
    DialogMaterial.h \
    PanelMaterial.h \
    PanelUTSetup.h \
    PanelUTSetupTimebase.h \
    PanelAScan.h \
    AbstractGraphicsView.h \
    AScanGraphicsView.h \
    GraphicsAxis.h \
    GraphicsGateItem.h \
    PlotterRuler.h \
    AlarmModelLabel.h \
    UTGraphicsScene.h \
    UTGraphicsView.h \
    GraphicsMarkItem.h \
    GraphicsInvertedLine.h \
    GraphicsEditablePolygon.h

SOURCES += \
    WaveformPlugin.cpp \
    DialogMaterial.cpp \
    PanelMaterial.cpp \
    PanelUTSetup.cpp \
    PanelUTSetupTimebase.cpp \
    PanelAScan.cpp \
    AbstractGraphicsView.cpp \
    AScanGraphicsView.cpp \
    GraphicsAxis.cpp \
    GraphicsGateItem.cpp \
    PlotterRuler.cpp \
    AlarmModelLabel.cpp \
    UTGraphicsScene.cpp \
    UTGraphicsView.cpp \
    GraphicsMarkItem.cpp \
    GraphicsEditablePolygon.cpp

FORMS += \
    DialogMaterial.ui \
    DialogUTSetupTimebase.ui \
    PanelMaterial.ui \
    PanelUTSetup.ui \
    PanelUTSetupTimebase.ui \
    PanelAScan.ui \
    AbstractGraphicsView.ui
