HEADERS += \
    $$PWD/allrising_global.h \
    $$PWD/ColorButton.h \
    $$PWD/ComboBoxMulti.h \
    $$PWD/ConnectionHelper.h \
    $$PWD/DockWidget.h \
    $$PWD/GeneralDelegate.h \
    $$PWD/HotkeyListener.h \
    $$PWD/QtExtend.h \
    $$PWD/Range.h \
    $$PWD/ThrottleUpdate.h \
    $$PWD/Transform1D.h \
    $$PWD/Util.h \
    $$PWD/MetaType/DialogGadgetEditor.h \
    $$PWD/MetaType/EnumComboBox.h \
    $$PWD/MetaType/GadgetListEditor.h \
    $$PWD/MetaType/GadgetTreeItem.h \
    $$PWD/MetaType/JsonConverter.h \
    $$PWD/MetaType/MetaType.h \
    $$PWD/MetaType/VectorEditor.h

SOURCES += \
    $$PWD/ColorButton.cpp \
    $$PWD/ComboBoxMulti.cpp \
    $$PWD/DockWidget.cpp \
    $$PWD/GeneralDelegate.cpp \
    $$PWD/HotkeyListener.cpp \
    $$PWD/QtExtend.cpp \
    $$PWD/Range.cpp \
    $$PWD/ThrottleUpdate.cpp \
    $$PWD/Transform1D.cpp \
    $$PWD/Util.cpp \
    $$PWD/MetaType/DialogGadgetEditor.cpp \
    $$PWD/MetaType/EnumComboBox.cpp \
    $$PWD/MetaType/GadgetListEditor.cpp \
    $$PWD/MetaType/GadgetTreeItem.cpp \
    $$PWD/MetaType/JsonConverter.cpp \
    $$PWD/MetaType/MetaType.cpp \
    $$PWD/MetaType/VectorEditor.cpp

FORMS += \
    $$PWD/MetaType/DialogGadgetEditor.ui \
    $$PWD/MetaType/GadgetListEditor.ui \
    $$PWD/MetaType/VectorEditor.ui

DEFINES += ALLRISING_UTILS_STATIC_LIB

# Avoid windows.h defines macro "min" and "max"
win32: DEFINES += NOMINMAX
