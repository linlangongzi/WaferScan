#pragma once

#include <qglobal.h>

#if defined(PLOT_LIBRARY)
#  define PLOT_EXPORT Q_DECL_EXPORT
#elif defined(DS_PLOT_STATIC_LIB)
#  define PLOT_EXPORT
#else
#  define PLOT_EXPORT Q_DECL_IMPORT
#endif
