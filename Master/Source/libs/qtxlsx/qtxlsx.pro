include(../library.pri)

CONFIG += build_xlsx_lib

DEFINES += QT_BUILD_XLSX_LIB

include($$DS_EXTERNAL_LIBRARY_ROOT/QtXlsxWriter/src/xlsx/qtxlsx.pri)
#VERSION = $$fromfile($$DS_EXTERNAL_LIBRARY_ROOT/QtXlsxWriter/.qmake.conf, MODULE_VERSION)
