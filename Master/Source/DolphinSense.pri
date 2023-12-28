
CONFIG += c++14 skip_target_version_ext

INCLUDEPATH += \
    $$PWD/libs \
    $$PWD/externals \
    $$DS_BUILD_PATH

VERSION = $$DS_VERSION

make_installer: CONFIG += no_debug_info

# Requires some library. So we add library root path
!isEmpty(DS_LIB_DEPENDS) {
    LIBS *= -L$$DS_LIBRARY_PATH
}

# Requires some plugin. So we add plugin root path
!isEmpty(DS_PLUGIN_DEPENDS) {
    LIBS *= -L$$DS_PLUGIN_PATH  # plugin path from output directory
}

for(dir, DS_SRC_PLUGIN_DIRS) {
    INCLUDEPATH += $$dir
}

# recursively resolve plugin deps
done_plugins =
for(ever) {
    isEmpty(DS_PLUGIN_DEPENDS): \
        break()
    done_plugins += $$DS_PLUGIN_DEPENDS
    for(dep, DS_PLUGIN_DEPENDS) {
        dependencies_file =
        for(dir, DS_SRC_PLUGIN_DIRS) {
            exists($$dir/$$dep/$${dep}_dependencies.pri) {
                dependencies_file = $$dir/$$dep/$${dep}_dependencies.pri
                break()
            }
        }
        isEmpty(dependencies_file): \
            error("DS: Plugin dependency $$dep not found for $$basename(_PRO_FILE_)")
        include($$dependencies_file)
        LIBS += -l$$qtLibraryName($$DS_PLUGIN_NAME)
    }
    DS_PLUGIN_DEPENDS = $$unique(DS_PLUGIN_DEPENDS)
    DS_PLUGIN_DEPENDS -= $$unique(done_plugins)
}

# recursively resolve library deps
done_libs =
for(ever) {
    isEmpty(DS_LIB_DEPENDS): \
        break()
    done_libs += $$DS_LIB_DEPENDS
    for(dep, DS_LIB_DEPENDS) {
        !include($$PWD/libs/$$dep/$${dep}_dependencies.pri) {
            error("DS: Library dependency $${dep} not found for $$basename(_PRO_FILE_)")
        }
        LIBS += -l$$qtLibraryName($$DS_LIB_NAME)
    }
    DS_LIB_DEPENDS = $$unique(DS_LIB_DEPENDS)
    DS_LIB_DEPENDS -= $$unique(done_libs)
}

for(dep, DS_EXTERNAL_DEPENDS) {
    include($$PWD/externals/$$dep/$${dep}_dependencies.pri)
}
