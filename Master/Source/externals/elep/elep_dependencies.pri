DS_LIB_NAME = elep

win32 {
    ELEP_ROOT = $$DS_EXTERNAL_LIBRARY_ROOT/elite-e
    equals(QMAKE_TARGET.arch, "x86_64") {
        ELEP_PLATFORM = $$ELEP_ROOT/x64-vc12
    } else {
        ELEP_PLATFORM = $$ELEP_ROOT/x86-vc12
    }

    INCLUDEPATH += $$ELEP_ROOT
    win32-g++ {
        LIBS += $$ELEP_PLATFORM/elep.lib
    } else {
        LIBS += -L$$ELEP_PLATFORM -lelep
    }

    CONFIG += ARS_LIB_ELITE_E
}
