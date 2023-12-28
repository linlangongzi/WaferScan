#pragma once

#ifdef DCM_CORE_EXPORTS
#   define DCM_CORE_API Q_DECL_EXPORT
#else
#   define DCM_CORE_API Q_DECL_IMPORT
#endif
