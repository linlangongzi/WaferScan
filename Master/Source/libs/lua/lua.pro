QT -= core gui

CONFIG -= qt

include(../library.pri)

include(lua.pri)

CONFIG -= hide_symbols c++14
CONFIG += use_c_linker
