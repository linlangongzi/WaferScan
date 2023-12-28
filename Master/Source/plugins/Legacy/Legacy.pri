QT += multimediawidgets

HEADERS += \
    $$PWD/LegacyPlugin.h \
    $$PWD/CameraView.h \
    $$PWD/FileTransform.h \
    $$PWD/ReadFileModule.h \
    $$PWD/WriteFileModule.h \
    $$PWD/MatlabEngine.h \
    $$PWD/WizardArdFileMaintain.h

SOURCES += \
    $$PWD/LegacyPlugin.cpp \
    $$PWD/CameraView.cpp \
    $$PWD/FileTransform.cpp \
    $$PWD/ReadFileModule.cpp \
    $$PWD/WriteFileModule.cpp \
    $$PWD/MatlabEngine.cpp \
    $$PWD/WizardArdFileMaintain.cpp

FORMS += \
    $$PWD/CameraView.ui \
    $$PWD/FileTransform.ui \
    $$PWD/WizardArdFileMaintain.ui
