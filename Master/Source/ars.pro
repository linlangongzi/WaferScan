TEMPLATE = subdirs

#CONFIG += ordered

SUBDIRS += \
    externals \
    libs \
    profiles \
    resources \
    translations \
    WaferScan.pro \
#    plugins

WaferScan.depends = libs
plugins.depends = libs

createInstaller {
    deploy.depends = $$SUBDIRS
    SUBDIRS += deploy
}

TRANSLATIONS = \
    translations/WaferScan_zh_CN.ts \
    translations/WaferScan_en.ts
