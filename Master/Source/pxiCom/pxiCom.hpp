#ifndef PXICOM_HPP
#define PXICOM_HPP

#include <QApplication>
#include <QThread>

#define DECLARE_ENUM(E_NAME) \
struct E_NAME \
{ \
public: \
    E_NAME(int nValue = 0) : _enumVal(static_cast<__Enum>(nValue)) \
    { \
    } \
    E_NAME& operator=(int nValue) \
    { \
        this->_enumVal = static_cast<__Enum>(nValue); \
        return *this; \
    } \
    operator int() const \
    { \
        return this->_enumVal; \
    } \
\
    enum __Enum {

#define END_ENUM() \
    }; \
\
private: \
    __Enum _enumVal; \
};
/*
DECLARE_ENUM(AXIS_e)
    X = 0,
    Y,
    Z,
    A,
    B,
    W,
    R
END_ENUM()
*/

//#define ARS_PXICOM
#define ARS_CHECK_MAIN_THREAD   Q_ASSERT_X(qApp->thread() == QThread::currentThread(), __FILE__, "Attempt to call outside of main thread");
#define ARS_CHECK_THREAD        Q_ASSERT_X(thread() == QThread::currentThread(), __FILE__, "Attempt to call slot function on different thread");

#define ARS_CHECK(pointer)                                      if ((pointer) == NULL) {return;}
#define ARS_CHECK_RETURN(pointer, returnValue)                  if ((pointer) == NULL) {return returnValue;}
#define ARS_CHECK_WARNING(pointer, errMsg)                      if ((pointer) == NULL) {qWarning(errMsg); return;}
#define ARS_CHECK_WARNING_RETURN(pointer, errMsg, value)  if ((pointer) == NULL) {qWarning(errMsg); return value;}
#define ARS_CHECK_BOUND(min, value, max)                        if ((value) < (min) || (value) > (max)) {return;}
#define ARS_CHECK_BOUND_RETURN(min, value, max, returnValue)    if ((value) < (min) || (value) > (max)) {return returnValue;}
#define ARS_CHECK_BOUND_WARNING(min, value, max, errMsg)        if ((value) < (min) || (value) > (max)) {qWarning(errMsg); return;}

#endif // PXICOM_HPP
