TEMPLATE = aux

TRANSLATION_FILES += \
    WaferScan_zh_CN.ts \
    WaferScan_en.ts

qtPrepareTool(LRELEASE, lrelease)
updateqm.input = TRANSLATION_FILES
updateqm.output = $$DS_BIN_PATH/translations/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
silent:updateqm.commands = @echo lrelease ${QMAKE_FILE_IN} && $$updateqm.commands
updateqm.depends = $$LRELEASE_EXE
updateqm.name = LRELEASE ${QMAKE_FILE_IN}
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm
