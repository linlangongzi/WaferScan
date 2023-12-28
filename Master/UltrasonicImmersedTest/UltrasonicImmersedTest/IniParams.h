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
	wchar_t wcMechineType[32];	// 设备型号
	wchar_t wcMechineNo[32];	// 设备序列号
	wchar_t wcUserCompany[32];	// 客户名称
	CTime   SaleTime;			// 出厂日期
	int		nCardNum;			// 卡数
	BYTE    FieldValue1[4];		// 模块1,2 IP
	BYTE    FieldValue2[4];
	BYTE    FieldValue3[4];
	BYTE    FieldValue4[4];	
	float fXRange;				// 扫查X范围
	float fYRange;				// 扫查Y范围
	float fZRange;				// 扫查Z范围
	int nXCoderPerRingNum;		// X编码器单圈脉冲数
	int nYCoderPerRingNum;		// Y编码器单圈脉冲数
	int nZCoderPerRingNum;		// Z编码器单圈脉冲数
	int nW1CoderPerRingNum;		// W1编码器单圈脉冲数
	int nACoderPerRingNum;		// A编码器单圈脉冲数
	int nBCoderPerRingNum;		// B编码器单圈脉冲数
	int nW2CoderPerRingNum;		// W2编码器单圈脉冲数
	float fXDiameter;			// X编码器直径
	float fYDiameter;			// Y编码器直径
	float fZDiameter;			// Z编码器直径
	float fW1Diameter;			// W1直径
	float fADiameter;			// A直径
	float fBDiameter;			// B直径
	float fW2Diameter;			// W2直径
	float fXJianSuBi;			// X减速比
	float fYJianSuBi;			// Y减速比
	float fZJianSuBi;			// Z减速比
	float fW1JianSuBi;			// W1减速比
	float fAJianSuBi;			// A减速比
	float fBJianSuBi;			// B减速比
	float fW2JianSuBi;			// W2减速比
	// Song Chenguang add start on 2012-12-27
	BOOL bXReverse;				// X方向反转
	BOOL bYReverse;				// Y方向反转
	BOOL bZReverse;				// Z方向反转
	// Song Chenguang add start on 2012-12-27
	// Song Chenguang add start on 2013-03-28
	float fXCorrect;			// X方向修正值
	float fYCorrect;			// Y方向修正值
	float fZCorrect;			// Z方向修正值
	float fW1Correct;			// W1方向修正值
	float fACorrect;			// A方向修正值
	float fBCorrect;			// B方向修正值
	float fW2Correct;			// W2方向修正值
	// Song Chenguang add end on 2013-03-28
	// wangbingfu add start on 2014-09-09
 	BOOL bXEnable;				// X轴使能标志
 	BOOL bYEnable;				// Y轴使能标志
 	BOOL bZEnable;				// Z轴使能标志
 	BOOL bAEnable;				// A轴使能标志
 	BOOL bBEnable;				// B轴使能标志
 	BOOL bW1Enable;				// W1轴使能标志
 	BOOL bW2Enable;				// W2轴使能标志
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
