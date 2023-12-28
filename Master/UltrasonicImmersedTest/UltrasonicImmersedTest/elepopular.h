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

// ����������
#define ELEP_RSA_PUB_KEY_LEN                   132       // RSA��Կ����
#define ELEP_LENGTH_SIGNATURE                  128       // ����ǩ������
#define ELEP_LENGTH_MEMORY_UPDATE_IN           2496      // ���������볤��
#define ELEP_LENGTH_MEMORY_UPDATE_OUT          3968      // �������������

// ���󷵻�ֵ
#define ELEP_SUCCESS                           0x80000000// �ɹ�
#define ELEP_INVALID_PARAMETER                 0x80000001// ��������
#define ELEP_INSUFFICIENT_BUFFER               0x80000002// ����������
#define ELEP_NOT_ENOUGH_MEMORY                 0x80000003// �ڴ治��
#define ELEP_INVALID_DEVICE_HANDLE             0x80000004// �豸�����Ч
#define ELEP_COMM_ERROR                        0x80000005// ͨѶ����
                                                 
#define ELEP_INVALID_SHARE_MODE                0x80000006// ���豸ʱ��ʹ������Ч�Ĺ���ʽ
#define ELEP_UNSUPPORTED_OS                    0x80000007// ����E��֧�ָò���ϵͳ
#define ELEP_ENUMERATE_DEVICE_FAILED           0x80000008// �о��豸ʧ��
#define ELEP_NO_MORE_DEVICE                    0x80000009// û�з����������豸

#define ELEP_INVALID_MODULE                    0x8000000a// ��Чģ��
                                                 
#define ELEP_NOT_RTL_DEVICE                    0x80000024// ����ʱ���豸
#define ELEP_NOT_MASTER_DEVICE                 0x80000025// ��������������
#define ELEP_NOT_USER_DEVICE                   0x80000026// �����û���
#define ELEP_CLOSE_DEVICE_FAILED               0x80000067// �ر��豸ʧ��
#define ELEP_VERIFY_SIGNATURE_ERROR            0x80000068// ��֤ǩ��ʧ�� 

#define ELEP_ERROR_HW_ERROR                    0x80010001// Ӳ������
#define ELEP_ERR_INVALID_PASSWORD              0x80010002// �������������
#define ELEP_ERR_OPT                           0x80010003// һ��д����
#define ELEP_ERR_INVALID_ADDR_OR_SIZE          0x80010004// ��д��ַ�򳤶�����
#define ELEP_ERR_DECREASE                      0x80010005// �ݼ���
#define ELEP_ERR_NO_PRIVILEGE                  0x80010006// ��Ȩ��
#define ELEP_ERR_EXPIRE                        0x80010007// ����������
#define ELEP_INVALID_UPDATE_PKG                0x80010008// ��������Ч
#define ELEP_ERR_INVALID_STATE                 0x80010009// ��Ч״̬

// ��ָ�������̱�ŵĵ�1���豸
// ��������̱��Ϊ0������Կ����̱���ҵ���1���豸
// ���δ�ҵ��豸���򷵻�ELEP_NO_MORE_DEVICE
// ����ҵ�����Ӧ�豸��PIN����ȷ���򷵻�ֵΪELEP_SUCCESS��pulHandle�����豸���
// ����ҵ�����Ӧ�豸��PIN�벻��ȷ���򷵻�ELEP_ERR_INVALID_PASSWORD��pulHandle�����豸���
// ���PIN��Ϊ0������֤PIN��
ULONG32_T ELEPAPI ElepOpenFirst(
  IN    ULONG32_T               ulDevMajNumber,
  IN    ULONG32_T               ulDevMinNumber,
  IN    ULONG32_T               ulPin1,
  IN    ULONG32_T               ulPin2,
  IN    ULONG32_T               ulPin3,
  IN    ULONG32_T               ulPin4,
  OUT   ULONG32_T               *pulHandle
  );


// ����OpenFirst����OpenNext���ص��豸Handle
// ������һ�����������ľ���E�ռ����豸
// ���δ�ҵ��豸���򷵻�ELEP_NO_MORE_DEVICE
// ����ҵ�����Ӧ�豸��PIN����ȷ���򷵻�ֵΪELEP_SUCCESS��pulHandle�����豸���
// ����ҵ�����Ӧ�豸��PIN�벻��ȷ���򷵻�ELEP_ERR_INVALID_PASSWORD��pulHandle�����豸���
// ���PIN��Ϊ0������֤PIN��
ULONG32_T ELEPAPI ElepOpenNext(
  IN    ULONG32_T                ulDeviceHandle,
  OUT   ULONG32_T                *pulHandle
  );


// �ر�ָ������E�ռ����豸
ULONG32_T ELEPAPI ElepClose(
  IN     ULONG32_T               ulDeviceHandle
  );


