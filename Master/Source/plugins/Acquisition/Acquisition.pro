QT += widgets

include(../plugin.pri)

DEFINES += ACQUISITION_LIBRARY

FORMS += \
    PanelScanPlanControl.ui \
    PointXYZ.ui \
    UTSetupSelector.ui \
    DialogMotionCondition.ui \
    DialogMotionElement.ui \
    DialogMotionPose.ui \
    DialogRotationInfo.ui \
    ScanPathDiskEdit.ui \
    DialogScanPlanEdit.ui \
    ScanPathAdvancedEdit.ui \
    ScanPathRingEdit.ui \
    DialogScanTaskInfo.ui

HEADERS += \
    PanelScanPlanControl.h \
    PointXYZ.h \
    ScanController.h \
    ScanPlan.h \
    ScanPlanModel.h \
    ScanPlanTreeView.h \
    ScanTask.h \
    UTSetupSelector.h \
    ScanTaskModel.h \
    DialogMotionCondition.h \
    DialogMotionElement.h \
    DialogMotionPose.h \
    DialogRotationInfo.h \
    ScanPathDisk.h \
    ScanPathDiskEdit.h \
    DialogScanPlanEdit.h \
    ScanPathAdvanced.h \
    ScanPathAdvancedEdit.h \
    ScanPathFactory.h \
    ScanPathRing.h \
    ScanPathRingEdit.h \
    ScanTaskCustomField.h \
    DialogScanTaskInfo.h

SOURCES += \
    PanelScanPlanControl.cpp \
    PointXYZ.cpp \
    ScanController.cpp \
    ScanPlan.cpp \
    ScanPlanModel.cpp \
    ScanPlanTreeView.cpp \
    ScanTask.cpp \
    UTSetupSelector.cpp \
    ScanTaskModel.cpp \
    DialogMotionCondition.cpp \
    DialogMotionElement.cpp \
    DialogMotionPose.cpp \
    DialogRotationInfo.cpp \
    ScanPathDisk.cpp \
    ScanPathDiskEdit.cpp \
    DialogScanPlanEdit.cpp \
    ScanPathAdvanced.cpp \
    ScanPathAdvancedEdit.cpp \
    ScanPathFactory.cpp \
    ScanPathRing.cpp \
    ScanPathRingEdit.cpp \
    ScanTaskCustomField.cpp \
    DialogScanTaskInfo.cpp

STATECHARTS += \
    ScanController.scxml
