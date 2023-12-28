TEMPLATE = subdirs

SUBDIRS += \
    Acquisition \
    Ars1000 \
    Ars3000 \
    Ars500 \
    Core \
    Gamepad \
    Imaging \
    Legacy \
    Mdo3000 \
    Motion \
    Opc \
    Pipe \
    Plot \
    Ultrasonic \
    Waveform \
#    Osg3d \
#    Diconde \
#    Temperature \
#    Wheels \
#    Visualization3d

SUBDIRS += $$DS_EXTRA_PLUGINS
SUBDIRS -= $$DS_DISABLED_PLUGINS

CONFIG(debug, debug|release): SUBDIRS += Helloworld

# Exclude plugins with unavailable EXTERNAL dependencies
DS_EXTERNALS_AVAILABLE = $$fromfile(../externals/externals.pro, SUBDIRS)
plugins_disabled =
for(part, SUBDIRS) {
    depends = $$fromfile($${part}/$${part}_dependencies.pri, DS_EXTERNAL_DEPENDS)
    for (dep, depends) {
        !contains(DS_EXTERNALS_AVAILABLE, $$dep) {
            infile($${part}/$${part}_dependencies.pri, DS_PLUGIN_OPTIONAL) {
                message("DS: Optional plugin $$part is disabled, since external dependency $$dep is not available.")
            } else {
                warning("DS: Plugin $$part is disabled, since external dependency $$dep is not available.")
            }
            SUBDIRS -= $$part
            plugins_disabled += $$part
            break()
        }
    }
}

# Exclude plugins with unavailable PLUGIN dependencies
for(ever) {
    isEmpty(plugins_disabled): \
        break()
    plugin_disabled = $$first(plugins_disabled)
    for(part, SUBDIRS) {
        depends = $$fromfile($${part}/$${part}_dependencies.pri, DS_PLUGIN_DEPENDS)
        contains(depends, $$plugin_disabled) {
            warning("DS: Plugin $$part is disabled, since plugin dependency $$plugin_disabled is not available.")
            SUBDIRS -= $$part
            plugins_disabled += $$part
        }
    }
    plugins_disabled -= $$plugin_disabled
}

# Read plugin dependency list, and configure "depends" attribute.
#   For example: Waveform.depends = Core Ultrasonic
#   Then Waveform plugin will be compiled after Core and Ultrasonic.
for(part, SUBDIRS) {
    dep = $$fromfile($${part}/$${part}_dependencies.pri, DS_PLUGIN_DEPENDS)
    $${part}.depends = $$dep
}
