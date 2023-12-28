/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ˮ������ͷ�ļ�
*Filename : CUltrasonicImmersedTestDoc.h
*
*@author       Song Chenguang
*@version      1.0         Date: 2011-7-1 
*/

#pragma once
#include <queue>

class CDibImage;
class CUltrasonicImmersedTestDoc : public CDocument
{
protected: // �������л�����
	CUltrasonicImmersedTestDoc();
	DECLARE_DYNCREATE(CUltrasonicImmersedTestDoc)

// ����
public:
//	CH_CONFIG m_stAllCH_CONFIG[CHANNEL_NUM];
//	CH_CONFIG m_stAllCH_CONFIG_BackUp[CHANNEL_NUM];
	CSCAN_ARS200FPGA_SETONE m_stAllCH_CONFIG[CHANNEL_NUM];
	CSCAN_ARS200FPGA_SETONE m_stAllCH_CONFIG_BackUp[CHANNEL_NUM];

	CH_CONFIG_GLOBAL ch_config_global;

	CList<CFeatureInfo, CFeatureInfo&> m_lstFlawData[CHANNEL_NUM];
	CList<CFeatureInfo, CFeatureInfo&> m_lstFlawDataOriginal[CHANNEL_NUM];	// 2011-11-18ԭʼ����
	CList<CPicData, CPicData&> m_lstAScanData[CHANNEL_NUM];
//	std::queue<CPicData> m_qAScanData[CHANNEL_NUM];		//2012-03-26ʵʱAɨ
	std::queue<CPicData_L> m_qAScanData[CHANNEL_NUM];		//2013-12-19 ʵʱAɨ Song Chenguang add on 2013-12-19
	std::queue<CFeatureInfo> m_qCScanData[CHANNEL_NUM];	//2012-03-26ʵʱCɨ
	CList<CFeatureInfo, CFeatureInfo&> m_lstFlawDataTemp[CHANNEL_NUM];	// wangbingfu add at 2012-10-12
	CString m_FilePath;
	CString m_FilePath1; //����ʱ Aɨ�������ļ�
	float fPlusEqualize[8][4];//8��Ƶ����˫ͨ�����油��  yachang.huang add on 2014-06-18
protected:
	HANDLE m_hDIB;
//	CDibImage* m_pDibImage;
//	CPalette* m_palDIB;
	CSize m_sizeDoc;

// ����
public:
	// ��ʼ��ͨ����ز���
	BOOL InitChannel();
// 	HANDLE GetHDIB() const
// 	{ return m_hDIB; }
// 	CDibImage* GetDibImage() const
// 	{ return m_pDibImage; }
// 	CPalette* GetDocPalette() const
// 	{ return m_palDIB; }
	CSize GetDocSize() const
	{ return m_sizeDoc; }


// 	void ReplaceHDIB(HANDLE hDIB);
// 	void InitDIBData();
private:
	// ��ȡ��Ӳ�����͵�����
//	void INIT_CONFIG(CH_CONFIG *pCHX_INIT_CONFIG, CH_CONFIG_GLOBAL *pCHX_CONFIG_GLOBAL);
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CUltrasonicImmersedTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSaveAScan();
};


