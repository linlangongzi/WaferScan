/**************************************************************************************
* Copyright (c) 2008 Senselock Data Security Centre.
* All rights reserved.
*
* filename: elepopular.h
*
* brief: EliteE popular edition library interface declaration, return value and some constant definition.
* 
* history:
*   2008,07,9 created 1.0
***************************************************************************************/

#ifndef __ELEPOPULAR_H__
#define __ELEPOPULAR_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef int                 BOOL;
typedef void                *HANDLE;
  
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
  
#if defined(_WIN64)
    typedef __int64 LONG_PTR, *PLONG_PTR;
#else
  typedef long LONG_PTR, *PLONG_PTR;
#endif

typedef char                CHAR8_T;
typedef unsigned char       UCHAR8_T;
typedef unsigned short      USHORT16_T;
typedef unsigned long       ULONG32_T;
typedef unsigned int        UINT32_T;
typedef int                 INT32_T;
typedef long                LONG32_T;
typedef BOOL                BOOL_T;


#ifndef ELEPAPI
#if defined WIN32 || defined _WIN32
#define ELEPAPI __stdcall
#else
#define ELEPAPI
#endif
#endif


#ifndef FALSE
#define FALSE               0
#endif
  
#ifndef TRUE
#define TRUE                1
#endif
  
#ifndef IN
#define IN
#endif
  
#ifndef OUT
#define OUT
#endif

// 缓冲区长度
#define ELEP_RSA_PUB_KEY_LEN                   132       // RSA公钥长度
#define ELEP_LENGTH_SIGNATURE                  128       // 数字签名长度
#define ELEP_LENGTH_MEMORY_UPDATE_IN           2496      // 升级包输入长度
#define ELEP_LENGTH_MEMORY_UPDATE_OUT          3968      // 升级包输出长度

// 错误返回值
#define ELEP_SUCCESS                           0x80000000// 成功
#define ELEP_INVALID_PARAMETER                 0x80000001// 参数错误
#define ELEP_INSUFFICIENT_BUFFER               0x80000002// 缓冲区不够
#define ELEP_NOT_ENOUGH_MEMORY                 0x80000003// 内存不够
#define ELEP_INVALID_DEVICE_HANDLE             0x80000004// 设备句柄无效
#define ELEP_COMM_ERROR                        0x80000005// 通讯错误
                                                 
#define ELEP_INVALID_SHARE_MODE                0x80000006// 打开设备时，使用了无效的共享方式
#define ELEP_UNSUPPORTED_OS                    0x80000007// 精锐E不支持该操作系统
#define ELEP_ENUMERATE_DEVICE_FAILED           0x80000008// 列举设备失败
#define ELEP_NO_MORE_DEVICE                    0x80000009// 没有符合条件的设备

#define ELEP_INVALID_MODULE                    0x8000000a// 无效模块
                                                 
#define ELEP_NOT_RTL_DEVICE                    0x80000024// 不是时钟设备
#define ELEP_NOT_MASTER_DEVICE                 0x80000025// 不是升级控制锁
#define ELEP_NOT_USER_DEVICE                   0x80000026// 不是用户锁
#define ELEP_CLOSE_DEVICE_FAILED               0x80000067// 关闭设备失败
#define ELEP_VERIFY_SIGNATURE_ERROR            0x80000068// 验证签名失败 

#define ELEP_ERROR_HW_ERROR                    0x80010001// 硬件错误
#define ELEP_ERR_INVALID_PASSWORD              0x80010002// 有锁，但密码错
#define ELEP_ERR_OPT                           0x80010003// 一次写错误
#define ELEP_ERR_INVALID_ADDR_OR_SIZE          0x80010004// 读写地址或长度有误
#define ELEP_ERR_DECREASE                      0x80010005// 递减错
#define ELEP_ERR_NO_PRIVILEGE                  0x80010006// 无权限
#define ELEP_ERR_EXPIRE                        0x80010007// 升级包过期
#define ELEP_INVALID_UPDATE_PKG                0x80010008// 升级包无效
#define ELEP_ERR_INVALID_STATE                 0x80010009// 无效状态