// ����ָ��ƫ�ƺͳ��ȶ�������
ULONG32_T ELEPAPI ElepReadMemory(
  IN    ULONG32_T               ulDeviceHandle,
  IN    USHORT16_T              usOffset,
  OUT   UCHAR8_T                *pucBuff,
  IN    USHORT16_T              usDataLen
  );


// ����ָ��ƫ�ƺͳ���д������
ULONG32_T ELEPAPI ElepWriteMemory(
  IN   ULONG32_T                ulDeviceHandle,
  IN   USHORT16_T               usOffset,
  IN   const UCHAR8_T           *pucBuff,
  IN   USHORT16_T               usDataLen
  );



// ���ָ��ģ���Ƿ���Ч
ULONG32_T ELEPAPI ElepModuleCheck(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  USHORT16_T                 usModuleIndex
  );

// �ݼ�ָ��ģ���ֵ
ULONG32_T ELEPAPI ElepModuleDecrease(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  USHORT16_T                 usModuleIndex
  );

// ����ָ��ģ���ֵ����Ч�Ժ͵ݼ���
ULONG32_T ELEPAPI ElepModuleSet(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  USHORT16_T                 usModuleIndex,
  IN  USHORT16_T                 usModuleVal,
  IN  BOOL_T                     bEnableDecrease
  );


// ��������������Լ�ģ�鷵��������
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
 
  
// ���ɶ���
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
  
  
// ����������
ULONG32_T   ELEPAPI ElepGenMemoryUpdate(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  ULONG32_T                  ulTargetSN1,
  IN  ULONG32_T                  ulTargetSN2,
  IN  const UCHAR8_T             *pucContent,
  OUT UCHAR8_T                   *pucUpdateBlock
  );
  
// �ö��������豸ָ��ģ��
ULONG32_T   ELEPAPI ElepModuleUpdate(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  ULONG32_T                  ulUpdateCode1,
  IN  ULONG32_T                  ulUpdateCode2,
  IN  ULONG32_T                  ulUpdateCode3,
  IN  ULONG32_T                  ulUpdateCode4
  );
  
// �������������豸������
ULONG32_T   ELEPAPI  ElepMemoryUpdate(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  const UCHAR8_T             *pucUpdateBlock
  );


// ��ָ�����ݽ���RSA����ǩ��
ULONG32_T   ELEPAPI  ElepSign(
  IN  ULONG32_T                  ulDeviceHandle,
  IN  const UCHAR8_T             *pucContent,
  IN  ULONG32_T                  ulLen,
  OUT UCHAR8_T                   *pucSignature
  );

// ��֤RSA����ǩ��
ULONG32_T   ELEPAPI   ElepVerifySign(
  IN  const UCHAR8_T              *pucPublicKey,
  IN  const UCHAR8_T              *pucContent,
  IN  ULONG32_T                   ulLen,
  IN  const UCHAR8_T              *pucSignature
  );


// ʹ��ָ��ģ�������Կ��ָ�����ݽ���AES����
ULONG32_T   ELEPAPI    ElepEncrypt(
  IN  ULONG32_T                    ulDeviceHandle,
  IN  USHORT16_T                   usModuleIndex,
  IN  const UCHAR8_T               *pucPlainBuff,
  IN  ULONG32_T                    ulPlainLen,
  OUT UCHAR8_T                     *pucCipherBuff,
  IN  OUT ULONG32_T                *pulCipherLen
  );


// ʹ��ָ��ģ�������Կ��ָ�����ݽ���AES����
ULONG32_T   ELEPAPI    ElepDecrypt(
  IN  ULONG32_T                    ulDeviceHandle,
  IN  USHORT16_T                   usModuleIndex,
  IN  const UCHAR8_T               *pucCipherBuff,
  IN  ULONG32_T                    ulCipherLen,
  OUT UCHAR8_T                     *pucPlainBuff,
  IN  OUT ULONG32_T                *pulPlainLen
  );


// �޸��豸PIN��
ULONG32_T   ELEPAPI     ElepChangePin(
  IN   ULONG32_T                    ulDeviceHandle,
  IN    ULONG32_T                   ulPin1,
  IN    ULONG32_T                   ulPin2,
  IN    ULONG32_T                   ulPin3,
  IN    ULONG32_T                   ulPin4
  );

// ȡ�豸ȫ��Ψһ���к�
ULONG32_T ELEPAPI       ElepGetDeviceSN(
  IN   ULONG32_T                     ulDeviceHandle,
  IN   ULONG32_T                     *pulDeviceSN1,
  IN   ULONG32_T                     *pulDeviceSN2
  );
 
// ȡ�豸�����̱��  
ULONG32_T ELEPAPI       ElepGetDeveloperNumber(
  IN   ULONG32_T                      ulDeviceHandle,
  IN   ULONG32_T                      *pulDeveloperNumber1,
  IN   ULONG32_T                      *pulDeveloperNumber2
  );


// ȡ�����汾��
ULONG32_T  ELEPAPI   ElepGetLibVersion();


#ifdef __cplusplus
}
#endif
#endif
