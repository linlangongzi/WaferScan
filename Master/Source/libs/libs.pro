TEMPLATE = subdirs

SUBDIRS += \
    utils \
    extensionsystem \
    lua \
    qtxlsx \
    allrising \
#    qdcm

# Read library dependency list, and configure "depends" attribute.
#   For example: extensionsystem.depends = utils
#   Then extensionsystem library will be compiled after utils.
for(part, SUBDIRS) {
    dep = $$fromfile($${part}/$${part}_dependencies.pri, DS_LIB_DEPENDS)
    $${part}.depends = $$dep
}
