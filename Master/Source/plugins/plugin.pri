# Plugin.pri initializes plugin build process.
# This file should be included in {pluginName}.pro

# Read *_dependencies.pri, which contains basic info about the plugin:
# DS_PLUGIN_NAME:       name of plugin (*.lib, *.dll)
# DS_PLUGIN_DEPENDS:    plugin dependencies
# DS_LIB_DEPENDS:       library dependencies (built from source)
# DS_EXTERNAL_DEPENDS:  external library dependencies (we got binary)
depfile = $$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri)
exists($$depfile) {
    include($$depfile)
}
isEmpty(DS_PLUGIN_NAME) {
    error("$$basename(depfile) does not define DS_PLUGIN_NAME.")
}

TEMPLATE = lib
CONFIG += plugin plugin_with_soname
TARGET = $$DS_PLUGIN_NAME       # DS_PLUGIN_NAME.dll

DLLDESTDIR = $$DS_PLUGIN_DLL_PATH
DESTDIR = $$DS_PLUGIN_PATH

TARGET = $$qtLibraryTargetName($$TARGET)

osx {
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/PlugIns/
}

isEmpty(PRECOMPILED_HEADER) {
    contains(QT, gui) {
        PRECOMPILED_HEADER += $$PWD/../shared/pch_gui.h
    } else {
        PRECOMPILED_HEADER += $$PWD/../shared/pch.h
    }
}

defineReplace(dependencyName) {
    dependencies_file =
    for(dir, DS_SRC_PLUGIN_DIRS) {
        exists($$dir/$$1/$${1}_dependencies.pri) {
            dependencies_file = $$dir/$$1/$${1}_dependencies.pri
            break()
        }
    }
    isEmpty(dependencies_file): \
        error("Plugin: Plugin dependency $$dep not found")
    include($$dependencies_file)
    return($$DS_PLUGIN_NAME)
}

# for substitution in the .json
dependencyList =
for(dep, DS_PLUGIN_DEPENDS) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$DS_VERSION\" }"
}
for(dep, DS_PLUGIN_RECOMMENDS) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$DS_VERSION\", \"Type\" : \"optional\" }"
}
for(dep, DS_TEST_DEPENDS) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$DS_VERSION\", \"Type\" : \"test\" }"
}
dependencyList = $$join(dependencyList, ",$$escape_expand(\\n)")

dependencyList = "\"Dependencies\" : [$$escape_expand(\\n)$$dependencyList$$escape_expand(\\n)    ]"

#message("$$DS_PLUGIN_NAME : $$dependencyList")
# Generate plugin json
PLUGINJSON = $$_PRO_FILE_PWD_/$${DS_PLUGIN_NAME}.json
PLUGINJSON_IN = $${PLUGINJSON}.in
exists($$PLUGINJSON_IN) {
    DISTFILES += $$PLUGINJSON_IN
    QMAKE_SUBSTITUTES += $$PLUGINJSON_IN
    PLUGINJSON = $$OUT_PWD/$${DS_PLUGIN_NAME}.json
} else {
    # need to support that for external plugins
    DISTFILES += $$PLUGINJSON
}

# All plugins inherit IPlugin, thus depends on ExtensionSystem.
DS_LIB_DEPENDS += extensionsystem
# DolphinSense.pri will resolve dependencies and set up include paths
include($$PWD/../DolphinSense.pri)

# Include output json
INCLUDEPATH += $$OUT_PWD
