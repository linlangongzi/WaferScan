#pragma once

#include <qglobal.h>
#include <QLoggingCategory>

#if defined(ACQUISITION_LIBRARY)
#  define ACQUISITION_EXPORT Q_DECL_EXPORT
#elif defined(DS_ACQUISITION_STATIC_LIB)
#  define ACQUISITION_EXPORT
#else
#  define ACQUISITION_EXPORT Q_DECL_IMPORT
#endif

#define ARS_FILE_EXT_DATA ".ard"
#define ARS_FILE_EXT_PARAMETER ".arp"
