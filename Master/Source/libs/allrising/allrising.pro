include(../library.pri)

dll {
    DEFINES += ALLRISING_UTILS_LIB
} else {
    DEFINES += ALLRISING_UTILS_STATIC_LIB
}

QT += gui widgets

HEADERS += \
    ConnectionHelper.h \
    GeneralDelegate.h \
    HotkeyListener.h \
    MetaType/DialogGadgetEditor.h \
    MetaType/EnumComboBox.h \
    MetaType/GadgetListEditor.h \
    MetaType/GadgetTreeItem.h \
    MetaType/JsonConverter.h \
    MetaType/MetaType.h \
    MetaType/VectorEditor.h \
    QtExtend.h \
    Range.h \
    ThrottleUpdate.h \
    Transform1D.h \
    Util.h \
    allrising_global.h \
    ComboBoxMulti.h \
    DockWidget.h

SOURCES += \
    GeneralDelegate.cpp \
    HotkeyListener.cpp \
    MetaType/DialogGadgetEditor.cpp \
    MetaType/EnumComboBox.cpp \
    MetaType/GadgetListEditor.cpp \
    MetaType/GadgetTreeItem.cpp \
    MetaType/JsonConverter.cpp \
    MetaType/MetaType.cpp \
    MetaType/VectorEditor.cpp \
    QtExtend.cpp \
    Range.cpp \
    ThrottleUpdate.cpp \
    Transform1D.cpp \
    Util.cpp \
    ComboBoxMulti.cpp \
    DockWidget.cpp

FORMS += \
    MetaType/DialogGadgetEditor.ui \
    MetaType/GadgetListEditor.ui \
    MetaType/VectorEditor.ui
