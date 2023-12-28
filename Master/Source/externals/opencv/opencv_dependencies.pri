DS_LIB_NAME = opencv

packagesExist(opencv) {
    PKGCONFIG += opencv
    CONFIG += link_pkgconfig
} else {

    !equals(QMAKE_TARGET.arch, "x86_64") {
        OPENCV_MODULES = highgui imgproc core
        OPENCV_VERSION = 249
        OPENCV_ROOT = $$DS_EXTERNAL_LIBRARY_ROOT/opencv
        OPENCV_PLATFORM = $$OPENCV_ROOT/x86-vc12
    } else: exists($$DS_EXTERNAL_LIBRARY_ROOT/opencv3) {
        OPENCV_MODULES = world
        OPENCV_VERSION = 320
        OPENCV_ROOT = $$DS_EXTERNAL_LIBRARY_ROOT/opencv3
        OPENCV_PLATFORM = $$OPENCV_ROOT/x64/vc14
    } else {
        OPENCV_MODULES = highgui imgproc core
        OPENCV_VERSION = 249
        OPENCV_ROOT = $$DS_EXTERNAL_LIBRARY_ROOT/opencv
        OPENCV_PLATFORM = $$OPENCV_ROOT/x64-vc12
    }

    osx {
        OPENCV_ROOT = $$clean_path($$DS_EXTERNAL_LIBRARY_ROOT/opencv)
        OPENCV_PLATFORM = $$clean_path($$DS_EXTERNAL_LIBRARY_ROOT/opencv/osx)
        OPENCV_VERSION =
    }
    INCLUDEPATH += $$OPENCV_ROOT/include
    LIBS += -L$$OPENCV_PLATFORM/lib
    #CONFIG(debug, debug|release): OPENCV_DEBUG_SUFFIX = d

    for(module, OPENCV_MODULES) {
        LIBS += -lopencv_$${module}$${OPENCV_VERSION}$${OPENCV_DEBUG_SUFFIX}
    }
}
