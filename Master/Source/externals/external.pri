depfile = $$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri)
include($$depfile)

isEmpty(DS_LIB_NAME): error("DS_LIB_NAME not defined for $$_PRO_FILE_. It should be defined in $$depfile")

TEMPLATE = aux

CONFIG += file_copies
