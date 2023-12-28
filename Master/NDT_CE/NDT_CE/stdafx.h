// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


// Windows Header Files:
#include <windows.h>
#include <Commctrl.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "const.h"

#define TIMER_PROCCESS	1
#define TIMER_PRESS		2
#define TIMER_CLOCK		3
#define TIMER_GATE_DEFECT_DISPLAY	4
#define TIMER_DAC_DEFECT_DISPLAY	5
#define TIMER_RECORD	6
#define TIMER_REPLAY	7
#define TIMER_CHECK_DEVICE	8
#define TIMER_ZHUJIAN_FAIZHI_DISPLAY 9
#define TIMER_FUHECAILIAO_DISPLAY 10
#define TIMER_LIEWENCEGAO		11
#define TIMER_TIME_DISPLAY		12

///////////////////////////////////////////////////////////////////////


#define dim(x) (sizeof(x) / sizeof(x[0]))
// dlg function
typedef struct{
	UINT uCode;
	BOOL (*functionName)(HWND, UINT, WPARAM, LPARAM);
}dlgMessageProc;

typedef struct tag_CURVEPOINT_DATA{
	float data;
	float serial;//512个波形数据中选取的序号
}CURVEPOINT_DATA;
/*
typedef struct DGS_CURVE_S_tag{
	BOOL bSwitch[DAC_CURVE_NUM - 1];
	float fPingDing;
	float fDingLiang;
	float fPanFei;
	float fFirstGain;
	float fOldRang;
	WORD wOldSerial[MAX_CURVE_POINT_NUM];
	WORD selectedDgsPointDataNum[DAC_CURVE_NUM];
	CURVEPOINT_DATA selectedDgsPointData[DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];
}DGS_CURVE_S;
typedef struct AVG_CURVE_S_tag{
	BOOL bSwitch;
	float fFai1;
	float fFai2;
	float fFai3;
	float fFirstGain;
	float fOldRange;
	WORD wOldSerial[MAX_CURVE_POINT_NUM];
	WORD selectedPointDataNum;
	CURVEPOINT_DATA selectedPointData[AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];
}AVG_CURVE_S;
*/
class DGS_CURVE_S{
	public:
		BOOL bSwitch[DAC_CURVE_NUM - 1];
		float fPingDing;
		float fDingLiang;
		float fPanFei;
		float fFirstGain;
		float fOldRang;
		short wOldSerial[MAX_CURVE_POINT_NUM];
		WORD selectedDgsPointDataNum[DAC_CURVE_NUM];
		CURVEPOINT_DATA selectedDgsPointData[DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];
	public:
		DGS_CURVE_S()
		{
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)
			{
				bSwitch[i] = FALSE;
			}
			fPingDing = -3;
			fDingLiang = 0;
			fPanFei = 3;
			fFirstGain = 0;
			fOldRang = 0;
			memset(wOldSerial, 0, sizeof(WORD)*MAX_CURVE_POINT_NUM);
			memset(selectedDgsPointDataNum, 0, sizeof(WORD)*DAC_CURVE_NUM);
			memset(selectedDgsPointData, 0, sizeof(CURVEPOINT_DATA)*DAC_CURVE_NUM*MAX_CURVE_POINT_NUM);
		}

		inline DGS_CURVE_S& operator=(const DGS_CURVE_S& rhs)
		{
			if(this == &rhs)
				return * this;
			memcpy(bSwitch, rhs.bSwitch, sizeof(BOOL)*(DAC_CURVE_NUM - 1));
			fPingDing = rhs.fPingDing;
			fDingLiang = rhs.fDingLiang;
			fPanFei = rhs.fPanFei;
			fFirstGain = rhs.fFirstGain;
			fOldRang = rhs.fOldRang;
			memcpy(wOldSerial, rhs.wOldSerial, sizeof(WORD)*MAX_CURVE_POINT_NUM);
			memcpy(selectedDgsPointDataNum, rhs.selectedDgsPointDataNum, sizeof(WORD)*DAC_CURVE_NUM);
			memcpy(selectedDgsPointData, rhs.selectedDgsPointData, sizeof(CURVEPOINT_DATA)*DAC_CURVE_NUM*MAX_CURVE_POINT_NUM);
			
			return *this;
		}	
};
class AVG_CURVE_S{
	public:
		BOOL bSwitch;
		float fFai1;
		float fFai2;
		float fFai3;
		float fFirstGain;
		float fOldRange;
		short wOldSerial[MAX_CURVE_POINT_NUM];
		WORD selectedPointDataNum;
		CURVEPOINT_DATA selectedPointData[AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];
	public:
		AVG_CURVE_S()
		{
			bSwitch = FALSE;
			fFai1 = 1;
			fFai2 = 0;
			fFai3 = 0;
			fFirstGain = 0;
			fOldRange = 0;
			memset(wOldSerial, 0, sizeof(WORD)*MAX_CURVE_POINT_NUM);
			selectedPointDataNum = 0;
			memset(selectedPointData, 0, sizeof(CURVEPOINT_DATA)*(AVG_CURVE_NUM + 1)*MAX_CURVE_POINT_NUM);
		}

		inline AVG_CURVE_S& operator=(const AVG_CURVE_S& rhs)
		{
			if(this == &rhs)
				return * this;
			bSwitch = rhs.bSwitch;
			fFai1 = rhs.fFai1;
			fFai2 = rhs.fFai2;
			fFai3 = rhs.fFai3;
			fFirstGain = rhs.fFirstGain;
			fOldRange = rhs.fOldRange;
			memcpy(wOldSerial, rhs.wOldSerial, sizeof(WORD)*MAX_CURVE_POINT_NUM);
			selectedPointDataNum = rhs.selectedPointDataNum;
			memcpy(selectedPointData, rhs.selectedPointData, sizeof(CURVEPOINT_DATA)*(AVG_CURVE_NUM + 1)*MAX_CURVE_POINT_NUM);
			
			return *this;
		}	
};

//#define _NDT_TEST  1	// 测试用宏
///////////////////////////////////////////////////////////////////////
// Local Header Files

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
