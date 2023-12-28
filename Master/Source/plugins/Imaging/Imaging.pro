QT += widgets datavisualization charts concurrent

include(../plugin.pri)

DEFINES += IMAGING_LIBRARY

HEADERS += \
    ImagingPlugin.h \
    ArrowRadioButton.h \
    ColorPaletteEdit.h \
    CScanDefectParameter.h \
    CursorManager.h \
#    DefectGraphicsItem.h \
#    DefectItem.h \
##    DefectManager.h \
    DialogChannelOffset.h \
    DialogColorPaletteList.h \
#    DialogDefectProperty.h \
    DialogImageABCOptions.h \
    DialogImageOptions.h \
#    EditDelegate.h \
    GraphicsDotItem.h \
#    GraphicsInvertedLine.h \
#    GraphicsPolygonItem.h \
#    Panel3DSurface.h \
#    Panel3DVolume.h \
    PanelCScan.h \
    PolygonRecordManager.h \
    ProbeIndicatorManager.h \
    RecordLineItem.h \
#    ReportDlg.h \
    ReportWriter.h \
    ReportWriterQtExcel.h \
#    SafeRectManager.h \
    ImageRecord.h \
    ImageOptions.h \
    UTImage.h \
    UTImagePalette.h \
    ColorPalette.h \
    ColorPaletteView.h \
#    UTDefect.h \
    UTDefectProcessorOptions.h \
    ImagingGlobal.h \
    ImageRecordLayerModel.h \
    PanelImageControl.h \
    ImageRecorder.h \
    Magnifier.h \
    PanelImport.h \
    ImagingConstants.h \
    ImageRecordLayer.h \
    DialogImageRecord.h

SOURCES += \
    ImagingPlugin.cpp \
    ArrowRadioButton.cpp \
    ColorPaletteEdit.cpp \
    CScanDefectParameter.cpp \
    CursorManager.cpp \
#    DefectGraphicsItem.cpp \
#    DefectItem.cpp \
##    DefectManager.cpp \
    DialogChannelOffset.cpp \
    DialogColorPaletteList.cpp \
#    DialogDefectProperty.cpp \
    DialogImageABCOptions.cpp \
    DialogImageOptions.cpp \
#    EditDelegate.cpp \
    GraphicsDotItem.cpp \
#    GraphicsPolygonItem.cpp \
#    Panel3DSurface.cpp \
#    Panel3DVolume.cpp \
    PanelCScan.cpp \
    PolygonRecordManager.cpp \
    ProbeIndicatorManager.cpp \
    RecordLineItem.cpp \
#    ReportDlg.cpp \
    ReportWriter.cpp \
    ReportWriterQtExcel.cpp \
#    SafeRectManager.cpp \
    ImageRecord.cpp \
    ImageOptions.cpp \
    UTImage.cpp \
    ColorPalette.cpp \
    ColorPaletteView.cpp \
#    UTDefect.cpp \
    UTDefectProcessorOptions.cpp \
    ImageRecordLayerModel.cpp \
    PanelImageControl.cpp \
    ImageRecorder.cpp \
    Magnifier.cpp \
    PanelImport.cpp \
    ImageRecordLayer.cpp \
    DialogImageRecord.cpp

FORMS += \
    ColorPaletteEdit.ui \
    CScanDefectParameter.ui \
    DialogChannelOffset.ui \
    DialogColorPaletteList.ui \
#    DialogDefectProperty.ui \
    DialogImageABCOptions.ui \
    DialogImageOptions.ui \
#    Panel3DSurface.ui \
#    Panel3DVolume.ui \
    PanelCScan.ui \
#    ReportDlg.ui
    PanelImageControl.ui \
    Magnifier.ui \
    PanelImport.ui \
    DialogImageRecord.ui

DISTFILES += \
    template.xlsx