// 打开指定开发商编号的第1把设备
// 如果开发商编号为0，则忽略开发商编号找到第1把设备
// 如果未找到设备，则返回ELEP_NO_MORE_DEVICE
// 如果找到了相应设备且PIN码正确，则返回值为ELEP_SUCCESS，pulHandle返回设备句柄
// 如果找到了相应设备但PIN码不正确，则返回ELEP_ERR_INVALID_PASSWORD，pulHandle返回设备句柄
// 如果PIN码为0，则不验证PIN码
ULONG32_T ELEPAPI ElepOpenFirst(
  IN    ULONG32_T               ulDevMajNumber,
  IN    ULONG32_T               ulDevMinNumber,
  IN    ULONG32_T               ulPin1,
  IN    ULONG32_T               ulPin2,
  IN    ULONG32_T               ulPin3,
  IN    ULONG32_T               ulPin4,
  OUT   ULONG32_T               *pulHandle
  );


// 输入OpenFirst或者OpenNext返回的设备Handle
// 返回下一个符合条件的精锐E普及版设备
// 如果未找到设备，则返回ELEP_NO_MORE_DEVICE
// 如果找到了相应设备且PIN码正确，则返回值为ELEP_SUCCESS，pulHandle返回设备句柄
// 如果找到了相应设备但PIN码不正确，则返回ELEP_ERR_INVALID_PASSWORD，pulHandle返回设备句柄
// 如果PIN码为0，则不验证PIN码
ULONG32_T ELEPAPI ElepOpenNext(
  IN    ULONG32_T                ulDeviceHandle,
  OUT   ULONG32_T                *pulHandle
  );


// 关闭指定精锐E普及版设备
ULONG32_T ELEPAPI ElepClose(
  IN     ULONG32_T               ulDeviceHandle
  );


// 按照指定偏移和长度读数据区
ULONG32_T ELEPAPI ElepReadMemory(
  IN    ULONG32_T               ulDeviceHandle,
  IN    USHORT16_T              usOffset,
  OUT   UCHAR8_T                *pucBuff,
  IN    USHORT16_T              usDataLen
  );


// 按照指定偏移和长度写数据区
ULONG32_T ELEPAPI ElepWriteMemory(
  IN   ULONG32_T                ulDeviceHandle,
  IN   USHORT16_T               usOffset,
  IN   const UCHAR8_T           *pucBuff,
  IN   USHORT16_T               usDataLen
  );



// 检查指定模块是否有效
ULONG32_T ELEPAPI ElepModuleCheck(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  USHORT16_T                 usModuleIndex
  );

// 递减指定模块的值
ULONG32_T ELEPAPI ElepModuleDecrease(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  USHORT16_T                 usModuleIndex
  );

// 设置指定模块的值、有效性和递减性
ULONG32_T ELEPAPI ElepModuleSet(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  USHORT16_T                 usModuleIndex,
  IN  USHORT16_T                 usModuleVal,
  IN  BOOL_T                     bEnableDecrease
  );


// 根据输入的种子以及模块返回种子码
ULONG32_T  ELEPAPI ElepSeed(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  USHORT16_T                 usModuleIndex,
  IN  const UCHAR8_T             *pucBuff,
  IN  ULONG32_T                  ulBuffLen,
  OUT ULONG32_T                  *pulSeed1,
  OUT ULONG32_T                  *pulSeed2,
  OUT ULONG32_T                  *pulSeed3,
  OUT ULONG32_T                  *pulSeed4
  );
 
  
