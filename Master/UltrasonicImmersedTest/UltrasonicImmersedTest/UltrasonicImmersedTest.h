/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* UltrasonicImmersedTest 应用程序的主头文件
*Filename : UltrasonicImmersedTest.h
*
*@author       Song Chenguang
*@version      1.0         Date: 2011-7-1 
*/
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

// WangQianfeng add start on 2012-11-15
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
using namespace log4cxx;
using namespace log4cxx::helpers;
// WangQianfeng add end on 2012-11-15

// CUltrasonicImmersedTestApp:
// 有关此类的实现，请参阅 UltrasonicImmersedTest.cpp
//
class CUltrasonicImmersedTestView;

class CUltrasonicImmersedTestApp : public CWinApp,
	public CBCGPWorkspace
{
public:
	CUltrasonicImmersedTestApp();

private:
	// 获取A扫视图指针
	CUltrasonicImmersedTestView* GetAScanView();
// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
public:
	AECTHREAD_PARAM m_stThreadParam[MAX_CARD_NO];
	CWinThread** m_pAECWorkThread;
private:
	void InitHardwareConfig();	// 配置卡数和IP
//wangbing modify start on 2014-12-31
public:
	void CalcCoderScale();		// 计算长度与脉冲比值
//wangbing modify end on 2014-12-31
private:
	int m_nCardNum;
	unsigned int nTimeCount;
	
public:
	CString* m_strIPArray;		// 下位机卡IP
	unsigned long m_ElepHandle;	// 加密狗句柄
	CIniParams m_IniParams;		// 加密狗参数
	float m_fCODER_PER_RING_X;	// X单圈脉冲距离比值
	float m_fCODER_PER_RING_Y;	// Y单圈脉冲距离比值
	float m_fCODER_PER_RING_Z;	// Z单圈脉冲距离比值
	float m_fCODER_PER_RING_W1;	// W1单圈脉冲距离比值
	float m_fCODER_PER_RING_A;	// A单圈脉冲距离比值
	float m_fCODER_PER_RING_B;	// B单圈脉冲距离比值
	float m_fCODER_PER_RING_W2;	// W2单圈脉冲距离比值
public:
	// 获取卡个数
	inline int GetCardNum () 
	{
		return m_nCardNum;
	} 
	// 获取卡的IP
	inline CString GetStrIP(const int& nCardIndex)
	{
		if (m_strIPArray != NULL && nCardIndex < m_nCardNum)
		{
			return m_strIPArray[nCardIndex];
		}
		else
			return _T("");
	}
// HuangYaChang add start on 2012-11-24
public:
	afx_msg void OnAppHelp();
// HuangYaChang add end on 2012-11-24
};

extern CUltrasonicImmersedTestApp theApp;