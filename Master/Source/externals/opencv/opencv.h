#include <QtGlobal>
#ifdef Q_CC_CLANG
#    pragma clang diagnostic push                                               /* 压制 clang 警告 */
#    pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
#ifdef Q_CC_MSVC
#   pragma warning(push)
#   pragma warning(disable: 4819)
#endif

#include <opencv2/opencv.hpp>

#ifdef Q_CC_MSVC
#   pragma warning(pop)
#endif
#ifdef Q_CC_CLANG
#    pragma clang diagnostic pop
#endif
