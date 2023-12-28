QT += gui widgets charts

include(../plugin.pri)

DEFINES += PLOT_LIBRARY

HEADERS += \
    PlotPlugin.h \
    PlotGlobal.h \
    PlotConfig.h \
    PlotCurveEdit.h \
    PlotLineEdit.h \
    PlotViewEditor.h \
    LocalScan.h \
    BScanChart.h

SOURCES += \
    PlotPlugin.cpp \
    PlotConfig.cpp \
    PlotCurveEdit.cpp \
    PlotLineEdit.cpp \
    PlotViewEditor.cpp \
    LocalScan.cpp \
    BScanChart.cpp

FORMS += \
    PlotCurveEdit.ui \
    PlotLineEdit.ui \
    PlotViewEditor.ui \
    LocalScan.ui

STATECHARTS += \
    LocalScan.scxml
