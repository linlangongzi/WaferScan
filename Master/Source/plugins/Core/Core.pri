QT += sql

RESOURCES += \
    $$PWD/Icons.qrc \

FORMS += \
    $$PWD/DialogAbout.ui \
    $$PWD/DialogCredits.ui \
    $$PWD/DialogLicenseList.ui \
    $$PWD/LicenseEditor.ui \
    $$PWD/MainWindow.ui \
    $$PWD/GeneralSettings.ui

HEADERS += \
    $$PWD/CoreGlobal.h \
    $$PWD/DialogAbout.h \
    $$PWD/LicenseDevice.h \
    $$PWD/LicenseDriver.h \
    $$PWD/LicenseEditor.h \
    $$PWD/LicenseManager.h \
    $$PWD/DocumentManager.h \
    $$PWD/DocumentInfo.h \
    $$PWD/IDocument.h \
    $$PWD/IDocumentPart.h \
    $$PWD/ICore.h \
    $$PWD/Id.h \
    $$PWD/actionmanager/actioncontainer.h \
    $$PWD/actionmanager/actioncontainer_p.h \
    $$PWD/actionmanager/actionmanager.h \
    $$PWD/actionmanager/actionmanager_p.h \
    $$PWD/actionmanager/command.h \
    $$PWD/actionmanager/command_p.h \
    $$PWD/actionmanager/commandbutton.h \
    $$PWD/actionmanager/commandmappings.h \
    $$PWD/IOptionsPage.h \
    $$PWD/MainWindow.h \
    $$PWD/FileOperateManager.h \
    $$PWD/CorePlugin.h \
    $$PWD/dialogs/shortcutsettings.h \
    $$PWD/GeneralSettings.h \
    $$PWD/SettingsDialog.h \
    $$PWD/Platform.h \
    $$PWD/PluginDialog.h

SOURCES += \
    $$PWD/DialogAbout.cpp \
    $$PWD/LicenseDevice.cpp \
    $$PWD/LicenseEditor.cpp \
    $$PWD/LicenseManager.cpp \
    $$PWD/DocumentManager.cpp \
    $$PWD/IDocument.cpp \
    $$PWD/IDocumentPart.cpp \
    $$PWD/ICore.cpp \
    $$PWD/Id.cpp \
    $$PWD/actionmanager/actioncontainer.cpp \
    $$PWD/actionmanager/actionmanager.cpp \
    $$PWD/actionmanager/command.cpp \
    $$PWD/actionmanager/commandbutton.cpp \
    $$PWD/actionmanager/commandmappings.cpp \
    $$PWD/IOptionsPage.cpp \
    $$PWD/MainWindow.cpp \
    $$PWD/FileOperateManager.cpp \
    $$PWD/CorePlugin.cpp \
    $$PWD/dialogs/shortcutsettings.cpp \
    $$PWD/GeneralSettings.cpp \
    $$PWD/SettingsDialog.cpp \
    $$PWD/Platform.cpp \
    $$PWD/PluginDialog.cpp

DEFINES += DS_CORE_STATIC_LIB

ARS_LIB_ELITE_E {
    HEADERS += $$PWD/LicenseDriverEliteE.h
    SOURCES += $$PWD/LicenseDriverEliteE.cpp
}

!contains(DEFINES, ARS_FINAL_RELEASE) {
    HEADERS += $$PWD/LicenseDriverDevelop.h
    SOURCES += $$PWD/LicenseDriverDevelop.cpp
}

win32 {
    HEADERS += $$PWD/LicenseDriverLdap.h
    SOURCES += $$PWD/LicenseDriverLdap.cpp
    LIBS += -lWldap32
}
