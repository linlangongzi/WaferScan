QT += gamepad

include(../plugin.pri)

HEADERS += \
    $$PWD/GamepadPlugin.h \
    $$PWD/GamepadController.h \
    $$PWD/GamepadSettings.h \
    $$PWD/GamepadButtonSettings.h \
    $$PWD/GamepadAxisSettings.h \
    $$PWD/GamepadConstants.h

SOURCES += \
    $$PWD/GamepadPlugin.cpp \
    $$PWD/GamepadController.cpp \
    $$PWD/GamepadSettings.cpp \
    $$PWD/GamepadButtonSettings.cpp \
    $$PWD/GamepadAxisSettings.cpp

FORMS += \
    $$PWD/GamepadButtonSettings.ui \
    $$PWD/GamepadAxisSettings.ui \
    $$PWD/GamepadSettings.ui

DISTFILES += \
    $$PWD/Gamepad.json.in
