DEFINES += CORE_LIBRARY

QT += gui widgets network sql

include(../plugin.pri)

HEADERS += \
    CoreGlobal.h \
    DialogAbout.h \
    LicenseDevice.h \
    LicenseDriver.h \
    LicenseEditor.h \
    LicenseManager.h \
    ICore.h \
    CorePlugin.h \
    PluginDialog.h \
    MainWindow.h \
    Id.h \
    IOptionsPage.h \
    SettingsDialog.h \
    actionmanager/actioncontainer.h \
    actionmanager/actioncontainer_p.h \
    actionmanager/actionmanager.h \
    actionmanager/actionmanager_p.h \
    actionmanager/command.h \
    actionmanager/command_p.h \
    actionmanager/commandbutton.h \
    actionmanager/commandmappings.h \
    dialogs/shortcutsettings.h \
    IContext.h \
    CoreConstants.h \
    GeneralSettings.h \
    DocumentManager.h \
    DocumentInfo.h \
    IDocument.h \
    IDocumentPart.h \
    FileOperateManager.h \
    Platform.h

SOURCES += \
    DialogAbout.cpp \
    LicenseDevice.cpp \
    LicenseEditor.cpp \
    LicenseManager.cpp \
    ICore.cpp \
    CorePlugin.cpp \
    PluginDialog.cpp \
    MainWindow.cpp \
    Id.cpp \
    IOptionsPage.cpp \
    SettingsDialog.cpp \
    actionmanager/actioncontainer.cpp \
    actionmanager/actionmanager.cpp \
    actionmanager/command.cpp \
    actionmanager/commandbutton.cpp \
    actionmanager/commandmappings.cpp \
    dialogs/shortcutsettings.cpp \
    IContext.cpp \
    GeneralSettings.cpp \
    DocumentManager.cpp \
    IDocument.cpp \
    IDocumentPart.cpp \
    FileOperateManager.cpp \
    Platform.cpp

FORMS += \
    MainWindow.ui \
    GeneralSettings.ui \
    DialogAbout.ui \
    DialogCredits.ui \
    DialogLicenseList.ui \
    LicenseEditor.ui

RESOURCES += \
    Icons.qrc \
    ../../resources/Resources.qrc

ARS_LIB_ELITE_E {
    HEADERS += LicenseDriverEliteE.h
    SOURCES += LicenseDriverEliteE.cpp
}

!contains(DEFINES, ARS_FINAL_RELEASE) {
    HEADERS += LicenseDriverDevelop.h
    SOURCES += LicenseDriverDevelop.cpp
}

win32 {
    HEADERS += LicenseDriverLdap.h
    SOURCES += LicenseDriverLdap.cpp
    LIBS += -lWldap32
}
