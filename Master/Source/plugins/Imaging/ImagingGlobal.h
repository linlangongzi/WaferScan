#pragma once

#include <qglobal.h>

#if defined(IMAGING_LIBRARY)
#  define IMAGING_EXPORT Q_DECL_EXPORT
#elif defined(DS_IMAGING_STATIC_LIB)
#  define IMAGING_EXPORT
#else
#  define IMAGING_EXPORT Q_DECL_IMPORT
#endif

#define ARS_FILE_EXT_DATA ".ard"
#define ARS_FILE_EXT_PARAMETER ".arp"
