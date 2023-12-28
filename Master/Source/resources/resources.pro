TEMPLATE = aux

CONFIG -= resources

RESOURCES = \
    icons/Fatcow.qrc

qtPrepareTool(QMAKE_RCC, rcc, _DEP)

rcc_bin.input = RESOURCES
rcc_bin.name = RCC ${QMAKE_FILE_IN}
rcc_bin.depend_command = $$QMAKE_RCC_DEP -list $$QMAKE_RESOURCE_FLAGS ${QMAKE_FILE_IN}
rcc_bin.CONFIG += add_inputs_as_makefile_deps
rcc_bin.output = $$DS_RESOURCE_PATH/${QMAKE_FILE_BASE}
rcc_bin.commands = $$QMAKE_RCC $$QMAKE_RESOURCE_FLAGS --binary ${QMAKE_FILE_IN} -o ${QMAKE_FILE_OUT}

rcc_bin.depends += $$QMAKE_RCC_EXE
silent:rcc_bin.commands = @echo rcc ${QMAKE_FILE_IN} && $$rcc_bin.commands
else:rcc_bin.commands ~= s/&&/$$escape_expand(\\n\\t)/g
QMAKE_EXTRA_COMPILERS += rcc_bin
