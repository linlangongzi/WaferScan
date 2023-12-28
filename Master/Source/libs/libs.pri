INCLUDEPATH += $$PWD

include($$PWD/allrising/allrising.pri)

include($$PWD/utils/utils.pri)

include($$PWD/lua/lua.pri)
INCLUDEPATH += $$DS_EXTERNAL_LIBRARY_ROOT/lua-5.2.4/src

ARS_QTXLSX_LIB = $$ARS_LIB_ROOT/QtXlsxWriter/src/xlsx
exists($$ARS_QTXLSX_LIB) {
    DEFINES += ARS_LIB_QTXLSX XLSX_NO_LIB
    include($$ARS_QTXLSX_LIB/qtxlsx.pri)
}

!contains(DEFINES, ARS_FINAL_RELEASE) {
    include($$ARS_LIB_INC_ROOT/osg.pri)
}
