include(../external.pri)

requires(exists($$DS_OPEN_CASCADE_ROOT))

win32 {
    copy_occt.path = $$DS_BIN_PATH
    for(module, OCCT_MODULES) {
        copy_occt.files += $$OCCT_PLATFORM/bin/$${module}.dll
    }
    copy_occt.files += $$DS_OPEN_CASCADE_ROOT/freetype-2.5.5-vc10-64/bin/freetype.dll
    copy_occt.files += $$DS_OPEN_CASCADE_ROOT/freeimage-3.17.0-vc10-64/bin/FreeImage.dll
    DS_TBB_PLATFORM = $$DS_OPEN_CASCADE_ROOT/tbb42_20140416oss/bin/intel64/vc10
    copy_occt.files += $$DS_TBB_PLATFORM/tbb.dll $$DS_TBB_PLATFORM/tbbmalloc.dll
} else {
    error(OpenCV not available for current platform.)
}

COPIES += copy_occt
