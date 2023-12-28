DEFINES += MOTION_LIBRARY

QT += widgets network

include(../plugin.pri)

HEADERS += \
    MotionPlugin.h \
    MotionGlobal.h \
    MotionOptions.h \
    MotionConstants.h \
    MotionCommand.h \
    MotionPath.h \
    MotionAxisList.h \
    MotionCondition.h \
    MotionElement.h \
    MotionPose.h \
    RotationInfo.h \
    ChannelList.h \
    TrioStream.h \
    AxisControl.h \
    DialogAxisConfig.h \
    PanelAxisControl.h \
    PanelMotionConsole.h \
    MotionManager.h \
    Axis.h \
    EndPoint.h \
    AxisKinematics.h \
    SafetyBarrier.h \
    WizardDatum.h \
    DialogSafetyBarrierOptions.h \
    DialogGlobalPositions.h \
    TrioDriver.h \
    TrioWarden.h \
    TrioQueue.h \
    TrioAllocator.h \
    AxisConfig.h \
    SafetyBarrierOptions.h \
    ControllerInfo.h \
    DialogEndPointOptions.h \
    AxisModel.h \
    DialogNamedPositionList.h \
    DialogNamedPosition.h \
    NamedPosition.h

SOURCES += \
    MotionPlugin.cpp \
    MotionOptions.cpp \
    MotionCommand.cpp \
    MotionPath.cpp \
    MotionAxisList.cpp \
    MotionCondition.cpp \
    MotionElement.cpp \
    MotionPose.cpp \
    RotationInfo.cpp \
    ChannelList.cpp \
    TrioStream.cpp \
    AxisControl.cpp \
    DialogAxisConfig.cpp \
    PanelAxisControl.cpp \
    PanelMotionConsole.cpp \
    MotionManager.cpp \
    Axis.cpp \
    EndPoint.cpp \
    AxisKinematics.cpp \
    SafetyBarrier.cpp \
    WizardDatum.cpp \
    DialogSafetyBarrierOptions.cpp \
    DialogGlobalPositions.cpp \
    TrioDriver.cpp \
    TrioWarden.cpp \
    TrioQueue.cpp \
    TrioAllocator.cpp \
    SafetyBarrierOptions.cpp \
    ControllerInfo.cpp \
    DialogEndPointOptions.cpp \
    AxisModel.cpp \
    DialogNamedPositionList.cpp \
    DialogNamedPosition.cpp \
    NamedPosition.cpp

FORMS += \
    MotionOptions.ui \
    AxisControl.ui \
    DialogAxisConfig.ui \
    PanelAxisControl.ui \
    PanelMotionConsole.ui \
    WizardDatum.ui \
    DialogSafetyBarrierOptions.ui \
    DialogGlobalPositions.ui \
    DialogEndPointOptions.ui \
    DialogNamedPositionList.ui \
    DialogNamedPosition.ui

RESOURCES += \
    TrioPrograms.qrc

STATECHARTS += \
    TrioDriver.scxml
