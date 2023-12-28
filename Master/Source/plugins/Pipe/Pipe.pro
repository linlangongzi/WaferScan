QT += widgets

include(../plugin.pri)

HEADERS += \
    PipePlugin.h \
    AlarmGroupFrame.h \
    AlarmLabel.h \
    DataManagerBase.h \
    GraphicsLineSeries.h \
    PanelAlarm.h \
    PanelAutoSave.h \
    PanelStatusFrame.h \
    PipeGlobalConfig.h \
    PipeModeEdit.h \
    PipePanelBScan.h \
    PipePanelBScanMain.h \
    PipeParameter.h \
    TestModeBase.h \
    PlotAlarmGroupEdit.h

SOURCES += \
    PipePlugin.cpp \
    AlarmGroupFrame.cpp \
    AlarmLabel.cpp \
    DataManagerBase.cpp \
    GraphicsLineSeries.cpp \
    PanelAlarm.cpp \
    PanelAutoSave.cpp \
    PanelStatusFrame.cpp \
    PipeGlobalConfig.cpp \
    PipeModeEdit.cpp \
    PipePanelBScan.cpp \
    PipePanelBScanMain.cpp \
    PipeParameter.cpp \
    TestModeBase.cpp \
    PlotAlarmGroupEdit.cpp

DISTFILES += \
    Pipe.json.in

FORMS += \
    AlarmGroupFrame.ui \
    PanelAlarm.ui \
    PanelAutoSave.ui \
    PanelStatusFrame.ui \
    PipeModeEdit.ui \
    PipePanelBScan.ui \
    PipePanelBScanMain.ui \
    PipeParameter.ui \
    TestModeBase.ui \
    PlotAlarmGroupEdit.ui
