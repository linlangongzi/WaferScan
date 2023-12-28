FORMS += \
    $$PWD/PanelScanPlanControl.ui \
    $$PWD/PointXYZ.ui \
#    $$PWD/UTSetupSelector.ui \
    $$PWD/DialogMotionCondition.ui \
    $$PWD/DialogMotionElement.ui \
    $$PWD/DialogMotionPose.ui \
    $$PWD/DialogRotationInfo.ui \
    $$PWD/ScanPathDiskEdit.ui \
    $$PWD/DialogScanPlanEdit.ui \
    $$PWD/ScanPathAdvancedEdit.ui \
    $$PWD/ScanPathRingEdit.ui \
    $$PWD/DialogScanTaskInfo.ui \
    $$PWD/ScanPathRectangleEdit.ui \
    $$PWD/ScanPathCylinderEdit.ui \
    $$PWD/ChannelListWidget.ui \
    $$PWD/ChannelTreeWidget.ui \
    $$PWD/ScanSettings.ui \
    $$PWD/ScanPathRingSettings.ui

HEADERS += \
    $$PWD/PanelScanPlanControl.h \
    $$PWD/PointXYZ.h \
    $$PWD/ScanController.h \
    $$PWD/ScanPlan.h \
    $$PWD/ScanPlanModel.h \
    $$PWD/ScanPlanTreeView.h \
    $$PWD/ScanTask.h \
#    $$PWD/UTSetupSelector.h \
    $$PWD/ScanTaskModel.h \
    $$PWD/DialogMotionCondition.h \
    $$PWD/DialogMotionElement.h \
    $$PWD/DialogMotionPose.h \
    $$PWD/DialogRotationInfo.h \
    $$PWD/ScanPathDisk.h \
    $$PWD/ScanPathDiskEdit.h \
    $$PWD/DialogScanPlanEdit.h \
    $$PWD/ScanPathAdvanced.h \
    $$PWD/ScanPathAdvancedEdit.h \
    $$PWD/ScanPathFactory.h \
    $$PWD/ScanPathRing.h \
    $$PWD/ScanPathRingEdit.h \
    $$PWD/ScanTaskCustomField.h \
    $$PWD/DialogScanTaskInfo.h \
    $$PWD/AxisSelector.h \
    $$PWD/AcquisitionConstants.h \
    $$PWD/ScanPathRectangleEdit.h \
    $$PWD/ScanPathRectangle.h \
    $$PWD/AcquisitionPlugin.h \
    $$PWD/ScanPathCylinder.h \
    $$PWD/ScanPathCylinderEdit.h \
    $$PWD/ScanPathUtilities.h \
    $$PWD/ChannelListWidget.h \
    $$PWD/ChannelTreeWidget.h \
    $$PWD/ScanSettings.h \
    $$PWD/ScanPathRingSettings.h

SOURCES += \
    $$PWD/PanelScanPlanControl.cpp \
    $$PWD/PointXYZ.cpp \
    $$PWD/ScanController.cpp \
    $$PWD/ScanPlan.cpp \
    $$PWD/ScanPlanModel.cpp \
    $$PWD/ScanPlanTreeView.cpp \
    $$PWD/ScanTask.cpp \
#    $$PWD/UTSetupSelector.cpp \
    $$PWD/ScanTaskModel.cpp \
    $$PWD/DialogMotionCondition.cpp \
    $$PWD/DialogMotionElement.cpp \
    $$PWD/DialogMotionPose.cpp \
    $$PWD/DialogRotationInfo.cpp \
    $$PWD/ScanPathDisk.cpp \
    $$PWD/ScanPathDiskEdit.cpp \
    $$PWD/DialogScanPlanEdit.cpp \
    $$PWD/ScanPathAdvanced.cpp \
    $$PWD/ScanPathAdvancedEdit.cpp \
    $$PWD/ScanPathFactory.cpp \
    $$PWD/ScanPathRing.cpp \
    $$PWD/ScanPathRingEdit.cpp \
    $$PWD/ScanTaskCustomField.cpp \
    $$PWD/DialogScanTaskInfo.cpp \
    $$PWD/AxisSelector.cpp \
    $$PWD/ScanPathRectangleEdit.cpp \
    $$PWD/ScanPathRectangle.cpp \
    $$PWD/AcquisitionPlugin.cpp \
    $$PWD/ScanPathCylinderEdit.cpp \
    $$PWD/ScanPathCylinder.cpp \
    $$PWD/ScanPathUtilities.cpp \
    $$PWD/ChannelListWidget.cpp \
    $$PWD/ChannelTreeWidget.cpp \
    $$PWD/ScanSettings.cpp \
    $$PWD/ScanPathRingSettings.cpp

STATECHARTS += \
    $$PWD/ScanTask.scxml
