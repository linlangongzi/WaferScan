#pragma once

#ifdef DCM_IMAGE_EXPORTS
    #define DCM_IMAGE_API Q_DECL_EXPORT
#else
    #define DCM_IMAGE_API Q_DECL_IMPORT
#endif
