win32: dll {
    DEFINES += LUA_BUILD_AS_DLL
} else {
#    DEFINES += LUA_STATIC_LIB
}

mac {
    DEFINES += LUA_USE_MACOSX
}

linux {
    DEFINES += LUA_USE_LINUX
    LIBS += -ldl
}

msvc {
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

LUA_SRC = $$DS_EXTERNAL_LIBRARY_ROOT/lua-5.2.4/src

HEADERS += \
    $$LUA_SRC/lapi.h \
    $$LUA_SRC/lauxlib.h \
    $$LUA_SRC/lcode.h \
    $$LUA_SRC/lctype.h \
    $$LUA_SRC/ldebug.h \
    $$LUA_SRC/ldo.h \
    $$LUA_SRC/lfunc.h \
    $$LUA_SRC/lgc.h \
    $$LUA_SRC/llex.h \
    $$LUA_SRC/llimits.h \
    $$LUA_SRC/lmem.h \
    $$LUA_SRC/lobject.h \
    $$LUA_SRC/lopcodes.h \
    $$LUA_SRC/lparser.h \
    $$LUA_SRC/lstate.h \
    $$LUA_SRC/lstring.h \
    $$LUA_SRC/ltable.h \
    $$LUA_SRC/ltm.h \
    $$LUA_SRC/lua.h \
    $$LUA_SRC/luaconf.h \
    $$LUA_SRC/lualib.h \
    $$LUA_SRC/lundump.h \
    $$LUA_SRC/lvm.h \
    $$LUA_SRC/lzio.h

SOURCES += \
    $$LUA_SRC/lapi.c \
    $$LUA_SRC/lauxlib.c \
    $$LUA_SRC/lbaselib.c \
    $$LUA_SRC/lbitlib.c \
    $$LUA_SRC/lcode.c \
    $$LUA_SRC/lcorolib.c \
    $$LUA_SRC/lctype.c \
    $$LUA_SRC/ldblib.c \
    $$LUA_SRC/ldebug.c \
    $$LUA_SRC/ldo.c \
    $$LUA_SRC/ldump.c \
    $$LUA_SRC/lfunc.c \
    $$LUA_SRC/lgc.c \
    $$LUA_SRC/linit.c \
    $$LUA_SRC/liolib.c \
    $$LUA_SRC/llex.c \
    $$LUA_SRC/lmathlib.c \
    $$LUA_SRC/lmem.c \
    $$LUA_SRC/loadlib.c \
    $$LUA_SRC/lobject.c \
    $$LUA_SRC/lopcodes.c \
    $$LUA_SRC/loslib.c \
    $$LUA_SRC/lparser.c \
    $$LUA_SRC/lstate.c \
    $$LUA_SRC/lstring.c \
    $$LUA_SRC/lstrlib.c \
    $$LUA_SRC/ltable.c \
    $$LUA_SRC/ltablib.c \
    $$LUA_SRC/ltm.c \
    $$LUA_SRC/lundump.c \
    $$LUA_SRC/lvm.c \
    $$LUA_SRC/lzio.c
