/**
* Copyright(c) 2012 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 通用计算函数实现文件
* Filename: CommonCalculate.cpp
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2012-08-16
*/


#include "stdafx.h"
#include "CommonCalculate.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float YIQI_ADJUST = 0.925f;		//???

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CCommonCalculateApp

BEGIN_MESSAGE_MAP(CCommonCalculateApp, CWinApp)
END_MESSAGE_MAP()


// CCommonCalculateApp construction

CCommonCalculateApp::CCommonCalculateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCommonCalculateApp object

CCommonCalculateApp theApp;

// CCommonCalculateApp initialization

BOOL CCommonCalculateApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/** 
* ???
* @param     nData [in]:  ???
* @return    float
* @since     1.0
*/
float Data2Gain( WORD nData )
{
	float Gain;
	if ( nData > GAIN_DATA_MAX )
	{
		Gain = GAIN_MAX;
	}
	else
	{
		Gain =  static_cast<float>(( nData * 3.0 / 20.0 ) / AEC_AMP_LINEAR_PARA_A );
	}

	return Gain;
}
/** 
* ???
* @param     db		   [in]:  ???
* @param     db_0_data [in]:  ???
* @param     db_1_data [in]:  ???
* @param     db_2_data [in]:  ???
* @param     db_3_data [in]:  ???
* @return    void
* @since     1.0
*/
void dBProcess( const float& db, 
			    WORD& db_0_data,
			    WORD& db_1_data, 
			    WORD& db_2_data, 
			    WORD& db_3_data )
{
	if ( db < 0 )
		return;

	float db_in = static_cast<float>( db * YIQI_ADJUST - 10.0 );
	float db_1 = 0.0f;
	float db_2 = 0.0f;
	float db_3 = 0.0f;

	if ( db_in < -10 )
	{	
		db_1 = -10;
		db_2 = 1;
		db_3 = 1;
	}
	else if ( db_in < 3 )
	{	
		db_1 = db_in - 2;
		db_2 = 1;
		db_3 = 1;
	}
	else if ( db_in < 107 )
	{
		db_1 = static_cast<float>( db_in / 3.0 );
		db_2 = static_cast<float>( db_in / 3.0 ); 
		db_3 = static_cast<float>( db_in / 3.0 ); 
	}
	else
	{	
		db_1 = 30;
		db_2 = 39;
		db_3 = 39;
	}

	db_0_data = 128;
	db_1_data = WORD(( db_1 + 14.0 ) * 16.0 / 3.0 );
	db_2_data = WORD( 20.0 * (db_2 - 0.8) / 3.0 );
	db_3_data = WORD( 20.0 * (db_3 - 0.8) / 3.0 );

}

/** 
* 距离值脉冲值转换
* @param     VSound		[in]:  声速
* @param     Range		[in]:  范围
* @return    转换结果值
* @since     1.0
*/
DWORD Range2Dot( float VSound, float Range )
{
	DWORD temp;
	temp = static_cast<DWORD>( Range * 2 / 1000.0 / VSound * ADFREQUENCY * 1000000.0 );
	return temp;
}

/** 
* 脉冲值距离值转换
* @param     VSound		[in]:  声速
* @param     Range		[in]:  范围
* @return    转换结果值
* @since     1.0
*/
float Dot2Range( float VSound, int DotNo )
{
	float temp;
	temp = static_cast<float>( DotNo / 2.0 * 1000.0 * VSound / float( ADFREQUENCY ) / 1000000.0 ); 
	return temp;
}
/** 
* 脉冲值微秒转换
* @param     nDot		[in]:  脉冲
* @return    转换结果值
* @since     1.0
*/
float Dot2MicroSecond( const int& nDot )
{
	return float( nDot ) / ADFREQUENCY;
}
/** 
* 微秒脉冲值转换
* @param     fMicroSecond		[in]:  微秒
* @return    转换结果值
* @since     1.0
*/
int MicroSecond2Dot( const float& fMicroSecond )
{
	int ret = static_cast<int>( fMicroSecond * 1 * ADFREQUENCY );

	if ( ret == 0 )
		ret = 1;
	return ret;
}
/** 
* 范围处理
* @param     VSound		[in]:  声速
* @param     Range		[in]:  范围
* @param     pInterSpace[in]:  间距
* @return    void
* @since     1.0
*/
void RangeProcess( float VSound,		 // VSound--------------m/s
				   float Range,		     // Range --------------m
				   WORD* pInterSpace )   // pInterSpace ---------
{
	int nTotal= (int)( Range * 2 / 1000.0 / VSound * ADFREQUENCY * 1000000.0 );
	if ( nTotal < MONITOR_SIZE )
	{
		for ( int i = 0; i < 512; i++ )
		{
			pInterSpace[i] = 0;
		}
	}
	else
	{
		int nInter = nTotal / 512;
		int nReset = nTotal % 512;
		int k = 1;
		for ( int i = 0; i < 512; i++ )
		{
			if ( nInter < 1 )
				nInter = 1;
			pInterSpace[i] = nInter - 1;
		}


		for ( int i = 0; i < 512; i++ )
		{
			float temp = static_cast<float>( i * 1.0 * nReset / 511 );
			if ( temp >= k )
			{
				pInterSpace[i] = pInterSpace[i] + 1;
				k++;
			}
			else
			{
				;
			}
		}
	}
}
/** 
* 增益转换
* @param     Gain		[in]:  增益
* @return    转换结果值
* @since     1.0
*/
WORD Gain2Data( float Gain )
{
	WORD nData;

	if ( Gain <= GAIN_MIN )
	{
		nData = GAIN_DATA_MIN; 
	}
	else if ( Gain > GAIN_MAX )
	{
		nData = GAIN_DATA_MAX;
	}
	else
	{
		int tmp = (int)(( Gain * 20.0 / 3.0 * AEC_AMP_LINEAR_PARA_A ) + 0.5 );
		nData = static_cast<WORD>( tmp );
	}


	return nData;
}
/** 
* 距离转换
* @param     nDistance		[in]:  距离
* @param     SoundVelocity	[in]:  声速
* @return    转换结果值
* @since     1.0
*/
WORD Distance2Data( float nDistance, WORD SoundVelocity )
{
	WORD nData;
	nData = (WORD)( nDistance * 100000.0 / SoundVelocity );
	return nData;
}