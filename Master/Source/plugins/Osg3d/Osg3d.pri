!contains(DEFINES, ARS_LIB_OSG): return()

QT += concurrent

HEADERS += \
    $$PWD/OsgContainer.h \
    $$PWD/Joint.h \
    $$PWD/Machine.h \
    $$PWD/PanelMachine.h \
    $$PWD/BSpline.h \
    $$PWD/BSplineCurve.h \
    $$PWD/BSplineSurface.h \
    $$PWD/MyMath.h \
    $$PWD/PathPlanning.h \
    $$PWD/Part.h \
    $$PWD/OriginIndicator.h \
    $$PWD/OsgManipulator.h \
    $$PWD/Item3d.h \
    $$PWD/PointMt.h \
    $$PWD/SurfaceMt.h \
    $$PWD/NurbsSurface.h \
    $$PWD/MachinePart.h \
    $$PWD/InverseSolver.h \
    $$PWD/CScanSurface.h \
    $$PWD/PathPlanningWizard.h \
    $$PWD/qtosgUtils.h \
    $$PWD/SurfaceContouring.h \
    $$PWD/SurfacePrediction.h \
    $$PWD/TriangulatedSurface.h

SOURCES += \
    $$PWD/OsgContainer.cpp \
    $$PWD/Joint.cpp \
    $$PWD/Machine.cpp \
    $$PWD/PanelMachine.cpp \
    $$PWD/BSpline.cpp \
    $$PWD/BSplineCurve.cpp \
    $$PWD/BSplineSurface.cpp \
    $$PWD/MyMath.cpp \
    $$PWD/PathPlanning.cpp \
    $$PWD/Part.cpp \
    $$PWD/OriginIndicator.cpp \
    $$PWD/OsgManipulator.cpp \
    $$PWD/Item3d.cpp \
    $$PWD/PointMt.cpp \
    $$PWD/SurfaceMt.cpp \
    $$PWD/NurbsSurface.cpp \
    $$PWD/MachinePart.cpp \
    $$PWD/InverseSolver.cpp \
    $$PWD/CScanSurface.cpp \
    $$PWD/PathPlanningWizard.cpp \
    $$PWD/qtosgUtils.cpp \
    $$PWD/SurfaceContouring.cpp \
    $$PWD/SurfacePrediction.cpp \
    $$PWD/TriangulatedSurface.cpp

FORMS += \
    $$PWD/PanelMachine.ui \
    $$PWD/DialogPartConfig.ui \
    $$PWD/PathPlanningWizard.ui \
    $$PWD/SurfaceContouring.ui \
    $$PWD/SurfacePrediction.ui

LIBS += -lopengl32 -lglu32

DEFINES += ARS_MODULE_3D