// 生成短码
ULONG32_T  ELEPAPI  ElepGenModuleUpdate(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  ULONG32_T                  ulTargetSN1,
  IN  ULONG32_T                  ulTargetSN2,
  IN  USHORT16_T                 usModuleIndex,
  IN  USHORT16_T                 usModuleVal,
  IN  BOOL_T                     bEnableDecrease,
  OUT ULONG32_T                  *pulUpdateCode1,
  OUT ULONG32_T                  *pulUpdateCode2,
  OUT ULONG32_T                  *pulUpdateCode3,
  OUT ULONG32_T                  *pulUpdateCode4
  );
  
  
// 生成升级包
ULONG32_T   ELEPAPI ElepGenMemoryUpdate(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  ULONG32_T                  ulTargetSN1,
  IN  ULONG32_T                  ulTargetSN2,
  IN  const UCHAR8_T             *pucContent,
  OUT UCHAR8_T                   *pucUpdateBlock
  );
  
// 用短码升级设备指定模块
ULONG32_T   ELEPAPI ElepModuleUpdate(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  ULONG32_T                  ulUpdateCode1,
  IN  ULONG32_T                  ulUpdateCode2,
  IN  ULONG32_T                  ulUpdateCode3,
  IN  ULONG32_T                  ulUpdateCode4
  );
  
// 用升级包升级设备数据区
ULONG32_T   ELEPAPI  ElepMemoryUpdate(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  const UCHAR8_T             *pucUpdateBlock
  );


// 对指定内容进行RSA数字签名
ULONG32_T   ELEPAPI  ElepSign(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  const UCHAR8_T             *pucContent,
  IN  ULONG32_T                  ulLen,
  OUT UCHAR8_T                   *pucSignature
  );

// 验证RSA数字签名
ULONG32_T   ELEPAPI   ElepVerifySign(
  IN  const UCHAR8_T              *pucPublicKey,
  IN  const UCHAR8_T              *pucContent,
  IN  ULONG32_T                   ulLen,
  IN  const UCHAR8_T              *pucSignature
  );


// 使用指定模块相关密钥对指定内容进行AES加密
ULONG32_T   ELEPAPI    ElepEncrypt(
  IN  ULONG32_T                    ulDeviceHandle,
  IN  USHORT16_T                   usModuleIndex,
  IN  const UCHAR8_T               *pucPlainBuff,
  IN  ULONG32_T                    ulPlainLen,
  OUT UCHAR8_T                     *pucCipherBuff,
  IN  OUT ULONG32_T                *pulCipherLen
  );


// 使用指定模块相关密钥对指定内容进行AES解密
ULONG32_T   ELEPAPI    ElepDecrypt(
  IN  ULONG32_T                    ulDeviceHandle,
  IN  USHORT16_T                   usModuleIndex,
  IN  const UCHAR8_T               *pucCipherBuff,
  IN  ULONG32_T                    ulCipherLen,
  OUT UCHAR8_T                     *pucPlainBuff,
  IN  OUT ULONG32_T                *pulPlainLen
  );


// 修改设备PIN码
ULONG32_T   ELEPAPI     ElepChangePin(
  IN   ULONG32_T                    ulDeviceHandle,
  IN    ULONG32_T                   ulPin1,
  IN    ULONG32_T                   ulPin2,
  IN    ULONG32_T                   ulPin3,
  IN    ULONG32_T                   ulPin4
  );

// 取设备全球唯一序列号
ULONG32_T ELEPAPI       ElepGetDeviceSN(
  IN   ULONG32_T                     ulDeviceHandle,
  IN   ULONG32_T                     *pulDeviceSN1,
  IN   ULONG32_T                     *pulDeviceSN2
  );
 
// 取设备开发商编号  
ULONG32_T ELEPAPI       ElepGetDeveloperNumber(
  IN   ULONG32_T                      ulDeviceHandle,
  IN   ULONG32_T                      *pulDeveloperNumber1,
  IN   ULONG32_T                      *pulDeveloperNumber2
  );


// 取程序库版本号
ULONG32_T  ELEPAPI   ElepGetLibVersion();


#ifdef __cplusplus
}
#endif
#endif
