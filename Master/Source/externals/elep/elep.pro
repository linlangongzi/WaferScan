include(../external.pri)

ARS_LIB_ELITE_E {
    copy_elep.files = $$ELEP_PLATFORM/elep.dll
    copy_elep.path = $$DS_LIBRARY_DLL_PATH

    COPIES += copy_elep
}
