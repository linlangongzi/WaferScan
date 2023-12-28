include(../external.pri)

requires(exists($$OPENCV_PLATFORM) | packagesExist(opencv))

win32 {
    copy_opencv.path = $$DS_BIN_PATH
    for(module, OPENCV_MODULES) {
        copy_opencv.files += $$OPENCV_PLATFORM/bin/opencv_$${module}$${OPENCV_VERSION}$${OPENCV_DEBUG_SUFFIX}.dll
    }
} else: osx {
    copy_opencv.path = $$DS_LIBRARY_DLL_PATH
    for(module, OPENCV_MODULES) {
        copy_opencv.files += $$OPENCV_PLATFORM/lib/libopencv_$${module}$${OPENCV_VERSION}$${OPENCV_DEBUG_SUFFIX}.dylib
    }
}

COPIES += copy_opencv
