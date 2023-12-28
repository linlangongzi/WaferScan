DEFINES += DS_PLOT_STATIC_LIB

QT += charts

HEADERS += \
    $$PWD/PlotGlobal.h \
    $$PWD/PlotConfig.h \
    $$PWD/PlotCurveEdit.h \
    $$PWD/PlotLineEdit.h \
    $$PWD/PlotViewEditor.h \
    $$PWD/LocalScan.h \
    $$PWD/BScanChart.h \
    $$PWD/PlotPlugin.h

SOURCES += \
    $$PWD/PlotConfig.cpp \
    $$PWD/PlotCurveEdit.cpp \
    $$PWD/PlotLineEdit.cpp \
    $$PWD/PlotViewEditor.cpp \
    $$PWD/LocalScan.cpp \
    $$PWD/BScanChart.cpp \
    $$PWD/PlotPlugin.cpp

FORMS += \
    $$PWD/PlotCurveEdit.ui \
    $$PWD/PlotLineEdit.ui \
    $$PWD/PlotViewEditor.ui \
    $$PWD/LocalScan.ui

STATECHARTS += \
    $$PWD/LocalScan.scxml
