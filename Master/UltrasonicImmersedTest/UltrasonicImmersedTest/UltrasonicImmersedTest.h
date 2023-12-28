/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* UltrasonicImmersedTest Ӧ�ó������ͷ�ļ�
*Filename : UltrasonicImmersedTest.h
*
*@author       Song Chenguang
*@version      1.0         Date: 2011-7-1 
*/
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

// WangQianfeng add start on 2012-11-15
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
using namespace log4cxx;
using namespace log4cxx::helpers;
// WangQianfeng add end on 2012-11-15

// CUltrasonicImmersedTestApp:
// �йش����ʵ�֣������ UltrasonicImmersedTest.cpp
//
class CUltrasonicImmersedTestView;

class CUltrasonicImmersedTestApp : public CWinApp,
	public CBCGPWorkspace
{
public:
	CUltrasonicImmersedTestApp();

private:
	// ��ȡAɨ��ͼָ��
	CUltrasonicImmersedTestView* GetAScanView();
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
public:
	AECTHREAD_PARAM m_stThreadParam[MAX_CARD_NO];
	CWinThread** m_pAECWorkThread;
private:
	void InitHardwareConfig();	// ���ÿ�����IP
//wangbing modify start on 2014-12-31
public:
	void CalcCoderScale();		// ���㳤���������ֵ
//wangbing modify end on 2014-12-31
private:
	int m_nCardNum;
	unsigned int nTimeCount;
	
public:
	CString* m_strIPArray;		// ��λ����IP
	unsigned long m_ElepHandle;	// ���ܹ����
	CIniParams m_IniParams;		// ���ܹ�����
	float m_fCODER_PER_RING_X;	// X��Ȧ��������ֵ
	float m_fCODER_PER_RING_Y;	// Y��Ȧ��������ֵ
	float m_fCODER_PER_RING_Z;	// Z��Ȧ��������ֵ
	float m_fCODER_PER_RING_W1;	// W1��Ȧ��������ֵ
	float m_fCODER_PER_RING_A;	// A��Ȧ��������ֵ
	float m_fCODER_PER_RING_B;	// B��Ȧ��������ֵ
	float m_fCODER_PER_RING_W2;	// W2��Ȧ��������ֵ
public:
	// ��ȡ������
	inline int GetCardNum () 
	{
		return m_nCardNum;
	} 
	// ��ȡ����IP
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