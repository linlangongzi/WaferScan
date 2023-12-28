#pragma once

#include <qglobal.h>

#if defined(TEMPERATURE_LIBRARY)
#  define TEMPERATURE_EXPORT Q_DECL_EXPORT
#else
#  define TEMPERATURE_EXPORT Q_DECL_IMPORT
#endif
