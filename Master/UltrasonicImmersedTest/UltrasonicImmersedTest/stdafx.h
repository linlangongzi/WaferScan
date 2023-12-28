// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ��������Ϊ�ʺ� Windows Me ����߰汾����Ӧֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����


 // �˴�Ҫ����С DB ֧�֡�δѡ���κ���ͼ��

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT





#include <atlbase.h>
#include <afxoledb.h>
#include <atlplus.h>
#include <afxdb.h>

#include <BCGCBProInc.h>
#include <shlobj.h>


#include "resource.h"
#include "const.h"
#include "..\pci9054_dll\plx_lib_x.h"	// plx_dll.dll PCI��������̬���ӿ�6
#include "..\pci9054_dll\p9054_lib.h"
#include "..\pictureNote\pictureNote\pictureNote.h"
#include "..\CommonCalculate\CommonCalculate\CommonCalculate.h"

#include "ars200fpga.h"
#include "DataDefine.h"
#include "PLXCommunication.h"
#include "const.h"
#include <afxsock.h>
#include "netCommand.h"

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "adoEOF") \
	rename("LockTypeEnum", "ADODBLockTypeEnum") \
	rename("DataTypeEnum", "ADODBDataTypeEnum") \
	rename("RecordStatusEnum", "ADODBRecordStatusEnum") \
	rename("ParameterDirectionEnum", "ADODBParameterDirectionEnum") \
	rename("FieldAttributeEnum", "ADODBFieldAttributeEnum") \
	rename("EditModeEnum", "ADODBEditModeEnum") 
//#include <afxdisp.h>
#include <afxtempl.h>
#include "IniParams.h"
#include <afxdisp.h>
#include "CommandSocket.h"
//#include "mdsocx.h"
//#pragma comment(linker,"/STACK:10240000,1024000")
//#pragma comment(linker,"/STACK:81920000,1024000")
#pragma comment(linker,"/STACK:4096000,102400")
//#pragma comment(linker,"/STACK:307200000,1024000")
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


