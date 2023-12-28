QT += network

SOURCES += \
    $$PWD/TrioDriver.cpp \
    $$PWD/TrioAllocator.cpp \
    $$PWD/TrioQueue.cpp \
    $$PWD/TrioWarden.cpp \
    $$PWD/MotionPath.cpp \
    $$PWD/MotionAxisList.cpp \
    $$PWD/MotionCondition.cpp \
    $$PWD/MotionElement.cpp \
    $$PWD/MotionPose.cpp \
    $$PWD/RotationInfo.cpp \
    $$PWD/ChannelList.cpp \
    $$PWD/TrioStream.cpp \
    $$PWD/AxisControl.cpp \
    $$PWD/DialogAxisConfig.cpp \
    $$PWD/PanelAxisControl.cpp \
    $$PWD/PanelMotionConsole.cpp \
    $$PWD/Axis.cpp \
    $$PWD/AxisKinematics.cpp \
    $$PWD/EndPoint.cpp \
    $$PWD/MotionManager.cpp \
    $$PWD/DialogNamedPosition.cpp \
    $$PWD/DialogNamedPositionList.cpp \
    $$PWD/NamedPosition.cpp \
    $$PWD/ControllerInfo.cpp \
    $$PWD/DialogSafetyBarrierOptions.cpp \
    $$PWD/SafetyBarrier.cpp \
    $$PWD/SafetyBarrierOptions.cpp \
    $$PWD/WizardDatum.cpp \
    $$PWD/AxisModel.cpp \
    $$PWD/MotionOptions.cpp \
    $$PWD/DialogEndPointOptions.cpp \
    $$PWD/AutoScaleLabel.cpp \
    $$PWD/MotionPlugin.cpp \
    $$PWD/EndPointSelector.cpp

HEADERS += \
    $$PWD/TrioDriver.h \
    $$PWD/TrioAllocator.h \
    $$PWD/TrioQueue.h \
    $$PWD/TrioWarden.h \
    $$PWD/MotionPath.h \
    $$PWD/MotionAxisList.h \
    $$PWD/MotionCondition.h \
    $$PWD/MotionElement.h \
    $$PWD/MotionPose.h \
    $$PWD/RotationInfo.h \
    $$PWD/ChannelList.h \
    $$PWD/TrioStream.h \
    $$PWD/AxisControl.h \
    $$PWD/DialogAxisConfig.h \
    $$PWD/PanelAxisControl.h \
    $$PWD/PanelMotionConsole.h \
    $$PWD/Axis.h \
    $$PWD/AxisKinematics.h \
    $$PWD/EndPoint.h \
    $$PWD/MotionManager.h \
    $$PWD/DialogNamedPosition.h \
    $$PWD/DialogNamedPositionList.h \
    $$PWD/NamedPosition.h \
    $$PWD/AxisConfig.h \
    $$PWD/ControllerInfo.h \
    $$PWD/DialogSafetyBarrierOptions.h \
    $$PWD/SafetyBarrier.h \
    $$PWD/SafetyBarrierOptions.h \
    $$PWD/WizardDatum.h \
    $$PWD/AxisModel.h \
    $$PWD/MotionOptions.h \
    $$PWD/DialogEndPointOptions.h \
    $$PWD/AutoScaleLabel.h \
    $$PWD/MotionPlugin.h \
    $$PWD/EndPointSelector.h

RESOURCES += $$PWD/TrioPrograms.qrc

DISTFILES += \
    $$PWD/Motion.json.in

FORMS += \
    $$PWD/AxisControl.ui \
    $$PWD/DialogAxisConfig.ui \
    $$PWD/PanelAxisControl.ui \
    $$PWD/PanelMotionConsole.ui \
    $$PWD/DialogNamedPosition.ui \
    $$PWD/DialogNamedPositionList.ui \
    $$PWD/DialogSafetyBarrierOptions.ui \
    $$PWD/WizardDatum.ui \
    $$PWD/MotionOptions.ui \
    $$PWD/DialogEndPointOptions.ui

DEFINES += DS_MOTION_STATIC_LIB
