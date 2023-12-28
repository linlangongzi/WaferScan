#ifndef _COMMONCALCULATE_H_
#define _COMMONCALCULATE_H

/**
* Copyright(c) 2012 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 通用计算函数头文件
* Filename: CommonCalculate.h
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2012-08-16
*/


#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


#ifdef _COMMON_CALCULATE_DLL
	#ifndef DLL_DECL
		#define DLL_DECL __declspec(dllexport)
	#endif
#else
	#ifndef DLL_DECL
		#define DLL_DECL __declspec(dllimport)
	#endif
#endif

/**
* Copyright(c) 2012 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved.
*
* See CommonCalculate.cpp for the implementation of this class.
* Class name: CCommonCalculateApp
*/
class CCommonCalculateApp : public CWinApp
{
public:
	CCommonCalculateApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

float DLL_DECL Data2Gain( WORD nData );
void DLL_DECL dBProcess( const float& db, 
						 WORD& db_0_data, 
						 WORD& db_1_data, 
						 WORD& db_2_data, 
						 WORD& db_3_data );
DWORD DLL_DECL Range2Dot( float VSound, float Range );
float DLL_DECL Dot2Range( float VSound, int DotNo );
float DLL_DECL Dot2MicroSecond( const int& nDot );
int DLL_DECL MicroSecond2Dot( const float& fMicroSecond );
void DLL_DECL RangeProcess( float VSound, float Range, WORD* pInterSpace );
WORD DLL_DECL Gain2Data( float Gain );
WORD DLL_DECL Distance2Data( float nDistance, WORD SoundVelocity );

#endif