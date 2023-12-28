#pragma once

#include <qglobal.h>

#if defined(WAVEFORM_LIBRARY)
#  define WAVEFORM_EXPORT Q_DECL_EXPORT
#elif defined(DS_WAVEFORM_STATIC_LIB)
#  define WAVEFORM_EXPORT
#else
#  define WAVEFORM_EXPORT Q_DECL_IMPORT
#endif
