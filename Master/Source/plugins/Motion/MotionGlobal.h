#pragma once

#include <qglobal.h>

#if defined(MOTION_LIBRARY)
#  define MOTION_EXPORT Q_DECL_EXPORT
#elif defined(DS_MOTION_STATIC_LIB)
#  define MOTION_EXPORT
#else
#  define MOTION_EXPORT Q_DECL_IMPORT
#endif
