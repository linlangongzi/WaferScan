#pragma once

#include <qglobal.h>

#if defined(ULTRASONIC_LIBRARY)
#  define ULTRASONIC_EXPORT Q_DECL_EXPORT
#elif defined(DS_ULTRASONIC_STATIC_LIB)
#  define ULTRASONIC_EXPORT
#else
#  define ULTRASONIC_EXPORT Q_DECL_IMPORT
#endif
