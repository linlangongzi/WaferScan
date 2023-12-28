// IniParams.h: interface for the CIniParams class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIPARAMS_H__2A4D961B_9BA9_4343_A53C_00AD0B135993__INCLUDED_)
#define AFX_INIPARAMS_H__2A4D961B_9BA9_4343_A53C_00AD0B135993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct DogInfo
{
	wchar_t wcMechineType[32];	// �豸�ͺ�
	wchar_t wcMechineNo[32];	// �豸���к�
	wchar_t wcUserCompany[32];	// �ͻ�����
	CTime   SaleTime;			// ��������
	int		nCardNum;			// ����
	BYTE    FieldValue1[4];		// ģ��1,2 IP
	BYTE    FieldValue2[4];
	BYTE    FieldValue3[4];
	BYTE    FieldValue4[4];	
	float fXRange;				// ɨ��X��Χ
	float fYRange;				// ɨ��Y��Χ
	float fZRange;				// ɨ��Z��Χ
	int nXCoderPerRingNum;		// X��������Ȧ������
	int nYCoderPerRingNum;		// Y��������Ȧ������
	int nZCoderPerRingNum;		// Z��������Ȧ������
	int nW1CoderPerRingNum;		// W1��������Ȧ������
	int nACoderPerRingNum;		// A��������Ȧ������
	int nBCoderPerRingNum;		// B��������Ȧ������
	int nW2CoderPerRingNum;		// W2��������Ȧ������
	float fXDiameter;			// X������ֱ��
	float fYDiameter;			// Y������ֱ��
	float fZDiameter;			// Z������ֱ��
	float fW1Diameter;			// W1ֱ��
	float fADiameter;			// Aֱ��
	float fBDiameter;			// Bֱ��
	float fW2Diameter;			// W2ֱ��
	float fXJianSuBi;			// X���ٱ�
	float fYJianSuBi;			// Y���ٱ�
	float fZJianSuBi;			// Z���ٱ�
	float fW1JianSuBi;			// W1���ٱ�
	float fAJianSuBi;			// A���ٱ�
	float fBJianSuBi;			// B���ٱ�
	float fW2JianSuBi;			// W2���ٱ�
	// Song Chenguang add start on 2012-12-27
	BOOL bXReverse;				// X����ת
	BOOL bYReverse;				// Y����ת
	BOOL bZReverse;				// Z����ת
	// Song Chenguang add start on 2012-12-27
	// Song Chenguang add start on 2013-03-28
	float fXCorrect;			// X��������ֵ
	float fYCorrect;			// Y��������ֵ
	float fZCorrect;			// Z��������ֵ
	float fW1Correct;			// W1��������ֵ
	float fACorrect;			// A��������ֵ
	float fBCorrect;			// B��������ֵ
	float fW2Correct;			// W2��������ֵ
	// Song Chenguang add end on 2013-03-28
	// wangbingfu add start on 2014-09-09
 	BOOL bXEnable;				// X��ʹ�ܱ�־
 	BOOL bYEnable;				// Y��ʹ�ܱ�־
 	BOOL bZEnable;				// Z��ʹ�ܱ�־
 	BOOL bAEnable;				// A��ʹ�ܱ�־
 	BOOL bBEnable;				// B��ʹ�ܱ�־
 	BOOL bW1Enable;				// W1��ʹ�ܱ�־
 	BOOL bW2Enable;				// W2��ʹ�ܱ�־
	int nCount;
	// wangbingfu add end on 2014-09-09
};

class CIniParams  
{
public:
	CIniParams();
	virtual ~CIniParams();
	BOOL ReadIniData();
	BOOL WriteIniData();
public:
	// machine params
	DogInfo m_info;
};

#endif // !defined(AFX_INIPARAMS_H__2A4D961B_9BA9_4343_A53C_00AD0B135993__INCLUDED_)
